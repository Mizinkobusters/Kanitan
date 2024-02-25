#include "Finish.h"

Finish::Finish(const InitData& init)
	: IScene{ init }
{
	AudioAsset(U"Title").stop();
	AudioAsset(U"Game").stop();
	AudioAsset(U"Result").stop();
	AudioAsset(U"Sazanami").stop();

	AudioAsset(U"Finish").setVolume(getData().valueSE).play();
}

void Finish::update()
{
	if (timer <= 0)
	{
		changeScene(State_Result);
	}
	else
	{
		timer -= Scene::DeltaTime();
	}
}

void Finish::draw() const
{
	// プレイヤーを描画
	{
		TextureAsset(U"Kani").scaled(0.5).drawAt(getData().lastPlayerPos);
	}

	// 敵を表示
	{
		for (const auto& a : getData().enemys)
		{
			a->draw();
		}
	}

	// 文字を表示
	{
		getData().cause == Cause_Death ?
			FontAsset(U"Common")(U"Game Over...").drawAt(Scene::CenterF())
			: FontAsset(U"Common")(U"Finish!!!").drawAt(Scene::CenterF());
	}
}
