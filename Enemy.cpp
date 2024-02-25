#include "Enemy.h"

Enemy::Enemy(EnemyType type, Vec2 pos)
	:type(type)
	, pos(pos)
	, count(0.0)
	, collision({ pos, 10.0 })
{

}

Enemy::EnemyType Enemy::GetEnemyType()
{
	return type;
}

Vec2 Enemy::GetPosition()
{
	return pos;
}

double Enemy::GetCount()
{
	return count;
}

Circle Enemy::GetCollision()
{
	return collision;
}

void Enemy::update(double deltaTime)
{

}

void Enemy::draw() const
{

}

void Enemy::MoveEnemy(double deltaTime)
{


}

void Enemy::MoveCollision()
{
	collision.setPos(pos);
}

void Enemy::AddCount(double deltaTime)
{
	count += deltaTime;
}
