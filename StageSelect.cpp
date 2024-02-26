#include "StageSelect.h"

StageSelect::StageSelect(const InitData& init)
	:IScene{ init }
{
	// セーブデータ読み込み
	{
		const CSV data{ SCORE_DATA };
		for (int32 row = 0; row < data.rows(); ++row)
		{
			diffs[row].isPlayable = Parse<bool>(data[row][0]) == 1 ? true : false;
			diffs[row].isNoDamage = Parse<bool>(data[row][1]) == 1 ? true : false;
			diffs[row].doesGetSRank = Parse<bool>(data[row][2]) == 1 ? true : false;
			diffs[row].score = Parse<int32>(data[row][3]);
			diffs[row].maxCombo = Parse<int32>(data[row][4]);
			diffs[row].killCount = Parse<int32>(data[row][5]);
		}
	}

	// ステージ選択ボタンを定義
	for (int32 i = 0; i < 3; ++i)
	{
		selectRects.push_back(RectF{ Arg::center(SCREEN_SIZE.x * 2 / 11 + 250 * i, SCREEN_SIZE.y / 2), 150, 300 });

		// セレクト用ボタンスタイルを設定
		switch (i)
		{
		case 0:
			selectStyle = SimpleButton::ButtonStyle(Palette::Greenyellow, Palette::Slategray, U"Common", { 40,0 }, 40.0);
			break;
		case 1:
			selectStyle = SimpleButton::ButtonStyle(Palette::Gold, Palette::Slategray, U"Common", { 40,0 }, 40.0);
			break;
		case 2:
			selectStyle = SimpleButton::ButtonStyle(Palette::Crimson, Palette::Slategray, U"Common", { 40,0 }, 40.0);
			break;
		}

		// ボタンをListに登録
		selectButtons.push_back(new SimpleButton(selectRects[i], U"", selectStyle, diffs[i].isPlayable));
	}

	// 戻るボタン用スタイルを設定
	quitStyle = SimpleButton::ButtonStyle(Palette::Silver, Palette::Deepskyblue, U"Common", { 40, 0 }, 40.0);

	// ボタンを定義
	quitButton = new SimpleButton(quitRect, U"タイトルへ", quitStyle, true);

	// コレクションボタン用スタイルを設定
	collectionStyle = quitStyle;

	// ボタンを定義
	collectionButton = new SimpleButton(collectionRect, U"データリセット", collectionStyle, true);
};

void StageSelect::update()
{
	// 退出ボタン
	if (quitButton->isLeftClicked())
	{
		changeScene(State_Title);
	}

	// ステージ選択ボタン
	if (selectButtons[0]->isLeftClicked())
	{
		getData().diff = StageDifficulty::Diff_Easy;
		changeScene(State_Countdown, 0.5s);
	}
	else if(selectButtons[1]->isLeftClicked())
	{
		getData().diff = StageDifficulty::Diff_Normal;
		changeScene(State_Countdown, 0.5s);
	}
	else if(selectButtons[2]->isLeftClicked())
	{
		getData().diff = StageDifficulty::Diff_Hard;
		changeScene(State_Countdown, 0.5s);
	}

	// データリセット
	if (collectionButton->isLeftClicked())
	{
		CSV data{ SCORE_DATA };
		for (int32 row = 0; row < data.rows(); ++row)
		{
			diffs[row].isNoDamage = false;
			diffs[row].doesGetSRank =  false;
			diffs[row].score = 0;
			diffs[row].maxCombo = 0;
			diffs[row].killCount = 0;

			data[row][1] = Format(false);
			data[row][2] = Format(false);
			data[row][3] = Format(0);
			data[row][4] = Format(0);
			data[row][5] = Format(0);
		}

		data.save(SCORE_DATA);

	}
}

void StageSelect::draw() const
{
	// 背景を描画
	{
		TextureAsset(U"BG0").scaled(1.5).drawAt(SCREEN_SIZE.x / 2, SCREEN_SIZE.y / 2, ColorF{ 1.0, 0.5 });
	}

	// ヘッダを表示
	{
		Vec2 center{ 50, 125 };
		FontAsset(U"Common")(U"ステージ選択").drawBase(75, center.movedBy(4, 4), ColorF{ 0.0,0.5 });
		FontAsset(U"Common")(U"ステージ選択").drawBase(75, center, Palette::Deepskyblue);
	}

	// タイトルへ戻るボタンを描画
	{
		quitButton->draw();
	}

	// コレクションボタンを描画
	{
		collectionButton->draw();
	}

	// 選択ボタンを描画
	{
		for (int32 i = 0; i < selectButtons.size(); ++i)
		{
			Vec2 pos = { SCREEN_SIZE.x * 2 / 11 + 250 * i, 175 };
			selectButtons[i]->draw();
			DrawDiffStatus(pos, i);
		}
	}
}

void StageSelect::DrawDiffStatus(Vec2 pos, int32 diff) const
{
	// 難易度を表示
	{
		FontAsset(U"Common")(msgDiff[diff]).drawAt(30, pos, Palette::White);
	}

	// データとか表示
	{
		Array<String> displayData =
		{ U"スコア: {}"_fmt(diffs[diff].score),
		U"最大コンボ: {}"_fmt(diffs[diff].maxCombo),
		U"捕獲した数: {}"_fmt(diffs[diff].killCount),
		U"Sランククリア",
		U"ノーダメージクリア" };

		for (int32 i = 0; i < displayData.size(); ++i)
		{
			Vec2 newPos = { pos.x, pos.y + 40 * (i + 1) };
			FontAsset(U"Common")(displayData[i]).drawAt(20, newPos, Palette::White);
		}
	}

	// 条件達成していたらトロフィー表示
	{
		if (diffs[diff].doesGetSRank)
		{
			Vec2 newPos = { pos.x - 90, pos.y + 160 };
			TextureAsset(U"Trophy").scaled(0.25).drawAt(newPos);
		}

		if (diffs[diff].isNoDamage)
		{
			Vec2 newPos = { pos.x - 90, pos.y + 200 };
			TextureAsset(U"Trophy").scaled(0.25).drawAt(newPos);
		}
	}
}
