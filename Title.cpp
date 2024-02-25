#include "Title.h"

Title::Title(const InitData& init)
	: IScene{ init }
{
	// オプションファイル読み込み
	CSV option{ OPTION_DATA };

	getData().valueBGM = Parse<int32>(option[0][0]);
	getData().valueSE = Parse<int32>(option[0][1]);

	AudioAsset(U"Title").stop();
	AudioAsset(U"Game").stop();
	AudioAsset(U"Result").stop();
	AudioAsset(U"Sazanami").stop();

	// BGMを流す
	double valueBGM = getData().valueBGM * 0.1;
	double valueSE = getData().valueBGM * 0.1;

	AudioAsset(U"Title").setVolume(valueBGM).play();
	AudioAsset(U"Sazanami").setVolume(valueSE).play();

	// メニュー用ボタンスタイルを設定
	menuStyle = SimpleButton::ButtonStyle(Palette::Deepskyblue, Palette::Slategray, U"Common", { 40, 0 }, 40.0);

	// タイトル用ボタンを格納
	for (int32 i = 0; i < 4; ++i)
	{
		menuRects.push_back(RectF{ Arg::center(SCREEN_SIZE.x / 2, SCREEN_SIZE.y * 5 / 9 + 75 * i), 400, 50 });
		menuButtons.push_back(new SimpleButton(menuRects[i], texts[i], menuStyle, true));
	}
}

void Title::update()
{
	// ボタンを押したときの挙動
	{
		if (menuButtons[0]->isLeftClicked())
		{
			changeScene(State_StageSelect);
			AudioAsset(U"Select").play();
		}
		else if (menuButtons[1]->isLeftClicked())
		{
			System::LaunchBrowser(U"https://github.com/Mizinkobusters/Kanitan#how-to-play");
		}
		else if (menuButtons[2]->isLeftClicked())
		{
			changeScene(State_Option);
		}
		else if (menuButtons[3]->isLeftClicked())
		{
			System::Exit();
		}
	}
}

void Title::draw() const
{
	// 背景を描画
	{
		// ガウスぼかしを1回かける
		Texture bg{ TextureAsset(U"BG0") };
		Shader::GaussianBlur(bg, internalTex, blur);
		blur.scaled(1.5).drawAt(SCREEN_SIZE.x / 2, SCREEN_SIZE.y / 2);
	}

	// タイトルを表示
	{
		Vec2 center{ SCREEN_SIZE.x / 2, SCREEN_SIZE.y * 3 / 10 };
		FontAsset(U"Common")(U"かにたん大合戦").drawAt(100, center.movedBy(4, 4), ColorF{ 0.0,0.5 });
		FontAsset(U"Common")(U"かにたん大合戦").drawAt(100, center, Palette::Deepskyblue);
	}

	// ボタンを表示
	{
		for (int32 i = 0; i < menuButtons.size(); i++)
		{
			menuButtons[i]->draw();
		}
	}
}
