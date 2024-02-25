#pragma once

#include "Common.h"
#include "SimpleButton.h"

class Title : public App::Scene
{
public:
	Title(const InitData& init);
	void update() override;
	void draw() const override;

private:
	Array<RectF> menuRects;
	Array<SimpleButton*> menuButtons;
	SimpleButton::ButtonStyle menuStyle;
	Array<String> texts =
	{
		U"はじめる",
		U"あそびかた",
		U"オプション",
		U"おわる"
	};

	RenderTexture blur{ TextureAsset(U"BG0").size() };
	RenderTexture internalTex{ TextureAsset(U"BG0").size() };
};
