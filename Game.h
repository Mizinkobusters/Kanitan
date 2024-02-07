#pragma once

#include "Common.h"

class Game : public App::Scene
{
	// プレイヤーの移動速度
	static inline const double PLAYER_SPEED = 1.25;

	// プレイヤー行動時の消費コスト
	static inline const double PLAYER_COST = 7.5;

	// 設置可能マーカー数
	static inline const int32 MAX_MARKER = 4;

	// マーカー設置インターバル(秒)
	static inline const double MARKER_INTERVAL = 0.5;

	// 敵出現可能数
	static inline const int32 MAX_ENEMY = 10;

	// 敵出現インターバル(秒)
	static inline const double ENEMY_INTERVAL = 1.5;

	// 無敵継続時間
	static inline const double INVINCIBLE_DURATION = 1.25;

	// 体力回復値
	static inline const double POWER_REGENERATION = 3.0;

public:
	Game(const InitData& init);
	void update() override;
	void draw() const override;

private:
	// プレイヤーの位置
	Vec2 playerPos = { Scene::CenterF() };

	// プレイヤーの体力(コスト)
	double playerPower = 100.0;

	// プレイヤーの気絶フラグ
	bool playerIsStan = false;

	// プレイヤーの無敵フラグ
	bool playerIsInvincible = false;

	// プレイヤーの残機
	int32 playerLife = 3;

	// プレイヤーの当たり判定
	Circle playerCollision;

	// マーカーの位置を格納する配列
	Array<Vec2> markers;

	// トラップ描画用
	Quad trapRange;

	// ゲーム用タイマー
	double gameTimer = 30.0;

	// マーカー用タイマー
	double markerTimer = 0.0;

	// 敵出現用タイマー
	double enemyTimer = 0.0;

	// 無敵時間用タイマー
	double invincibleTimer = 1.25;

	// スコア
	int32 gameScore = 0;

	// コンボ
	int32 gameCombo = 0;

	// 最大コンボ
	int32 gameMaxCombo = 0;

	// キルカウント
	int32 gameKillCount = 0;

	// 敵データ格納用配列
	Array<Enemy> enemys;

	void CountdownTimer(const double deltaTime);
	void SpawnEnemy(const double deltaTime);
	void MovePlayer(const double deltaTime);
	void ConsumePower(const double deltaTime);
	void RegeneratePower(const double deltaTime);
	void PlaceMarker(const double deltaTime);
	void ClearMarker();
	void CheckCollision();
	void OnInvincible(const double deltaTime);
	void SaveData();
};
