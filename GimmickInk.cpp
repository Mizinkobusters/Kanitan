#include "GimmickInk.h"

GimmickInk::GimmickInk(Vec2 pos)
	:Gimmick(Gimmick::Gimmick_Spike, pos)
	, alpha(1.0)
	, ink({ pos, 300.0 })
{

}

void GimmickInk::update(double deltaTime)
{
	MoveGimmick(deltaTime);
	MoveCollision();
}

void GimmickInk::draw() const
{
	ink.draw(color);
}

double GimmickInk::GetAlpha()
{
	return alpha;
}

void GimmickInk::MoveGimmick(double deltaTime)
{
	alpha -= GIMMICK_SPEED * deltaTime;
	color = ColorF(0.0, alpha);
}
