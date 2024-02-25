#pragma once

#include "Common.h"
#include "SimpleButton.h"

class Option : public App::Scene
{
public:
	Option(const InitData& init);
	void update() override;
	void draw() const override;

	void Decrase(SimpleButton*& button, RectF& rect, int32& value);
	void Increase(SimpleButton*& button, RectF& rect, int32& value);

private:
	RectF quitRect{ 0, 520, 250, 70 };
	SimpleButton* quitButton;


	RectF bgmmRect { 300, 215, 40, 70 };
	SimpleButton* bgmmButton;

	RectF bgmpRect{ 460, 215, 40, 70 };
	SimpleButton* bgmpButton;

	RectF semRect{ 300, 365, 40, 70 };
	SimpleButton* semButton;

	RectF sepRect{ 460, 365, 40, 70 };
	SimpleButton* sepButton;

	SimpleButton::ButtonStyle quitStyle;
	SimpleButton::ButtonStyle buttonStyle;

	int32 valueBGM;
	int32 valueSE;
};
