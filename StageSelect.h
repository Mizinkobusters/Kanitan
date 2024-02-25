#pragma once

#include "Common.h"
#include "SimpleButton.h"

class StageSelect : public App::Scene
{
public:
	StageSelect(const InitData& init);
	void update() override;
	void draw() const override;

private:
	void DrawDiffStatus(Vec2 pos, int32 diff) const;

	Array<RectF> selectRects;
	Array<SimpleButton*> selectButtons;
	SimpleButton::ButtonStyle selectStyle;

	RectF quitRect{ 0, 520, 250, 70 };
	SimpleButton* quitButton;
	SimpleButton::ButtonStyle quitStyle;

	RectF collectionRect { 550, 520, 250, 70 };
	SimpleButton* collectionButton;
	SimpleButton::ButtonStyle collectionStyle;

	// ステージのスコア状況(難易度3つ分)
	StageStatus diffs[3]; // easy, normal, hard
};
