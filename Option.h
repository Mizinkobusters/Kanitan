#pragma once

#include "Common.h"

class Option : public App::Scene
{
public:
	Option(const InitData& init);
	void update() override;
	void draw() const override;

private:
	Array<RectF> buttons;
	Array<String> texts =
	{
		U"はじめる",
		U"あそびかた",
		U"オプション",
		U"おわる"
	};

private:
	Rect buttonQuit{ 550, 520, 250, 70 };
};
