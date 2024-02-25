#include "EnemyOctopus.h"
#include "GimmickInk.h"

EnemyOctopus::EnemyOctopus(Vec2 pos)
	:Enemy(Enemy::Enemy_Blow, pos)
	, timer(INK_INTERVAL)
{

}

void EnemyOctopus::update(double deltaTime)
{
	SpawnInk(deltaTime);
	// ギミックを動かす
	{
		for (const auto& a : gimmicks)
		{
			a->AddCount(deltaTime);
			a->update(deltaTime);
		}
	}
}

void EnemyOctopus::draw() const
{
	TextureAsset(U"Enemy3").scaled(0.5).drawAt(pos);

	// ギミックを描画
	{
		for (const auto& a : gimmicks)
		{
			a->draw();
		}
	}
}

void EnemyOctopus::MoveEnemy(double deltaTime)
{

}

void EnemyOctopus::SpawnInk(double deltaTime)
{
	if (timer <= 0.0)
	{
		timer += INK_INTERVAL;

		// 射出可能数より下回っていれば棘を射出
		if (gimmicks.size() >= MAX_INK)
		{
			return;
		}

		gimmicks.push_back(new GimmickInk(pos));
	}
	else
	{
		timer -= deltaTime;
	}
}

void EnemyOctopus::RemoveInk()
{
	for (auto iter = gimmicks.begin(); iter != gimmicks.end();)
	{
		double alpha = (*iter)->GetAlpha();
		if (alpha <= 0.0)
		{
			iter = gimmicks.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}
