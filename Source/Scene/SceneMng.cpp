#include "Fwk/Framework.h"
#include "GameObjectMng/GameObjectMng.h"
#include "SceneMng.h"
#include "SceneTitle.h"
#include "SceneMenu.h"
#include "SceneSelect.h"
#include "SceneTraining.h"
#include "SceneResult.h"
#include "SceneContest.h"
#include "SceneGameClear.h"
#include "SceneGameOver.h"

//============================================================
// ■ SceneMng 実装
//   ゲームのシーン遷移を管理
//   各シーンの生成・初期化・更新・描画・終了を統括
//============================================================

//------------------------------------------------------------
// ■ 初期化
//   ゲーム開始時に呼び出す。
//   startScene で指定したシーンからスタート。
//   現在シーンは None として初期化
//------------------------------------------------------------
void SceneMng::Init(SceneType startScene)
{
    // 現在シーンは無し
    mCurrentScene = SceneType::None;

    // 次に開始するシーンを設定
    mNextScene = startScene;

    // シーンクラスのポインタは nullptr に初期化
    mpScene = nullptr;
}

//------------------------------------------------------------
// ■ 終了
//   現在のシーンを終了し、リソース解放
//------------------------------------------------------------
void SceneMng::Term()
{
    // 現在のシーンを終了
    _endCurrentScene();

    // 次のシーンは無し
    mNextScene = SceneType::None;
}

//------------------------------------------------------------
// ■ 更新
//   毎フレーム呼び出される。
//   - シーン切り替え判定
//   - 現在のシーン更新
//------------------------------------------------------------
void SceneMng::Update()
{
    // 次のシーンが設定されており、
    // 現在のシーンと異なる場合は切り替え
    if (mNextScene != SceneType::None && mCurrentScene != mNextScene) {
        _beginScene(mNextScene);  // シーン開始処理
    }

    // 現在のシーンを更新
    if (mpScene != nullptr) {
        mpScene->Update();

        // 現在シーンの次に遷移するシーンを取得
        mNextScene = mpScene->GetNextScene();
    }
}

//------------------------------------------------------------
// ■ 描画
//   現在のシーンを描画
//------------------------------------------------------------
void SceneMng::Render()
{
    if (mpScene != nullptr) {
        mpScene->Render();
    }
}

//------------------------------------------------------------
// ■ 新しいシーンを開始する
//   - 現在のシーンを終了
//   - 指定シーンを生成・初期化
//   - mCurrentScene に設定
//------------------------------------------------------------
void SceneMng::_beginScene(SceneType newScene)
{
    // 現在のシーンを終了
    _endCurrentScene();

    // 新しいシーンを生成
    switch (newScene)
    {
    case SceneType::Title:     mpScene = new SceneTitle(); break;
    case SceneType::Select:    mpScene = new SceneSelect(); break;
    case SceneType::Menu:      mpScene = new SceneMenu(); break;
    case SceneType::Training:  mpScene = new SceneTraining(); break;
    case SceneType::Result:    mpScene = new SceneResult(); break;
    case SceneType::Contest:   mpScene = new SceneContest(); break;
    case SceneType::GameClear: mpScene = new SceneGameClear(); break;
    case SceneType::GameOver:  mpScene = new SceneGameOver(); break;
    default: break;
    }

    // シーンの初期化
    if (mpScene != nullptr) {
        mpScene->Init();
    }

    // 現在シーンを更新
    mCurrentScene = newScene;
}

//------------------------------------------------------------
// ■ 現在のシーンを停止する
//   - シーンの Term を呼び出してリソース解放
//   - ポインタを削除
//   - 現在シーンを None に設定
//------------------------------------------------------------
void SceneMng::_endCurrentScene()
{
    if (mpScene != nullptr) {
        mpScene->Term();
        SAFE_DELETE(mpScene);
    }

    mCurrentScene = SceneType::None;
}
