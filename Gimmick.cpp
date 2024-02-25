#include "Gimmick.h"

Gimmick::Gimmick(GimmickType type, Vec2 pos)
	:type(type)
	, pos(pos)
	, count(0.0)
	, collision({ pos, 10.0 })
{

}

Gimmick::GimmickType Gimmick::GetGimmickType()
{
	return type;
}

Vec2 Gimmick::GetPosition()
{
	return pos;
}

double Gimmick::GetCount()
{
	return count;
}

Circle Gimmick::GetCollision()
{
	return collision;
}

void Gimmick::update(double deltaTime)
{

}

void Gimmick::draw() const
{

}

void Gimmick::MoveGimmick(double deltaTime)
{


}

void Gimmick::MoveCollision()
{
	collision.setPos(pos);
}

void Gimmick::AddCount(double deltaTime)
{
	count += deltaTime;
}
