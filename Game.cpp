#include "Game.h"

#include "EnemyFish.h"
#include "EnemyTropical.h"
#include "EnemyBlow.h"
#include "EnemyOctopus.h"

Game::Game(const InitData& init)
	: IScene{ init }
{
	// BGMを流す
	double value = getData().valueBGM * 0.1;
	AudioAsset(U"Game").setVolume(value).play();

	// セーブデータ読み込み
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

// 状態更新
void Game::update()
{
	// 前フレームからの経過時間
	const double deltaTime = Scene::DeltaTime();

	// 敵を動かす
	{
		for (const auto& a : enemys)
		{
			a->AddCount(deltaTime);
			a->update(deltaTime);
		}
	}

	// いろいろやる
	CountdownTimer(deltaTime);
	SpawnEnemy(deltaTime);
	MovePlayer(deltaTime);
	RemoveEnemy();
	ConsumePower(deltaTime);
	RegeneratePower(deltaTime);
	PlaceMarker(deltaTime);
	ClearMarker();
	CheckCollision();
	OnInvincible(deltaTime);
}

// 描画
void Game::draw() const
{
	// 背景を描画
	{
		TextureAsset(U"BG1").drawAt(SCREEN_SIZE.x / 2, SCREEN_SIZE.y / 2);
	}

	// プレイヤーを描画
	{
		// 気絶状態ならヒヨコで描画
		if (playerIsStan)
		{
			TextureAsset(U"StanEffect").scaled(0.5).drawAt(playerPos);
		}
		else
		{
			// 無敵中なら半透明で描画する
			playerIsInvincible ?
				TextureAsset(U"Kani").scaled(0.5).drawAt(playerPos, ColorF{ 1.0, 0.5 })
				: TextureAsset(U"Kani").scaled(0.5).drawAt(playerPos);
		}

	}

	// マーカーを表示
	{
		for (const auto& a : markers)
		{
			TextureAsset(U"Marker").scaled(0.25).drawAt(a);
		}
	}

	// 敵を表示
	{
		for (const auto& a : enemys)
		{
			a->draw();
		}
	}

	// トラップ範囲を表示
	{
		// 矩形を描画
		trapRange.draw(ColorF{ 0.9, 0.1, 0.1, 0.55 });
	}

	// デバッグ情報を描画
	{
		// タイマーを描画
		DrawTimer(gameTimer);
		
		// ライフを描画
		DrawLife(playerLife);
		
		// パワーを描画
		DrawPower(playerPower);
	}
}

// ゲーム用タイマーをカウントダウンする処理
void Game::CountdownTimer(const double deltaTime)
{
	if (gameTimer > 0)
	{
		gameTimer -= deltaTime;
	}
	else
	{
		gameTimer = 0;
		SaveData();
		changeScene(State_Finish, 0);
	}
}

// 敵をランダムに出現させる処理
void Game::SpawnEnemy(const double deltaTime)
{
	if (enemyTimer <= 0.0) // タイマーが0のとき
	{
		// タイマーをリセット
		enemyTimer += ENEMY_INTERVAL;

		// 出現可能数より下回っていれば敵を出現させる
		if (enemys.size() >= MAX_ENEMY)
		{
			return;
		}

		int32 diff = getData().diff;
		int32 type = 0;
		Vec2 pos = RandomVec2(RectF{ 50, 50, 700, 500 });
		Enemy* newEnemy;
		// 難易度によって出現させる敵を変える
		switch (diff)
		{
		case StageDifficulty::Diff_Easy:
			type = Random(0, 1);
			break;
		case StageDifficulty::Diff_Normal:
			type = Random(1, 2);
			break;
		case StageDifficulty::Diff_Hard:
			type = Random(1, 3);
			break;
		}

		// タイプに対応した敵をポインタに格納
		switch (type)
		{
		case 0:
			newEnemy = new EnemyFish(pos);
			break;
		case 1:
			newEnemy = new EnemyTropical(pos, playerPos);
			break;
		case 2:
			newEnemy = new EnemyBlow(pos, playerPos);
			break;
		case 3:
			newEnemy = new EnemyOctopus(pos);
			break;
		}

		// 敵をリストに格納
		enemys.push_back(newEnemy);
	}
	else // タイマーが動いているとき
	{
		// カウントダウン
		enemyTimer -= deltaTime;
	}
}

// プレイヤーの移動に関する処理
void Game::MovePlayer(const double deltaTime)
{
	if (playerIsStan)    // 体力があるときにしか動けない
	{
		return;
	}

	//プレイヤーの移動処理
	Vec2 cursor = Cursor::PosF();
	double t = PLAYER_SPEED * deltaTime;
	playerPos = playerPos.lerp(cursor, t);

	// プレイヤーが画面外に出ないようにする
	playerPos.x = Clamp(playerPos.x, 0.0, SCREEN_SIZE.x);
	playerPos.y = Clamp(playerPos.y, 0.0, SCREEN_SIZE.y);

	// 当たり判定をプレイヤーの位置に追従させる
	playerCollision = Circle{ playerPos, 15 };
}

// 画面範囲外に出た敵を削除
void Game::RemoveEnemy()
{
	for (auto iter = enemys.begin(); iter != enemys.end();)
	{
		double x = (*iter)->GetPosition().x;
		double y = (*iter)->GetPosition().y;

		if (!(InRange(x, 0.0, SCREEN_SIZE.x)
			&& InRange(y, 0.0, SCREEN_SIZE.y)))
		{
			// 敵削除
			iter = enemys.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

// プレイヤー行動時のコスト消費に関する処理
void Game::ConsumePower(const double deltaTime)
{
	// スペースキーを押している間かつプレイヤーが行動可能時は体力を消費し続ける
	if (MouseR.pressed() && !playerIsStan)
	{
		playerPower -= (PLAYER_COST * deltaTime);
	}

	// 体力が0になると気絶フラグが立つ
	if (playerPower <= 0.0)
	{
		playerIsStan = true;
		AudioAsset(U"Stan").setVolume(getData().valueSE).play();
	}

	// プレイヤーの体力が範囲外にならないようにする
	playerPower = Clamp(playerPower, 0.0, 100.0);	
}

// プレイヤーの体力を徐々に回復する処理
void Game::RegeneratePower(const double deltaTime)
{
	// 右クリックされていない間は体力が回復し続ける
	if (!MouseR.pressed())
	{
		playerPower += (POWER_REGENERATION * deltaTime);
	}

	// ある程度体力が回復したら気絶を解除する
	if (playerPower >= 3.0)
	{
		playerIsStan = false;
	}

	// プレイヤーの体力が範囲外にならないようにする
	playerPower = Clamp(playerPower, 0.0, 100.0);
}

// プレイヤーが行動している間マーカーを設置する処理
void Game::PlaceMarker(const double deltaTime)
{
	if (!MouseR.pressed())
	{
		return;
	}

	if (markerTimer <= 0.0) // タイマーが0のとき
	{
		// タイマーをリセット
		markerTimer += MARKER_INTERVAL;

		// 設置済みマーカー数が既定値以上のとき
		if (markers.size() == MAX_MARKER && MAX_MARKER > 0)
		{
			// 最も古いマーカーを削除する
			markers.pop_front();
		}

		// 同じ位置にマーカーが設置されているかチェック
		for (const auto& m : markers)
		{
			// すでに設置されていれば何もしない
			if (m == playerPos)
			{
				return;
			}
		}

		// 新規のマーカーを設置
		markers.push_back(playerPos);
	}
	else // タイマーが動いているとき
	{
		// カウントダウン
		markerTimer -= deltaTime;
	}
}

// プレイヤーが行動していない間の処理
void Game::ClearMarker()
{
	if (MouseR.pressed())
	{
		return;
	}

	// マーカーが規定数設置されていなかったら設置済みマーカーを削除
	if (markers.size() != MAX_MARKER)
	{
		markers.clear();
		return;
	}

	// マーカーが既定数設置されていたら範囲内の敵を爆破する処理
	// 範囲を表す矩形を作成
	trapRange = { markers[0], markers[1], markers[2], markers[3] };


	// 敵が範囲内に居たらスコアを加算
	for (auto iter = enemys.begin(); iter != enemys.end();)
	{
		if (trapRange.intersects((*iter)->GetCollision()))
		{
			// キルカウント加算
			gameKillCount++;
			// コンボ加算
			gameCombo++;
			// スコア加算
			gameScore += gameCombo;
			// 体力回復
			playerPower += POWER_REGENERATION;
			// 敵削除
			iter = enemys.erase(iter);
			// SE鳴らす
			AudioAsset(U"Attack").stop();
			AudioAsset(U"Attack").setVolume(getData().valueSE).play();
		}
		else
		{
			++iter;
		}
	}

	// 最大コンボを更新
	if (gameMaxCombo < gameCombo)
	{
		gameMaxCombo = gameCombo;
	}

	// コンボをリセット
	gameCombo = 0;
	
	// マーカーを削除
	markers.clear();
}

// 敵とぶつかったときの処理
void Game::CheckCollision()
{
	// 無敵中なら無視
	if (playerIsInvincible)
	{
		return;
	}

	if (enemys.size() == 0)
	{
		return;
	}

	for (auto iter = enemys.begin(); iter != enemys.end();)
	{
		if (playerCollision.intersects((*iter)->GetCollision()))
		{
			// ぶつかった敵を削除
			iter = enemys.erase(iter);
			// ライフを減少させる
			playerLife--;
			// 無敵フラグを立てる
			playerIsInvincible = true;
			//SEを鳴らす
			AudioAsset(U"Damage").stop();
			AudioAsset(U"Damage").setVolume(getData().valueSE).play();
		}
		else
		{
			++iter;
		}
	}

	// ライフが0になったらゲームオーバー
	if (playerLife == 0)
	{
		SaveData();
		changeScene(State_Finish);
	}
}

// 無敵中の処理
void Game::OnInvincible(const double deltaTime)
{
	if (!playerIsInvincible)
	{
		return;
	}

	if (invincibleTimer <= 0)	// タイマーが0のとき
	{
		invincibleTimer = INVINCIBLE_DURATION;	// タイマーをリセット
		playerIsInvincible = false;				// 無敵フラグを下ろす
	}
	else
	{
		invincibleTimer -= deltaTime;
	}
}

// シーンを跨いで使うデータを保存する
void Game::SaveData()
{
	playerLife == 0 ? getData().cause = Cause_Death : getData().cause = Cause_Timeover;	// ゲーム終了の原因を保存
	getData().lastScore = gameScore;	// スコアを保存
	getData().lastMaxCombo = gameMaxCombo; // 最大コンボを保存
	getData().lastKillCount = gameKillCount; // キルカウントを保存
	getData().lastPlayerPos = playerPos;	// プレイヤーの位置を保存
	getData().enemys = enemys;	// 敵の位置を保存

	// 最後にデータ書き出し
	WriteData();
}

// CSVファイルにデータ書き出し
void Game::WriteData()
{
	int32 row = getData().diff;
	bool isPlayable = diffs[row].isPlayable;
	bool isNoDamage = diffs[row].isNoDamage;
	bool doesGetSRank = diffs[row].doesGetSRank;
	int32 score = diffs[row].score;
	int32 maxCombo = diffs[row].maxCombo;
	int32 killCount = diffs[row].killCount;

	if (gameScore >= Rank_S)
	{
		if (!isPlayable)
		{
			isPlayable = true;
		}

		if (!doesGetSRank)
		{
			doesGetSRank = true;
		}
	}

	if (playerLife == 3 && !isNoDamage)
	{
		isNoDamage = true;
	}

	if (gameScore >= score)
	{
		score = gameScore;
	}

	if (gameMaxCombo >= maxCombo)
	{
		maxCombo = gameMaxCombo;
	}

	if (gameKillCount >= killCount)
	{
		killCount = gameKillCount;
	}

	CSV data{ SCORE_DATA };

	if (row + 1 < StageDifficulty::Diff_Max)
	{
		data[row+1][0] = Format(isPlayable);
	}
	data[row][1] = Format(isNoDamage);
	data[row][2] = Format(doesGetSRank);
	data[row][3] = Format(score);
	data[row][4] = Format(maxCombo);
	data[row][5] = Format(killCount);

	// CSVファイルを保存
	data.save(SCORE_DATA);
}

void Game::DrawTimer(double value) const
{
	Point center{ 700, 100 };
	int32 size = 70;

	Circle timerBG{ center,size };
	Circle timerPie{ center,size };

	double pie = value / 30 * Math::Pi * 2;

	double alpha = 1.0;
	// マウスカーソルと重なっていれば半透明で描画
	if (timerBG.mouseOver())
	{
		alpha = 0.5;

	}
	timerBG.draw(ColorF{ 0.7, alpha });
	timerPie.drawPie(0_deg, pie, ColorF{ 0.0, 0.8, 0.0, alpha });
	FontAsset(U"Common")(U"{:0>2.0f}"_fmt(value)).draw(50, Arg::center(center), ColorF{ 1.0, alpha });
}

void Game::DrawLife(int32 value) const
{
	RectF rect{ 0, 0, 165, 110 };
	Point tex{ 50, 50 };
	Point text{ 90, 25 };

	double alpha = 1.0;
	if (rect.mouseOver())
	{
		alpha = 0.5;

	}
	TextureAsset(U"Kani").scaled(0.5).drawAt(tex, ColorF{ 1.0, alpha });
	FontAsset(U"Common")(U"× {}"_fmt(value)).draw(50, text, ColorF{ 1.0, alpha });
}

void Game::DrawPower(double value) const
{
	RectF powerBG{ 20, 120, 300, 45 };
	double corner = value / 100 * 300;
	RectF power{ 20, 120, corner, 45 };

	double alpha = 1.0;
	if (powerBG.mouseOver())
	{
		alpha = 0.5;
	}

	powerBG.draw(ColorF{ 0.7, alpha });

	if (value < 20.0)
	{
		power.draw(ColorF{ 1.0, 0.0, 0.0, alpha });
	}
	else if (value < 50.0)
	{
		power.draw(ColorF{ 1.0, 1.0, 0.0, alpha });
	}
	else
	{
		power.draw(ColorF{ 0.0, 1.0, 0.0, alpha });
	}

	FontAsset(U"Common")(U"ぱわ～").drawAt(30, powerBG.center(), ColorF{1.0, alpha});
}
