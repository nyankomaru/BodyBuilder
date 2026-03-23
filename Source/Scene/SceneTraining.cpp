#include "SceneTraining.h"
#include "Fwk/Framework.h"
#include "MiniGame/MiniGameMng.h"
#include "MiniGame/MiniGame.h"

//============================================================
// ■ 初期化
//   - シーン開始時に呼び出される
//   - 背景、UI、プレイヤー、ミニゲームマネージャなどの初期化
//============================================================
void SceneTraining::Init() {
    // 次のシーンの初期値をなしに設定
    mNextScene = SceneType::None;

    // 時間管理初期化
    mTimeMng.Init("Assets\\Data\\TimeData.kv");  // デフォルトタイムパターン読み込み
    mTimeMng.SetPattern(0);

    // ミニゲームマネージャに関連オブジェクトをセット
    mMiniGameMng.SetBG(&mBG);
    mMiniGameMng.SetTimeMng(&mTimeMng);
    mMiniGameMng.SetUIMng(&mUIMng);
    mMiniGameMng.SetPlayer(&mPlayer);
    mMiniGameMng.SetDataMng(GetDataMng());
    mMiniGameMng.Init(GameType::None);  // 最初はゲームなし

    // 背景初期化
    GetBG()->Init();
    GetBG2()->Init();

    // 初期ステータス（スクワット）で開始
    mPlayer.SetStatus(Status::Squat);
}

//============================================================
// ■ 終了
//   - シーン終了時に呼び出される
//   - 各オブジェクトのリソースを解放
//============================================================
void SceneTraining::Term() {
    mPlayer.Term();
    GetBG()->Term();
    GetBG2()->Term();
    mMiniGameMng.Term();
}

//============================================================
// ■ 更新
//   - 毎フレーム呼び出される
//   - 日数やステップに応じて更新処理を切り替える
//============================================================
void SceneTraining::Update() {

    // 現在が8日目以上なら強制的に大会シーンへ遷移
    if (GetDataMng()->GetDayData().GetCurrentDay() > 7) {
        mNextScene = SceneType::Contest;
        return;
    }

    // Stepに応じて更新処理を呼び分け
    switch (mStep)
    {
    case TrainingStep::Select:
        Update_Select(); // ゲーム選択画面の更新
        break;
    case TrainingStep::Play:
        Update_Play();   // ミニゲームプレイ中の更新
        break;
    }
}

//============================================================
// ■ 描画
//   - 毎フレーム呼び出される
//   - Stepに応じて描画処理を切り替える
//============================================================
void SceneTraining::Render() {

    switch (mStep)
    {
    case TrainingStep::Select:
        Render_Select();      // 選択画面の描画
        break;
    case TrainingStep::Play:
        mMiniGameMng.Render(); // ミニゲーム描画
        break;
    }
}

//============================================================
// ■ ミニゲーム選択画面の更新
//   - 上下キーで選択
//   - Zキーで決定してミニゲーム開始
//============================================================
void SceneTraining::Update_Select() {
    // 上キー・下キーで選択
    if (Input_I->IsKeyDown(VK_DOWN)) {
        ++mSelectedIndex;
        if (mSelectedIndex >= kGameCount) mSelectedIndex = 0;
    }
    if (Input_I->IsKeyDown(VK_UP)) {
        --mSelectedIndex;
        if (mSelectedIndex < 0) mSelectedIndex = kGameCount - 1;
    }

    // 現在選択中のゲームに応じてプレイヤーのアニメーション変更
    switch (mGameList[mSelectedIndex])
    {
    case GameType::Barbell:       mPlayer.SetStatus(Status::Barbell); break;
    case GameType::Dumbbel:       mPlayer.SetStatus(Status::Dumbbel); break;
    case GameType::LatPulldown:   mPlayer.SetStatus(Status::LatPulldown); break;
    case GameType::RunningMachine:mPlayer.SetStatus(Status::RunningMachine); break;
    case GameType::Squat:         mPlayer.SetStatus(Status::Squat); break;
    }

    // Zキーで決定 → ミニゲーム開始
    if (Input_I->IsKeyDown('Z')) {
        GameType selectedGame = mGameList[mSelectedIndex];
        mMiniGameMng._beginGame(selectedGame);
        mStep = TrainingStep::Play;  // ミニゲーム開始に切り替え
    }
}

//============================================================
// ■ ミニゲームプレイ中の更新
//   - ミニゲーム更新
//   - 終了時に日数加算とセーブ
//============================================================
void SceneTraining::Update_Play() {
    mMiniGameMng.Update();

    // ミニゲーム終了判定
    if (mMiniGameMng.IsFinished()) {

        // 次の日に進める
        int NextDay = GetDataMng()->GetDayData().GetCurrentDay() + 1;
        GetDataMng()->GetDayData().SetCurrentDay(NextDay);

        // 結果表示画面にする
        mNextScene = SceneType::Result;
    }
}

//============================================================
// ■ ミニゲーム選択画面の描画
//   - 背景描画
//   - ゲーム選択メニュー描画
//============================================================
void SceneTraining::Render_Select() {
    // 背景描画
    GetBG()->Render();
    GetBG2()->Render();

    // 文字色設定
    float color_selected[3] = { 0.0f, 1.0f, 1.0f }; // 選択中の文字は色
    float color_normal[3] = { 0.85f, 0.0f, 0.85f }; // 通常文字は紫

    // ゲーム選択肢描画
    for (int i = 0; i < kGameCount; ++i) {
        const std::string& label = menuLabels[i];
        float* color = (mSelectedIndex == i) ? color_selected : color_normal;
        SetTextColor(color);
        PrintText(label.c_str(), 380.0f, 250.0f + i * 40.0f);
    }
}
