#pragma once

#include "Common.h"

class Result : public App::Scene
{
public:
	Result(const InitData& init);
	void update() override;
	void draw() const override;

private:
	enum Rank
	{
		Rank_S,
		Rank_A,
		Rank_B,
		Rank_C,
		Rank_D,
	};
	Rank rank;

	Rect buttonRetry{ 0, 520, 250, 70 };
	Rect buttonQuit{ 550, 520, 250, 70 };
};
