#include "Title.h"

Title::Title(const InitData& init)
	: IScene{ init }
{
	AudioAsset(U"Title").stop();
	AudioAsset(U"Game").stop();
	AudioAsset(U"Result").stop();
	AudioAsset(U"Sazanami").stop();

	// BGMを流す
	AudioAsset(U"Title").play();
	AudioAsset(U"Sazanami").play();


	for (int32 i = 0; i < 4; i++)
	{
		buttons.push_back(RectF{ Arg::center(SCREEN_SIZE.x / 2, SCREEN_SIZE.y * 5 / 9 + 75 * i), 400, 50 });
	}
}

void Title::update()
{
	// マウスオーバーでマウスカーソルを手のやつにする
	{
		for (int32 i = 0; i < buttons.size(); i++)
		{
			if (buttons[i].mouseOver())
			{
				Cursor::RequestStyle(CursorStyle::Hand);
			}
		}
	}

	// ボタンを押したときの挙動
	{
		if (buttons[0].leftClicked())
		{
			changeScene(State_Countdown);
			AudioAsset(U"Select").play();
		}
		else if (buttons[1].leftClicked())
		{
			System::LaunchBrowser(U"https://github.com/Mizinkobusters/Kanitan#how-to-play");
		}
		else if (buttons[2].leftClicked())
		{
			changeScene(State_Option);
		}
		else if (buttons[3].leftClicked())
		{
			System::Exit();
		}
	}

}

void Title::draw() const
{
	// 背景を描画
	{
		TextureAsset(U"BG0").scaled(1.5).drawAt(SCREEN_SIZE.x / 2, SCREEN_SIZE.y / 2);
	}

	// タイトルを表示
	{
		Vec2 center{ SCREEN_SIZE.x / 2, SCREEN_SIZE.y * 3 / 10 };
		FontAsset(U"Common")(U"かにたん大合戦").drawAt(70, center.movedBy(4, 4), ColorF{ 0.0,0.5 });
		FontAsset(U"Common")(U"かにたん大合戦").drawAt(70, center, Palette::Deepskyblue);
	}

	// ボタンを表示
	{
		for (int32 i = 0; i < buttons.size(); i++)
		{
			// マウスオーバー時
			if (buttons[i].mouseOver())
			{
				buttons[i].rounded(50).draw(Palette::Slategray);
				FontAsset(U"Common")(texts[i]).drawAt(40, buttons[i].center(), Palette::Deepskyblue);
			}
			else
			{
				buttons[i].rounded(50).drawFrame(3, Palette::Slategray);
				FontAsset(U"Common")(texts[i]).drawAt(40, buttons[i].center(), Palette::Slategray);
			}
		}
	}
}
