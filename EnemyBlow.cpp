#include "EnemyBlow.h"
#include "GimmickSpike.h"

EnemyBlow::EnemyBlow(Vec2 pos, Vec2 playerPos)
	:Enemy(Enemy::Enemy_Blow, pos)
	, timer(SPIKE_INTERVAL)
	, playerPos(playerPos)
{

}

void EnemyBlow::update(double deltaTime)
{
	SpawnSpike(deltaTime);

	// ギミックを動かす
	{
		for (const auto& a : gimmicks)
		{
			a->AddCount(deltaTime);
			a->update(deltaTime);
		}
	}
}

void EnemyBlow::draw() const
{
	TextureAsset(U"Enemy2").scaled(0.5).drawAt(pos);
	// ギミックを描画
	{
		for (const auto& a : gimmicks)
		{
			a->draw();
		}
	}
}

void EnemyBlow::MoveEnemy(double deltaTime)
{
	SpawnSpike(deltaTime);
}

// 棘発射
void EnemyBlow::SpawnSpike(double deltaTime)
{
	if (timer <= 0.0)
	{
		timer += SPIKE_INTERVAL;

		// 射出可能数より下回っていれば棘を射出
		if (gimmicks.size() >= MAX_SPIKE)
		{
			return;
		}

		gimmicks.push_back(new GimmickSpike(pos, playerPos));
	}
	else
	{
		timer -= deltaTime;
	}
}

void EnemyBlow::RemoveSpike()
{
	for (auto iter = gimmicks.begin(); iter != gimmicks.end();)
	{
		double x = (*iter)->GetPosition().x;
		double y = (*iter)->GetPosition().y;

		if (!(InRange(x, 0.0, SCREEN_SIZE.x)
			&& InRange(y, 0.0, SCREEN_SIZE.y)))
		{
			// 敵削除
			iter = gimmicks.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}
