#include "Game.h"

Game::Game(const InitData& init)
	: IScene{ init }
{
	// グローバルオーディオを一斉停止
	GlobalAudio::PauseAll();
	// BGMを流す
	AudioAsset(U"Game").play(MixBus0);
}

// 状態更新
void Game::update()
{
	// 前フレームからの経過時間
	const double deltaTime = Scene::DeltaTime();

	// いろいろやる
	CountdownTimer(deltaTime);
	SpawnEnemy(deltaTime);
	MovePlayer(deltaTime);
	ConsumePower(deltaTime);
	RegeneratePower(deltaTime);
	PlaceMarker(deltaTime);
	ClearMarker(deltaTime);
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
		// 無敵中なら半透明で描画する
		playerIsInvincible ?
			TextureAsset(U"Kani").scaled(0.5).drawAt(playerPos, ColorF{ 1.0, 0.5 })
			: TextureAsset(U"Kani").scaled(0.5).drawAt(playerPos);
	}

	// マーカーを表示
	{
		for (const auto& a : markers)
		{
			TextureAsset(U"Marker").scaled(0.2).drawAt(a);
		}
	}

	// 敵を表示
	{
		for (const auto& a : enemys)
		{
			TextureAsset(U"Enemy{}"_fmt(a.type)).scaled(0.5).drawAt(a.pos);
		}
	}

	// デバッグ情報を描画
	{
		// 最大コンボを描画
		FontAsset(U"Common")(U"MAX COMBO: {:>4}"_fmt(gameMaxCombo)).draw(30, Arg::topRight(SCREEN_SIZE.x, 0));
		// スコアを描画
		FontAsset(U"Common")(U"SCORE: {:>4}"_fmt(gameScore)).draw(30, Arg::topRight(SCREEN_SIZE.x, 40));
		// タイマーを描画
		FontAsset(U"Common")(U"TIME: {:0>2.0f}"_fmt(gameTimer)).draw(30, Arg::topRight(SCREEN_SIZE.x, 80));

		// ライフを描画
		FontAsset(U"Common")(U"LIFE: {}"_fmt(playerLife)).draw(30, 0, 0);
		// 体力を描画
		FontAsset(U"Common")(U"POWER: {:.1f}"_fmt(playerPower)).draw(30, 0, 40);
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
		
		int32 type = Random(0, 4);
		Vec2 pos = RandomVec2(RectF{ 50, 50, 700, 500 });
		Circle collision = Circle{ pos, 10 };

		enemys.push_back(Enemy(type, pos, collision));
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

	if (KeyLeft.pressed())
	{
		playerPos.x -= (PLAYER_SPEED * deltaTime);
	}
	if (KeyRight.pressed())
	{
		playerPos.x += (PLAYER_SPEED * deltaTime);
	}
	if (KeyUp.pressed())
	{
		playerPos.y -= (PLAYER_SPEED * deltaTime);
	}
	if (KeyDown.pressed())
	{
		playerPos.y += (PLAYER_SPEED * deltaTime);
	}

	// プレイヤーが画面外に出ないようにする
	playerPos.x = Clamp(playerPos.x, 0.0, SCREEN_SIZE.x);
	playerPos.y = Clamp(playerPos.y, 0.0, SCREEN_SIZE.y);

	// 当たり判定をプレイヤーの位置に追従させる
	playerCollision = Circle{ playerPos, 15 };
}

// プレイヤー行動時のコスト消費に関する処理
void Game::ConsumePower(const double deltaTime)
{
	// スペースキーを押している間は体力を消費し続ける
	if (KeySpace.pressed())
	{
		playerPower -= (PLAYER_COST * deltaTime);
	}

	// 体力が0になると気絶フラグが立つ
	if (playerPower <= 0.0)
	{
		playerIsStan = true;
	}

	// プレイヤーの体力が範囲外にならないようにする
	playerPower = Clamp(playerPower, 0.0, 100.0);	
}

// プレイヤーの体力を徐々に回復する処理
void Game::RegeneratePower(const double deltaTime)
{
	// スペースキーが押されていない間は体力が回復し続ける
	if (!KeySpace.pressed())
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
	if (!KeySpace.pressed())
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
void Game::ClearMarker(const double deltaTime)
{
	if (KeySpace.pressed())
	{
		return;
	}

	// マーカーが既定数設置されていたら範囲内の敵を爆破する処理
	if (markers.size() != MAX_MARKER)
	{
		return;
	}

	// 範囲を表す矩形を作成
	Quad q = { markers[0], markers[1], markers[2], markers[3] };

	// 矩形を描画
	q.draw(ColorF{ 1.0, 0.2 });

	// 敵が範囲内に居たらスコアを加算
	for (auto iter = enemys.begin(); iter != enemys.end();)
	{
		if (q.intersects(iter->collision))
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

// 敵とぶつかったときに処理
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
		if (playerCollision.intersects(iter->collision))
		{
			// ぶつかった敵を削除
			iter = enemys.erase(iter);
			// ライフを減少させる
			playerLife--;
			// 無敵フラグを立てる
			playerIsInvincible = true;
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
}
