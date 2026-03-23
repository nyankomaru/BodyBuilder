#include "MiniGame.h"
#include "Fwk/Framework.h"

//============================================================
// ■ MiniGame 実装
//   ミニゲームの初期化・更新・描画・終了処理と
//   共通入力（Zキーによるスコア判定）を提供。
//============================================================

//------------------------------------------------------------
// ■ 初期化
//   MiniGame の開始時に呼び出す。
//   BG、UI、Player を初期化し、DataMng からステータスを取得。
//   制限時間を Endurance に応じて設定。
//------------------------------------------------------------
void MiniGame::Init() {
    // BG 初期化
    if (!mBG) {
        DebugLog("MiniGame::Init() mBG is nullptr!");
        assert(false && "mBG is nullptr");
        return;
    }
    mBG->Init();

    // UI 初期化
    if (!mUIMng) {
        DebugLog("MiniGame::Init() mUIMng is nullptr!");
        assert(false && "mUIMng is nullptr");
        return;
    }
    mUIMng->Init();

    // Player 初期化
    if (!mPlayer) {
        DebugLog("MiniGame::Init() mPlayer is nullptr!");
        assert(false && "mPlayer is nullptr");
        return;
    }
    mPlayer->Init();

    // DataMng と StatusData を安全にセット
    if (!mDataMng) {
        DebugLog("MiniGame::Init: mDataMng is nullptr!");
        assert(false && "mDataMng is nullptr");
        return;
    }
    StatusData* statusData = &mDataMng->GetStatusData();
    if (!statusData) {
        DebugLog("MiniGame::Init: StatusData is nullptr!");
        assert(false && "StatusData is nullptr");
        return;
    }
    mPlayer->SetStatusData(statusData);

    // Endurance に応じて制限時間を設定
    int endurance = mDataMng->GetStatusData().GetEndurance();
    mLimitTime = 10.0f + (endurance - 10) * 0.5f;

    // 次に開始するゲームを初期設定
    mNextGame = GameType::Barbell;

    // スコア初期化
    mLastScore = Score::None;

    // 内部タイマー初期化
    mTimer = 0.f;
}

//------------------------------------------------------------
// ■ 終了
//   MiniGame 終了時に呼び出す。
//   BG、UI、Player の終了処理を行う。
//------------------------------------------------------------
void MiniGame::Term() {
    mBG->Term();
    mUIMng->Term();
    mPlayer->Term();
}

//------------------------------------------------------------
// ■ 更新
//   毎フレーム呼び出される。
//   ・内部タイマー更新
//   ・TimeMng 更新
//   ・入力判定
//   ・UI、Player 更新
//   ・制限時間経過でゲーム終了判定
//------------------------------------------------------------
void MiniGame::Update() {
    if (!mTimeMng) return;

    if (mTimer <= mLimitTime) {
        // 経過時間更新
        mTimer += Time_I->GetDeltaTime();

        // 時間管理オブジェクト更新
        mTimeMng->Update(Time_I->GetDeltaTime());

        // 入力処理
        HandleInput();

        // 残り時間を計算
        float remaining = mLimitTime - mTimer;
        if (remaining < 0.0f) remaining = 0.0f;

        // UI 更新
        mUIMng->Update(*mTimeMng);
        mUIMng->UpdateRemaining(remaining);

        // プレイヤー更新
        mPlayer->Update();
    }
    else {
        // 制限時間経過でゲーム終了
        DebugLog("MiniGame終了（制限時間経過）");
        mNextGame = GameType::None;
    }
}

//------------------------------------------------------------
// ■ 描画
//   毎フレーム呼び出される。
//   BG → UI → Player の順で描画
//------------------------------------------------------------
void MiniGame::Render() {
    mBG->Render();
    mUIMng->Render();
    mPlayer->Render();
}

//------------------------------------------------------------
// ■ 共通入力処理
//   Zキー入力によるスコア判定を行う。
//   判定範囲によって Good / Normal / Bad を設定。
//   また、タイマーが終了して押されなかった場合も Bad とする。
//------------------------------------------------------------
void MiniGame::HandleInput()
{
    const TimeData& t = mTimeMng->GetCurrent();
    float timer = mTimeMng->GetTimer();

    bool scoreOccurred = false;

    // Zキー押下でスコア判定
    if (Input_I->IsKeyDown('Z')) {
        if (timer >= t.goodStart && timer <= t.goodEnd) {
            mLastScore = Score::Good;
        }
        else if (timer >= t.normalStart && timer <= t.normalEnd) {
            mLastScore = Score::Normal;
        }
        else {
            mLastScore = Score::Bad;
        }
        scoreOccurred = true;
    }

    // タイマーが端に到達して未入力の場合は Bad 判定
    if (mTimeMng->IsReachedEnd() && mLastScore == Score::None) {
        mLastScore = Score::Bad;
        scoreOccurred = true;
    }

    if (scoreOccurred) {
        // タイマーリセット
        mTimeMng->Reset();

        // スコアUIに反映
        mUIMng->mScore.SetScore(mLastScore);

        // プレイヤーのステータス反映
        if (mPlayer)
            mPlayer->IncreaseStatus(mLastScore);

        mLastScore = Score::None;
    }
}
