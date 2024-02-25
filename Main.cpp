#include <Siv3D.hpp> // OpenSiv3D v0.6.11

#include "Common.h"

#include "Title.h"
#include "StageSelect.h"
#include "Countdown.h"
#include "Game.h"
#include "Finish.h"
#include "Result.h"
#include "Option.h"

void Main()
{
	// ウィンドウタイトルを変更
	Window::SetTitle(U"かにたん大合戦");

	// 背景を黒くする
	Scene::SetBackground(ColorF{ 0.0 });

	// フォントアセット登録
	FontAsset::Register(U"Common", FontMethod::MSDF, 48, Typeface::Bold);

	// テクスチャアセット登録
	TextureAsset::Register(U"Marker", U"🐚"_emoji);
	TextureAsset::Register(U"Kani", U"🦀"_emoji);
	TextureAsset::Register(U"Enemy0", U"🐟"_emoji);
	TextureAsset::Register(U"Enemy1", U"🐠"_emoji);
	TextureAsset::Register(U"Enemy2", U"🐡"_emoji);
	TextureAsset::Register(U"Enemy3", U"🐙"_emoji);

	TextureAsset::Register(U"BombEffect", U"💥"_emoji);
	TextureAsset::Register(U"StanEffect", U"🐣"_emoji);

	TextureAsset::Register(U"Trophy", U"🏆"_emoji);

	TextureAsset::Register(U"BG0", U"Assets/Textures/bg_nangoku.jpg");
	TextureAsset::Register(U"BG1", U"Assets/Textures/bg_natural_ocean.jpg");

	// オーディオアセット登録
	AudioAsset::Register(U"Title", U"Assets/Audios/Happy_late_summer.mp3", Loop::Yes);
	AudioAsset::Register(U"Game", U"Assets/Audios/FESTIVAL.mp3", Loop::Yes);
	AudioAsset::Register(U"Result", U"Assets/Audios/Result.mp3", Loop::Yes);

	AudioAsset::Register(U"Trap", U"Assets/Audios/Trap.mp3");
	AudioAsset::Register(U"Attack", U"Assets/Audios/Attack.mp3");
	AudioAsset::Register(U"Damage", U"Assets/Audios/Damage.mp3");
	AudioAsset::Register(U"Select", U"Assets/Audios/Select.mp3");
	AudioAsset::Register(U"Stan", U"Assets/Audios/Stan.mp3");
	AudioAsset::Register(U"Countdown", U"Assets/Audios/se_321go4.mp3");
	AudioAsset::Register(U"Finish", U"Assets/Audios/Finish.mp3");
	AudioAsset::Register(U"Mouseover", U"Assets/Audios/Mouseover.mp3");

	AudioAsset::Register(U"Sazanami", U"Assets/Audios/Sazanami.mp3", Loop::Yes);

	// シーンを登録
	App manager;
	manager.add<Title>(State_Title);
	manager.add<StageSelect>(State_StageSelect);
	manager.add<Countdown>(State_Countdown);
	manager.add<Game>(State_Game);
	manager.add<Finish>(State_Finish);
	manager.add<Result>(State_Result);
	manager.add<Option>(State_Option);

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
