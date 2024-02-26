#include "Result.h"

Result::Result(const InitData& init)
	: IScene{ init }
{
	// BGMを流す
	double value = getData().valueBGM * 0.1;
	AudioAsset(U"Result").setVolume(value).play();

	// ランクを算出
	{
		// ゲームオーバーならランクをDにする
		if (getData().cause == Cause_Death)
		{
			rank = Rank_D;
		}
		else // それ以外ならスコアに応じてS～Cにする
		{
			if (getData().lastScore < Rank_B)
			{
				rank = Rank_C;
			}
			else if (getData().lastScore < Rank_A)
			{
				rank = Rank_B;
			}
			else if (getData().lastScore < Rank_S)
			{
				rank = Rank_A;
			}
			else
			{
				rank = Rank_S;
			}
		}
	}

	// ボタンスタイルを設定
	buttonStyle = SimpleButton::ButtonStyle(Palette::Silver, Palette::Deepskyblue, U"Common", { 40,0 }, 40.0);

	// ボタンを定義
	retryButton = new SimpleButton(retryRect, U"もう一度！", buttonStyle, true);
	quitButton = new SimpleButton(quitRect, U"タイトルへ", buttonStyle, true);
}

void Result::update()
{
	if (retryButton->isLeftClicked())
	{
		changeScene(State_Countdown, 0.5s);
		AudioAsset(U"Select").setVolume(getData().valueSE).play();
	}
	else if (quitButton->isLeftClicked())
	{
		changeScene(State_Title);
	}
}

void Result::draw() const
{
	// 背景を描画
	{
		TextureAsset(U"BG0").scaled(1.5).drawAt(SCREEN_SIZE.x / 2, SCREEN_SIZE.y / 2, ColorF{ 1.0, 0.5 });
	}

	// リザルトを表示
	{
		Vec2 center{ 50, 125 };
		FontAsset(U"Common")(U"リザルト").drawBase(75, center.movedBy(4, 4), ColorF{ 0.0,0.5 });
		FontAsset(U"Common")(U"リザルト").drawBase(75, center, Palette::Deepskyblue);


		FontAsset(U"Common")(U" スコア: {:>4}"_fmt(getData().lastScore))
			.draw(40, 0, 175)
			.stretched(40, 5)
			.shearedX(-40)
			.drawFrame(3, Palette::Deepskyblue);
		FontAsset(U"Common")(U" 最大コンボ: {:>4}"_fmt(getData().lastMaxCombo))
			.draw(40, 0, 275)
			.stretched(50, 5)
			.shearedX(-40)
			.drawFrame(3, Palette::Deepskyblue);
		FontAsset(U"Common")(U" 捕獲した数: {:>4}"_fmt(getData().lastKillCount))
			.draw(40, 0, 375)
			.stretched(100, 5)
			.shearedX(-40)
			.drawFrame(3, Palette::Deepskyblue);
	}

	// ランクを表示
	{
		Vec2 center{ 500, 125 };
		FontAsset(U"Common")(U"Rank").drawBase(30, center.movedBy(3, 3), ColorF{ 0.0,0.5 });
		FontAsset(U"Common")(U"Rank").drawBase(30, center, Palette::Deepskyblue);

		String s = U"";
		switch (rank)
		{
		case Rank_S:
			s = U"S";
			break;
		case Rank_A:
			s = U"A";
			break;
		case Rank_B:
			s = U"B";
			break;
		case Rank_C:
			s = U"C";
			break;
		default:
			s = U"D";
			break;
		}

		FontAsset(U"Common")(s)
			.drawAt(300, SCREEN_SIZE.x * 4 / 5, SCREEN_SIZE.y / 2, Palette::Deepskyblue);
	}

	// ボタンを表示
	{
		quitButton->draw();
		retryButton->draw();
	}
}
