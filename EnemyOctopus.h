#pragma once

#include "Enemy.h"
#include "Gimmick.h"
#include "GimmickInk.h"

class EnemyOctopus : public Enemy
{
	// インク発射間隔
	static inline const double INK_INTERVAL = 5.0;

	// インク射出可能最大棘数
	static inline const int32 MAX_INK = 1;

public:
	EnemyOctopus(Vec2 pos);

	void update(double deltaTime) override;
	void draw() const override;
	void MoveEnemy(double deltaTime) override;

	void SpawnInk(double deltaTime);
	void RemoveInk();

private:
	double timer;
	Array<GimmickInk*> gimmicks;
};
