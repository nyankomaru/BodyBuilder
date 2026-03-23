#include "SceneSalon.h"
#include "Fwk/Framework.h"

//初期化
void SceneSalon::Init() {
	//次のシーンの初期値をなしにしておく
	mNextScene = SceneType::None;
}

//終了
void SceneSalon::Term() {
	//無し
}

//更新
void SceneSalon::Update() {
	//現在が31日目なら強制的に大会にする
	if (GetDataMng()->GetDayData().GetCurrentDay() > 30) {
		mNextScene = SceneType::Contest;
		return;
	}

	//Zキーを押したら次のシーンにInGameを設定する
	if (Input_I->IsKeyDown('Z')) {
		mNextScene = SceneType::Select;
	}
}

//描画
void SceneSalon::Render() {
	//文字列を描画する
	PrintText("日サロ！！", 380.0f, 250.0f);
}