#pragma once

// シーンの名前
enum State
{
	State_Title,
	State_Countdown,
	State_Game,
	State_Finish,
	State_Result,
	State_Credit,
	State_Option,
};

// ゲーム終了の原因
enum Cause
{
	Cause_Timeover,
	Cause_Death,
};

// 敵データ
struct Enemy
{
	int32 type;
	Vec2 pos;
	Circle collision;
};

// シーン間で共有するデータ
struct GameData
{
	Cause cause;			// ゲーム終了の原因
	int32 lastScore;		// ゲーム終了時のスコア
	int32 lastMaxCombo;		// ゲーム終了時の最大コンボ
	int32 lastKillCount;	// ゲーム終了時のキルカウント
	Vec2 lastPlayerPos;		// ゲーム終了時のプレイヤーの位置
	Array<Enemy> enemys;	// ゲーム終了時の敵の位置
};

// シーンマネージャークラスを作成
using App = SceneManager<State, GameData>;

// スクリーンの大きさ
static const Vec2 SCREEN_SIZE = { 800,600 };
