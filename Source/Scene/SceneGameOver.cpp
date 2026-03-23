#include "SceneGameOver.h"
#include "Fwk/Framework.h"
#include <iostream>

//初期化
void SceneGameOver::Init() {
	//次のシーンの初期値をなしにしておく
	mNextScene = SceneType::None;

	GetBG3()->Init();
	GetBG2()->Init();

	// DataMng の StatusData 参照を取得し、そのアドレスをプレイヤーにセット
	StatusData& status = GetDataMng()->GetStatusData();
	GetPlayer()->SetStatusData(&status);

	// ファイルから敵ステータス読み込み
	if (!GetEnemy()->LoadEnemyStatuses("Assets\\Data\\EnemyData.kv", mEnemies)) {
		// 読み込み失敗時の処理（エラーログなど）
		std::cerr << "Failed to load enemy statuses." << std::endl;
	}
}

//終了
void SceneGameOver::Term() {
	GetBG3()->Term();
	GetBG2()->Term();
}

//更新
void SceneGameOver::Update() {
	// プレイヤーのステータス合計スコアを計算
	int playerScore = 0;
	StatusData* sd = GetPlayer()->GetStatusData();
	if (sd) {
		playerScore = sd->GetChest() + sd->GetBack() + sd->GetArm() + sd->GetLeg();
	}

	// 敵9人の合計ステータススコア計算（mEnemies は std::vector<EnemyStatus>）
	std::vector<int> scores;
	for (const EnemyStatus& es : mEnemies) {
		int score = es.mChest + es.mBack + es.mArm + es.mLeg;
		scores.push_back(score);
	}

	// プレイヤースコアを最後に追加
	scores.push_back(playerScore);

	// 降順にソート（大きいスコアが上位）
	std::vector<int> sortedScores = scores;
	std::sort(sortedScores.begin(), sortedScores.end(), std::greater<int>());

	// プレイヤーの順位を計算（1位がrank=1）
	int rank = 1;
	for (int s : sortedScores) {
		if (s == playerScore) break;
		rank++;
	}

	// Contest クラスに順位とスコアを設定
	mContest.SetRank(rank);
	mContest.SetScore(playerScore);

	//Zキーを押したら次のシーンにTitleを設定する
	if (Input_I->IsKeyDown('Z')) {
		GetDataMng()->Delete("Assets\\Data\\SaveData.kv");
		mNextScene = SceneType::Title;
	}
}

//描画
void SceneGameOver::Render() {
	GetBG3()->Render();
	GetBG2()->Render();

	// 元に戻す
	float normalColor[3] = { 0.0f, 1.0f, 1.0f }; // 通常文字色（白）
	SetTextColor(normalColor);
	SetTextSize(24.0f); // 元の文字サイズに戻す

	//文字列を描画する
	PrintText("敗退だ", 300.0f, 200.0f);

	// プレイヤーのステータスとスコアを描画
	StatusData* sd = GetPlayer()->GetStatusData();
	int playerTotalScore = 0;
	if (sd) {
		playerTotalScore = sd->GetChest() + sd->GetBack() + sd->GetArm() + sd->GetLeg();

		char buf[128];
		snprintf(buf, sizeof(buf), "Total=%d", playerTotalScore);
		PrintText(buf, 300.0f, 250.0f);
	}

	// 順位を表示
	char rankBuf[64];
	snprintf(rankBuf, sizeof(rankBuf), "Your Rank: %d", mContest.GetRank());
	PrintText(rankBuf, 300.0f, 280.0f);
}