#pragma once
#include "Enemy.h"

// シーンの名前
enum State
{
	State_Title,
	State_StageSelect,
	State_Countdown,
	State_Game,
	State_Finish,
	State_Result,
	//State_Credit,
	State_Option,
};

// ゲーム終了の原因
enum Cause
{
	Cause_Timeover,
	Cause_Death,
};

// ステージ難易度
enum StageDifficulty
{
	Diff_Easy,
	Diff_Normal,
	Diff_Hard,
	Diff_Max,
};

// ランクボーダー
enum Rank
{
	Rank_S = 25,
	Rank_A = 20,
	Rank_B = 15,
	Rank_C = 10,
	Rank_D = 0,
};

// ステージのスコア状況
struct StageStatus
{
	bool isPlayable;
	bool isNoDamage;
	bool doesGetSRank;
	int32 score;
	int32 maxCombo;
	int32 killCount;
};

// シーン間で共有するデータ
struct GameData
{
	StageDifficulty diff;	// ステージの難易度
	Cause cause;			// ゲーム終了の原因
	int32 lastScore;		// ゲーム終了時のスコア
	int32 lastMaxCombo;		// ゲーム終了時の最大コンボ
	int32 lastKillCount;	// ゲーム終了時のキルカウント
	Vec2 lastPlayerPos;		// ゲーム終了時のプレイヤーの位置
	Array<Enemy*> enemys;	// ゲーム終了時の敵の位置

	int valueBGM;			// BGMの大きさ
	int valueSE;			// SEの大きさ
};

static const Array<String> msgDiff = { U"かんたん", U"ふつう", U"むずかしい" };

// シーンマネージャークラスを作成
using App = SceneManager<State, GameData>;

// スクリーンの大きさ
static const Vec2 SCREEN_SIZE = { 800,600 };

// セーブデータ
static const String SCORE_DATA = U"Assets/CSV/scoredata.csv";

// オプションデータ
static const String OPTION_DATA = U"Assets/CSV/option.csv";

// キルデータ
static const String KILL_DATA = U"Assets/CSV/killdata.csv";
