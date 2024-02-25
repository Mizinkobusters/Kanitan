#pragma once

#include "Enemy.h"

class EnemyTropical : public Enemy
{
	// 敵の移動速度
	static inline const double ENEMY_SPEED = 10.0;

public:
	EnemyTropical(Vec2 pos, Vec2 playerPos);

	void update(double deltaTime) override;
	void draw() const override;
	void MoveEnemy(double deltaTime) override;

private:
	Vec2 playerPos;
};
