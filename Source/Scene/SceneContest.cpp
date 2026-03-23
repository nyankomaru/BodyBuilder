#include "SceneContest.h"
#include "Fwk/Framework.h"
#include <iostream>

//============================================================
// ■ SceneContest 実装
//   大会シーン（Contest）の更新・描画処理
//============================================================

//------------------------------------------------------------
// ■ 初期化
//   シーン開始時に呼び出される
//   - 次のシーンを None に初期化し、遷移状態をリセット
//------------------------------------------------------------
void SceneContest::Init() {
    mNextScene = SceneType::None; // 次のシーンは未設定


    // DataMng の StatusData 参照を取得し、そのアドレスをプレイヤーにセット
    StatusData& status = GetDataMng()->GetStatusData();
    GetPlayer()->SetStatusData(&status);

    // ファイルから敵ステータス読み込み
    if (!GetEnemy()->LoadEnemyStatuses("Assets\\Data\\EnemyData.kv", mEnemies)) {
        // 読み込み失敗時の処理（エラーログなど）
        std::cerr << "Failed to load enemy statuses." << std::endl;
    }

    GetBG3()->Init();

    mTexture.Load("Images/mukimuki.png");
    mSprite.Init();
    mSprite.SetTexture(mTexture);
    mSprite.SetSize(584.0f, 413.0f);   // 表示サイズ
    mSprite.SetPosition(0, 0);       // 初期位置
    mSprite.SetVisible(true);

}

//------------------------------------------------------------
// ■ 終了
//   シーン終了時に呼び出される
//   - 大会シーン固有のリソース解放や整理処理があればここに記述
//   - デフォルトでは何も処理しない
//------------------------------------------------------------
void SceneContest::Term() {
    GetBG3()->Term();
    mSprite.Term();
    mTexture.Unload();
}

//------------------------------------------------------------
// ■ 更新
//   毎フレーム呼び出される
//   - ユーザー入力によって次のシーンへの遷移を設定する
//------------------------------------------------------------
void SceneContest::Update() {
    // プレイヤーのステータス合計スコアを計算
    int playerScore = 0;
    StatusData* sd = GetPlayer()->GetStatusData();
    if (sd) {
        playerScore = sd->GetChest() + sd->GetBack() + sd->GetArm() + sd->GetLeg();
    }

    // 敵9人の合計ステータススコア計算（mEnemies は std::vector<EnemyStatus>）
    std::vector<int> scores;
    for (const EnemyStatus& es : mEnemies) {
        int score = es.mChest + es.mBack + es.mArm + es.mLeg;
        scores.push_back(score);
    }

    // プレイヤースコアを最後に追加
    scores.push_back(playerScore);

    // 降順にソート（大きいスコアが上位）
    std::vector<int> sortedScores = scores;
    std::sort(sortedScores.begin(), sortedScores.end(), std::greater<int>());

    // プレイヤーの順位を計算（1位がrank=1）
    int rank = 1;
    for (int s : sortedScores) {
        if (s == playerScore) break;
        rank++;
    }

    // Contest クラスに順位とスコアを設定
    mContest.SetRank(rank);
    mContest.SetScore(playerScore);

    if (Input_I->IsKeyDown('Z')) {
        // ランクによる結果判定
        if (rank == 1) {
            // クリア処理例
            mNextScene = SceneType::GameClear;  // クリアシーンへ遷移
        }
        else {
            // ゲームオーバー処理例
            mNextScene = SceneType::GameOver;  // ゲームオーバーシーンへ遷移
        }
    }
}

//------------------------------------------------------------
// ■ 描画
//   毎フレーム呼び出される
//   - 大会シーンの画面表示を担当
//------------------------------------------------------------
void SceneContest::Render() {
    float startX = 100.0f;    // 描画開始X座標
    float startY = 50.0f;     // 描画開始Y座標
    float lineHeight = 30.0f; // 行間

    GetBG3()->Render();
    mSprite.Draw();

    // -----------------------------
    // 画面中央上部に「採点中」を金色で描画
    // -----------------------------
    float goldColor[3] = { 1.0f, 0.843f, 0.0f }; // 金色
    SetTextColor(goldColor);

    const char* title = "採点中";

    // 画面右下基準
    float screenWidth = 800.0f; // 仮の画面幅
    float screenHeight = 600.0f; // 仮の画面高さ
    float textSize = 48.0f;      // 大きめの文字サイズ
    SetTextSize(textSize);

    float textWidthEstimate = 100.0f; // 仮の文字幅

    PrintText(title, 540,50);

}