#include "TimeMng.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <random>

//-----------------------------------------------------------------------------
// ■ TimeMng::Init
//    ファイルからタイムパターンをロードして初期化
//    各パターンは TimeData 構造体に格納される
//-----------------------------------------------------------------------------
void TimeMng::Init(const char* filename)
{
    std::ifstream ifs(filename);
    if (!ifs.is_open()) {
        std::cerr << "TimeData file not found: " << filename << std::endl;
        return;
    }

    mPatterns.clear(); // 既存パターンをクリア
    std::string line;

    // ファイルを1行ずつ読み込む
    while (std::getline(ifs, line))
    {
        if (line.empty()) continue;

        size_t colonPos = line.find(':');
        if (colonPos == std::string::npos) continue;

        std::string patternKey = line.substr(0, colonPos);  // pattern0 など
        std::string params = line.substr(colonPos + 1);     // maxTime=5.0,... 形式

        int index = -1;
        if (sscanf_s(patternKey.c_str(), "pattern%d", &index) != 1)
            continue;

        TimeData data{};
        std::istringstream paramStream(params);
        std::string pair;

        // key=value ペアをパース
        while (std::getline(paramStream, pair, ',')) {
            // 空白除去
            pair.erase(std::remove_if(pair.begin(), pair.end(), ::isspace), pair.end());
            size_t eqPos = pair.find('=');
            if (eqPos == std::string::npos) continue;

            std::string key = pair.substr(0, eqPos);
            std::string value = pair.substr(eqPos + 1);

            if (key == "maxTime") data.maxTime = std::stof(value);
            else if (key == "normalStart") data.normalStart = std::stof(value);
            else if (key == "normalEnd") data.normalEnd = std::stof(value);
            else if (key == "goodStart") data.goodStart = std::stof(value);
            else if (key == "goodEnd") data.goodEnd = std::stof(value);
        }

        mPatterns.push_back(data);
    }

    ifs.close();

    // パターンが読み込まれなかった場合
    if (mPatterns.empty()) {
        std::cerr << "No time patterns loaded from " << filename << std::endl;
    }
    else {
        std::cout << "Loaded " << mPatterns.size() << " time patterns from " << filename << std::endl;

        // 初期パターンをランダムで選択
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, (int)mPatterns.size() - 1);
        mCurrent = mPatterns[dist(gen)];
    }

    // 初期状態
    mTimer = 0.f;           // タイマー初期化
    mMovingRight = true;    // 初期移動方向は右
    mReachedEnd = false;    // 端到達フラグをリセット
}

//-----------------------------------------------------------------------------
// ■ TimeMng::Update
//    経過時間を加算し、端に到達したかを判定する
//-----------------------------------------------------------------------------
void TimeMng::Update(float deltaTime)
{
    if (mPatterns.empty()) return;

    // 移動方向に応じてタイマーを進める
    mTimer += (mMovingRight ? deltaTime : -deltaTime);

    // 端に到達したかの判定
    if (mMovingRight && mTimer >= mCurrent.maxTime) {
        mTimer = mCurrent.maxTime;
        mReachedEnd = true; // 端に到達した
    }
    else if (!mMovingRight && mTimer <= 0.f) {
        mTimer = 0.f;
        mReachedEnd = true; // 端に到達した
    }
}

//-----------------------------------------------------------------------------
// ■ TimeMng::Reset
//    タイマーをリセットし、ランダムにパターンを変更
//    Zキー押下や時間切れ時に呼び出される
//-----------------------------------------------------------------------------
void TimeMng::Reset()
{
    if (mPatterns.empty()) return;

    // ランダムに新しいパターンを選択
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, (int)mPatterns.size() - 1);
    mCurrent = mPatterns[dist(gen)];

    // 移動方向を反転
    mMovingRight = !mMovingRight;

    // タイマー初期化
    mTimer = mMovingRight ? 0.f : mCurrent.maxTime;
    mReachedEnd = false; // 端到達フラグリセット
}

//-----------------------------------------------------------------------------
// ■ TimeMng::SetPattern
//    指定インデックスのパターンを強制設定
//-----------------------------------------------------------------------------
void TimeMng::SetPattern(int index)
{
    if (index >= 0 && index < static_cast<int>(mPatterns.size())) {
        mCurrent = mPatterns[index];
        mTimer = 0.f;
        mReachedEnd = false;
    }
}