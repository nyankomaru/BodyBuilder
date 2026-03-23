#include "App.h"
#include "Scene/SceneMng.h"

SceneMng sceneMng;

//ゲームの初期化処理を行う関数
void App::Init() {
	GameObjectInit();
	//シーン管理クラスを初期化し、最初のシーンはタイトルシーン
	sceneMng.Init(SceneType::Title);
}

//ゲームの更新処理を行う関数
void App::Update() {
	//シーン更新
	sceneMng.Update();
}

//ゲームの描画処理を行う関数
void App::Render() {
	//シーン描画
	sceneMng.Render();
}

// ゲームの後片付け処理を行う関数
void App::Term() {
	//シーン解放
	sceneMng.Term();
	GameObjectTerm();
}
