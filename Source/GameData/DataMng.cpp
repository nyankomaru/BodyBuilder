#include "DataMng.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <windows.h>

//------------------------------------------------------------
// グローバル変数（※不要なら削除可）
// ※本クラス内の mDayData / mStatusData とは別に存在している。
//   競合防止のため基本的には DataMng 内のメンバーを使うことを推奨。
//------------------------------------------------------------
static DayData gDayData;
static StatusData gStatusData;

//------------------------------------------------------------
// ■ 初期化処理
// 各データクラス（DayData, StatusData, Contest）を初期化。
// ゲーム開始時やデータロード失敗時のフォールバックとして呼び出される。
//------------------------------------------------------------
void DataMng::Init() {
    mDayData.Init();
    mStatusData.Init();
    mContestData.Init();
}

//------------------------------------------------------------
// ■ 更新処理
// 必要に応じて日付や状態データの更新を行う。
// 今回は DayData のみ毎フレーム更新。
//------------------------------------------------------------
void DataMng::Update() {
    mDayData.Update();
}

//------------------------------------------------------------
// ■ 終了処理
// データマネージャ終了時の後始末。
// 現時点では特に処理なし（将来的にリソース解放などに使用可能）。
//------------------------------------------------------------
void DataMng::Term() {

}

//------------------------------------------------------------
// ■ データ保存処理
// ゲームデータを指定ファイルに保存する。
// ファイルは実行ファイルと同じディレクトリに作成される。
//------------------------------------------------------------
void DataMng::Save(const char* filename) {
    // --- 実行ファイルのパスを取得 ---
    char exePath[MAX_PATH];
    DWORD len = GetModuleFileNameA(nullptr, exePath, MAX_PATH);
    if (len == 0 || len == MAX_PATH) {
        std::cerr << "Failed to get executable path." << std::endl;
        return;
    }

    // --- ディレクトリ部分を抽出 ---
    std::string dir(exePath);
    size_t pos = dir.find_last_of("\\/");
    if (pos != std::string::npos) {
        dir = dir.substr(0, pos + 1);  // 最後のスラッシュまで含める
    }

    // --- 保存ファイルの絶対パスを生成 ---
    std::string fullPath = dir + filename;

    // --- ファイルを開く ---
    std::ofstream ofs(fullPath.c_str(), std::ios::out | std::ios::trunc);
    if (!ofs.is_open()) {
        std::cerr << "Failed to open save file: " << fullPath << std::endl;
        return;
    }

    //--------------------------------------------------------
    // データ内容を書き出し
    //--------------------------------------------------------
    ofs << "CurrentDay=" << mDayData.GetCurrentDay() << "\n";
    ofs << "ActionsToday=" << mDayData.GetActionsToday() << "\n";

    ofs << "Endurance=" << mStatusData.GetEndurance() << "\n";
    ofs << "Chest=" << mStatusData.GetChest() << "\n";
    ofs << "Back=" << mStatusData.GetBack() << "\n";
    ofs << "Arm=" << mStatusData.GetArm() << "\n";
    ofs << "Leg=" << mStatusData.GetLeg() << "\n";

    ofs << "ContestScore=" << mContestData.GetScore() << "\n";
    ofs << "ContestRank=" << mContestData.GetRank() << "\n";

    ofs.close();

    std::cout << "Save completed: " << fullPath << std::endl;
}

//------------------------------------------------------------
// ■ データ読み込み処理
// 指定ファイルからセーブデータを読み込む。
// 存在しない場合は初期化してフォールバック。
//------------------------------------------------------------
void DataMng::Load(const char* filename) {
    // --- 実行ファイルのディレクトリを取得 ---
    char exePath[MAX_PATH];
    DWORD len = GetModuleFileNameA(nullptr, exePath, MAX_PATH);
    if (len == 0 || len == MAX_PATH) {
        std::cerr << "Failed to get executable path." << std::endl;
        Init();  // フォールバック初期化
        return;
    }

    // --- ディレクトリ部分を抽出 ---
    std::string dir(exePath);
    size_t pos = dir.find_last_of("\\/");
    if (pos != std::string::npos) {
        dir = dir.substr(0, pos + 1); // 最後の \ まで
    }

    // --- フルパス生成 ---
    std::string fullPath = dir + filename;

    // --- ファイルを開く ---
    std::ifstream ifs(fullPath.c_str());
    if (!ifs.is_open()) {
        std::cerr << "Save file not found: " << fullPath << std::endl;
        Init();  // 存在しない場合は初期化
        return;
    }

    //--------------------------------------------------------
    // ファイルを1行ずつ解析
    // 各行は "キー=値" 形式で格納されている。
    //--------------------------------------------------------
    bool hasData = false;
    std::string line;

    while (std::getline(ifs, line)) {
        if (line.empty()) continue;
        hasData = true;

        std::istringstream iss(line);
        std::string key, value;

        if (std::getline(iss, key, '=') && std::getline(iss, value)) {
            // --- 各キーに対応する変数へ反映 ---
            if (key == "CurrentDay")       mDayData.SetCurrentDay(std::stoi(value));
            else if (key == "ActionsToday") mDayData.SetActionsToday(std::stoi(value));

            else if (key == "Endurance")   mStatusData.SetEndurance(std::stoi(value));
            else if (key == "Chest")       mStatusData.SetChest(std::stoi(value));
            else if (key == "Back")        mStatusData.SetBack(std::stoi(value));
            else if (key == "Arm")         mStatusData.SetArm(std::stoi(value));
            else if (key == "Leg")         mStatusData.SetLeg(std::stoi(value));

            else if (key == "ContestScore") mContestData.SetScore(std::stoi(value));
            else if (key == "ContestRank")  mContestData.SetRank(std::stoi(value));
        }
    }

    ifs.close();

    //--------------------------------------------------------
    // 読み込み結果の確認
    //--------------------------------------------------------
    if (!hasData) {
        std::cerr << "Save file was empty, initializing defaults." << std::endl;
        Init();
    }
    else {
        std::cout << "Loaded save file successfully: " << fullPath << std::endl;
    }
}

//------------------------------------------------------------
// ■ データ削除処理
// 指定ファイルを空ファイルとして上書きし、
// 実質的にセーブデータを削除する。
//------------------------------------------------------------
void DataMng::Delete(const char* filename) {
    std::ofstream ofs(filename, std::ios::trunc); // 空ファイルで上書き
    ofs.close();
}