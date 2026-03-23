#pragma once
#include "MiniGame.h"

//バーベルクラス
class Barbell :public MiniGame
{
public:
	//初期化
	void Init();
	//終了
	void Term();
	//更新
	void Update();
	//描画
	void Render();
};