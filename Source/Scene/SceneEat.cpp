#include "SceneEat.h"
#include "Fwk/Framework.h"

//初期化
void SceneEat::Init() {
	//次のシーンの初期値をなしにしておく
	mNextScene = SceneType::None;
}

//終了
void SceneEat::Term() {
	//無し
}

//更新
void SceneEat::Update() {
	//Zキーを押したら次のシーンにInGameを設定する
	if (Input_I->IsKeyDown('Z')) {
		mNextScene = SceneType::Select;
	}
}

//描画
void SceneEat::Render() {
	//文字列を描画する
	PrintText("食事", 380.0f, 250.0f);
}