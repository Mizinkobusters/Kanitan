#pragma once

#include "Common.h"
#include "SimpleButton.h"

class Result : public App::Scene
{
public:
	Result(const InitData& init);
	void update() override;
	void draw() const override;

private:
	Rank rank;

	RectF retryRect{ 550, 520, 250, 70 };
	SimpleButton* retryButton;
	RectF quitRect{ 0, 520, 250, 70 };
	SimpleButton* quitButton;

	SimpleButton::ButtonStyle buttonStyle;
};
