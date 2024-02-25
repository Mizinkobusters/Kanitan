#pragma once

#include "Enemy.h"

class EnemyFish : public Enemy
{
	// 敵の移動速度
	static inline const double ENEMY_SPEED = 12.5;

public:
	EnemyFish(Vec2 pos);

	void update(double deltaTime) override;
	void draw() const override;
	void MoveEnemy(double deltaTime) override;
};
