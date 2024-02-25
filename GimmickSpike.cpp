#include "GimmickSpike.h"

GimmickSpike::GimmickSpike(Vec2 pos, Vec2 playerPos)
	:Gimmick(Gimmick::Gimmick_Spike, pos)
	, playerPos(playerPos)
	, origin(pos)
{

}

void GimmickSpike::update(double deltaTime)
{
	MoveGimmick(deltaTime);
	MoveCollision();
}

void GimmickSpike::draw() const
{
	collision.draw(Palette::Dimgray);
}

void GimmickSpike::MoveGimmick(double deltaTime)
{
	Vec2 distance = (playerPos - origin);
	pos += distance.normalized() * GIMMICK_SPEED * deltaTime;
}
