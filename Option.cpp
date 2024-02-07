#include "Option.h"

Option::Option(const InitData& init)
	: IScene{ init }
{
	AudioAsset(U"Title").stop();
	AudioAsset(U"Game").stop();
	AudioAsset(U"Result").stop();
	AudioAsset(U"Sazanami").stop();
	// BGMを流す
	AudioAsset(U"Result").play();

}

void Option::update()
{
	if (buttonQuit.leftClicked())
	{
		changeScene(State_Title);
	}
}

void Option::draw() const
{
	// 背景を描画
	{
		TextureAsset(U"BG0").scaled(1.5).drawAt(SCREEN_SIZE.x / 2, SCREEN_SIZE.y / 2, ColorF{ 1.0, 0.5 });
	}

	// ヘッダを表示
	{
		Vec2 center{ 50, 125 };
		FontAsset(U"Common")(U"オプション").drawBase(75, center.movedBy(4, 4), ColorF{ 0.0,0.5 });
		FontAsset(U"Common")(U"オプション").drawBase(75, center, Palette::Deepskyblue);
	}

	// マウスオーバー時
	if (buttonQuit.stretched(40, 0).mouseOver())
	{
		buttonQuit.stretched(40, 0).rounded(40).draw(Palette::Silver);
		FontAsset(U"Common")(U"タイトルへ").drawAt(40, buttonQuit.center(), Palette::Deepskyblue);
	}
	else
	{
		buttonQuit.stretched(40, 0).rounded(40).drawFrame(3, Palette::Silver);
		FontAsset(U"Common")(U"タイトルへ").drawAt(40, buttonQuit.center(), Palette::Silver);
	}
}
