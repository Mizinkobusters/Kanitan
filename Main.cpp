﻿#include <Siv3D.hpp> // OpenSiv3D v0.6.11

#include "Common.h"

#include "Title.h"
#include "Countdown.h"
#include "Game.h"
#include "Finish.h"
#include "Result.h"

void Main()
{
	// ウィンドウタイトルを変更
	Window::SetTitle(U"カニたんの大冒険");

	// 背景を黒くする
	Scene::SetBackground(ColorF{ 0.0 });

	// フォントアセット登録
	FontAsset::Register(U"Common", FontMethod::MSDF, 48, Typeface::Bold);

	// テクスチャアセット登録
	TextureAsset::Register(U"Marker", U"💠"_emoji);
	TextureAsset::Register(U"Kani", U"🦀"_emoji);
	TextureAsset::Register(U"Enemy0", U"🐟"_emoji);
	TextureAsset::Register(U"Enemy1", U"🐠"_emoji);
	TextureAsset::Register(U"Enemy2", U"🐡"_emoji);
	TextureAsset::Register(U"Enemy3", U"🐳"_emoji);
	TextureAsset::Register(U"Enemy4", U"🦈"_emoji);

	TextureAsset::Register(U"BG0", U"Assets/Textures/bg_nangoku.jpg");
	TextureAsset::Register(U"BG1", U"Assets/Textures/bg_natural_ocean.jpg");

	// オーディオアセット登録
	AudioAsset::Register(U"Title", U"Assets/Audios/Happy_late_summer.mp3");
	AudioAsset::Register(U"Game", U"Assets/Audios/FESTIVAL.mp3");
	AudioAsset::Register(U"Result", U"Assets/Audios/夏の終わりに_2.mp3");

	// シーンを登録
	App manager;
	manager.add<Title>(State_Title);
	manager.add<Countdown>(State_Countdown);
	manager.add<Game>(State_Game);
	manager.add<Finish>(State_Finish);
	manager.add<Result>(State_Result);

	//manager.init(State_Result);

	// ゲームループ
	while (System::Update())
	{
		if (not manager.update())
		{
			break;
		}
	}
}
