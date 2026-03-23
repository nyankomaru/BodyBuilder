#pragma once
#include "MiniGame.h"

//ランニングマシンクラス
class RunningMachine :public MiniGame
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