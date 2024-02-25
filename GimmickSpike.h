#pragma once

#include "Gimmick.h"

class GimmickSpike : public Gimmick
{
	// 棘の移動速度
	static inline const double GIMMICK_SPEED = 50.0;

public:
	GimmickSpike(Vec2 pos, Vec2 playerPos);

	void update(double deltaTime) override;
	void draw() const override;
	void MoveGimmick(double deltaTime) override;

private:
	Vec2 playerPos;
	Vec2 origin;
};
