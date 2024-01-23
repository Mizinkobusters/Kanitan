#include "Countdown.h"

Countdown::Countdown(const InitData& init)
	: IScene{ init }
{

}

void Countdown::update()
{
	if (timer <= 0)
	{
		changeScene(State_Game, 0);
	}
	else
	{
		timer -= Scene::DeltaTime();
	}
}

void Countdown::draw() const
{
	// 背景を描画
	{
		TextureAsset(U"BG1").drawAt(SCREEN_SIZE.x / 2, SCREEN_SIZE.y / 2);
	}

	// 文字を表示
	{
		timer - 0.5 > 0.5 ?
			FontAsset(U"Common")(U"{:.0f}"_fmt(timer - 0.5)).drawAt(100, Scene::CenterF())
			: FontAsset(U"Common")(U"START!!!").drawAt(100, Scene::CenterF());

	}

}
