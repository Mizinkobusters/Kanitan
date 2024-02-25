#pragma once

#include "Enemy.h"
#include "Gimmick.h"

class EnemyBlow : public Enemy
{
	// 棘発射間隔
	static inline const double SPIKE_INTERVAL = 3.0;

	// 射出可能最大棘数
	static inline const int32 MAX_SPIKE = 3;

public:
	EnemyBlow(Vec2 pos, Vec2 playerPos);

	void update(double deltaTime) override;
	void draw() const override;
	void MoveEnemy(double deltaTime) override;

	void SpawnSpike(double deltaTime);
	void RemoveSpike();

private:
	double timer;
	Vec2 playerPos;
	Array<Gimmick*> gimmicks;
};
