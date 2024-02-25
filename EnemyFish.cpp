#include "EnemyFish.h"

EnemyFish::EnemyFish(Vec2 pos)
	:Enemy(Enemy::Enemy_Fish, pos)
{

}

void EnemyFish::update(double deltaTime)
{
	MoveEnemy(deltaTime);
	MoveCollision();
}

void EnemyFish::draw() const
{
	TextureAsset(U"Enemy0").scaled(0.5).drawAt(pos);
}

void EnemyFish::MoveEnemy(double deltaTime)
{
	pos.x -= ENEMY_SPEED * deltaTime;
}
