#include "EnemyTropical.h"

EnemyTropical::EnemyTropical(Vec2 pos, Vec2 playerPos)
	:Enemy(EnemyType::Enemy_Tropical, pos)
	, playerPos(playerPos)
{

}

void EnemyTropical::update(double deltaTime)
{
	MoveEnemy(deltaTime);
	MoveCollision();
}

void EnemyTropical::draw() const
{
	TextureAsset(U"Enemy1").scaled(0.5).drawAt(pos);
}

void EnemyTropical::MoveEnemy(double deltaTime)
{
	Vec2 distance = pos - playerPos;
	pos -= distance.normalized() * ENEMY_SPEED * deltaTime;
}
