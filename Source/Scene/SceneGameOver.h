#pragma once

//シーンクラスの継承するのでinclude
#include "Scene.h"
#include "Enemy/Enemy.h"

//ゲームオーバーシーンクラス
class SceneGameOver :public Scene
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

private:
	Player* mPlayer = nullptr;                  // プレイヤーオブジェクトへのポインタ
	std::vector<EnemyStatus> mEnemies;          // 9人の敵ステータス
	Contest mContest;                           // 大会結果管理クラス
};