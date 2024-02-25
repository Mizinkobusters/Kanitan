#pragma once

#include "Gimmick.h"

class GimmickInk : public Gimmick
{
	// インクの乗算値
	static inline const double GIMMICK_SPEED = 0.25;

public:
	GimmickInk(Vec2 pos);

	void update(double deltaTime) override;
	void draw() const override;
	void MoveGimmick(double deltaTime) override;

	double GetAlpha();

private:
	Circle ink;
	ColorF color{ 0.0, 1.0 };
	double alpha;
};
