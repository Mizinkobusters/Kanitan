#include "Option.h"

Option::Option(const InitData& init)
	: IScene{ init }
{
	valueBGM = getData().valueBGM;
	valueSE = getData().valueSE;

	AudioAsset(U"Title").stop();
	AudioAsset(U"Game").stop();
	AudioAsset(U"Result").stop();
	AudioAsset(U"Sazanami").stop();
	// BGMを流す
	AudioAsset(U"Result").setVolume(valueBGM * 0.1).play();

	// ボタン用スタイルを設定
	quitStyle = SimpleButton::ButtonStyle(Palette::Silver, Palette::Deepskyblue, U"Common", { 40, 0 }, 40.0);
	buttonStyle = SimpleButton::ButtonStyle(Palette::Silver, Palette::Deepskyblue, U"Common", { 0, 0 }, 20.0);

	// ボタンを定義
	quitButton = new SimpleButton(quitRect, U"タイトルへ", quitStyle, true);
	bgmmButton = new SimpleButton(bgmmRect, U"-", buttonStyle, true);
	bgmpButton = new SimpleButton(bgmpRect, U"+", buttonStyle, true);
	semButton = new SimpleButton(semRect, U"-", buttonStyle, true);
	sepButton = new SimpleButton(sepRect, U"+", buttonStyle, true);
}

void Option::update()
{
	if (quitButton->isLeftClicked())
	{
		// データ保存
		getData().valueBGM = valueBGM;
		getData().valueSE = valueSE;

		CSV option{ OPTION_DATA };
		option[0][0] = Format(valueBGM);
		option[0][1] = Format(valueSE);

		option.save(OPTION_DATA);

		// シーン変更
		changeScene(State_Title);
	}

	Decrase(bgmmButton, bgmmRect, valueBGM);
	Decrase(semButton, semRect, valueSE);
	Increase(bgmpButton, bgmpRect, valueBGM);
	Increase(sepButton, sepRect, valueSE);

	AudioAsset(U"Result").fadeVolume(valueBGM * 0.1, 0.0s);
}

void Option::Decrase(SimpleButton*& button, RectF& rect, int32& value)
{
	if (value <= 0)
	{
		button = new SimpleButton(rect, U"-", buttonStyle, false);
	}
	else
	{
		button = new SimpleButton(rect, U"-", buttonStyle, true);
		if (button->isLeftClicked())
		{
			value--;
		}
	}
}

void Option::Increase(SimpleButton*& button, RectF& rect, int32& value)
{
	if (value >= 10)
	{
		button = new SimpleButton(rect, U"+", buttonStyle, false);
	}
	else
	{
		button = new SimpleButton(rect, U"+", buttonStyle, true);
		if (button->isLeftClicked())
		{
			value++;
		}
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

	// BGMを表示
	{
		Vec2 center{ 50, 175 };
		FontAsset(U"Common")(U"BGM").drawBase(30, center.movedBy(3, 3), ColorF{ 0.0,0.5 });
		FontAsset(U"Common")(U"BGM").drawBase(30, center, Palette::Deepskyblue);

		FontAsset(U"Common")(U"{}"_fmt(valueBGM)).drawAt(60, { SCREEN_SIZE.x / 2, 250 }, Palette::White);

		bgmmButton->draw();
		bgmpButton->draw();

	}

	// SEを表示
	{
		Vec2 center{ 50, 325 };
		FontAsset(U"Common")(U"SE").drawBase(30, center.movedBy(3, 3), ColorF{ 0.0,0.5 });
		FontAsset(U"Common")(U"SE").drawBase(30, center, Palette::Deepskyblue);

		FontAsset(U"Common")(U"{}"_fmt(valueSE)).drawAt(60, { SCREEN_SIZE.x / 2, 400 }, Palette::White);

		semButton->draw();
		sepButton->draw();
	}

	// ボタンを表示
	{
		quitButton->draw();
	}
}
