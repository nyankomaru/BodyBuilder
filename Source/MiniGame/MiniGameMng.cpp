// MiniGameMng.cpp
#include "MiniGameMng.h"
#include "Fwk/Framework.h"
#include "Barbell.h"
#include "Dumbbell.h"
#include "LatPulldown.h"
#include "RunningMachine.h"
#include "Squat.h"

//============================================================
// ■ MiniGameMng 実装
//   ミニゲームの生成、更新、描画、終了処理を実装。
//   現在のゲーム、次に開始するゲーム、外部オブジェクトとの接続を管理。
//============================================================

//------------------------------------------------------------
// ■ 初期化
//   MiniGameMng の初期状態を設定。
//   startGame: 最初に開始するゲームタイプ
//------------------------------------------------------------
void MiniGameMng::Init(GameType startGame)
{
    // 現在のゲームは無し
    mCurrentGame = GameType::Barbell; // 初期値として適当なゲームを設定（開始前の仮設定）

    // 次に開始するゲームを記録
    mNextGame = startGame;

    // 現在のゲームポインタは nullptr にしておく
    mpGame = nullptr;
}

//------------------------------------------------------------
// ■ 終了
//   MiniGameMng を終了し、現在のゲームを解放。
//------------------------------------------------------------
void MiniGameMng::Term()
{
    // 現在動作中のゲームを終了
    _endCurrentGame();

    // 次のゲームは無しとしておく
    mNextGame = GameType::None;
}

//------------------------------------------------------------
// ■ 更新
//   毎フレーム呼び出され、現在のミニゲームを更新。
//------------------------------------------------------------
void MiniGameMng::Update()
{
    // 現在ゲームが None の場合は何もしない
    if (mCurrentGame == GameType::None) return;

    // 現在のミニゲームを更新
    mpGame->Update();

    // ミニゲーム側で終了した場合、現在ゲームを None に設定
    if (mpGame->GetNextGame() == GameType::None) {
        mCurrentGame = GameType::None;
        DebugLog("MiniGameMng: ミニゲーム終了検知");
    }
}

//------------------------------------------------------------
// ■ 描画
//   毎フレーム呼び出され、現在のゲームを描画。
//------------------------------------------------------------
void MiniGameMng::Render()
{
    // ゲームオブジェクトが存在する場合のみ描画
    if (mpGame != nullptr) {
        mpGame->Render();
    }
}

//------------------------------------------------------------
// ■ 新しいゲームを開始する
//   現在のゲームを終了し、新しいゲームを生成・初期化。
//------------------------------------------------------------
void MiniGameMng::_beginGame(GameType newGame)
{
    // 現在動作中のゲームを停止
    _endCurrentGame();

    // 新しいゲームを生成
    switch (newGame)
    {
    case GameType::Barbell:
        mpGame = new Barbell();       // バーベルゲーム
        break;
    case GameType::Dumbbel:
        mpGame = new Dumbbell();      // ダンベルゲーム
        break;
    case GameType::LatPulldown:
        mpGame = new LatPulldown();   // ラットプルダウン
        break;
    case GameType::RunningMachine:
        mpGame = new RunningMachine();// ランニングマシン
        break;
    case GameType::Squat:
        mpGame = new Squat();         // スクワット
        break;
    }

    // 生成したゲームに必要な外部オブジェクトを設定
    if (mpGame != nullptr) {
        mpGame->SetTimeMng(mTimeMng);
        mpGame->SetBG(mBG);
        mpGame->SetUIMng(mUIMng);
        mpGame->SetPlayer(mPlayer);
        mpGame->SetDataMng(mDataMng);

        // ゲームの初期化を実行
        mpGame->Init();
    }

    // 現在ゲームを新しいゲームに更新
    mCurrentGame = newGame;
}

//------------------------------------------------------------
// ■ 現在のゲームを停止する
//   現在のゲームを終了し、ポインタを解放。
//------------------------------------------------------------
void MiniGameMng::_endCurrentGame()
{
    if (mpGame != nullptr) {
        mpGame->Term();         // ゲーム終了処理
        SAFE_DELETE(mpGame);    // メモリ解放
    }

    // ゲームを止めたので現在ゲームも None に設定
    mCurrentGame = GameType::None;
}

//------------------------------------------------------------
// ■ ゲーム終了判定
//   現在のゲームが None なら終了扱いとして true を返す
//------------------------------------------------------------
bool MiniGameMng::IsFinished() const
{
    return (mCurrentGame == GameType::None);
}
