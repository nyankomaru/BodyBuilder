#pragma once

//シーン種別
enum class SceneType {
	None = -1,
	Title,
	Select,
	Menu,
	Training,
	Result,
	Contest,
	GameOver,
	GameClear
};

//スコア
enum class Score { 
	None=-1,
	Bad,
	Normal,
	Good
};

//ミニゲーム
enum class GameType {
	None=-1,
	Barbell,
	Dumbbel,
	LatPulldown,
	RunningMachine,
	Squat
};

//トレーニングシーン
enum class TrainingStep {
	None=-1,
	Select,   // ゲーム選択中
	Play      // ミニゲーム実行中
};

//プレイヤーの状態
enum class Status {
	None=-1,
	Barbell,
	Dumbbel,
	LatPulldown,
	RunningMachine,
	Squat
};