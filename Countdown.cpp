#include "Countdown.h"

Countdown::Countdown(const InitData& init)
	: IScene{ init }
{
	AudioAsset(U"Title").stop();
	AudioAsset(U"Game").stop();
	AudioAsset(U"Result").stop();
	AudioAsset(U"Sazanami").stop();
}

void Countdown::update()
{
	if (timer <= 0.0)
	{
		changeScene(State_Game, 0.0s);
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
		if (timer <= 0.0)
		{
			FontAsset(U"Common")(U"START!!!").drawAt(100, Scene::CenterF());
		}
		else
		{
			FontAsset(U"Common")(U"{:.0f}"_fmt(timer)).drawAt(100, Scene::CenterF());
			AudioAsset(U"Countdown").setVolume(getData().valueSE).play();
		}
	}

}
