#pragma once

#include "Common.h"

class Gimmick
{
public:
	enum GimmickType
	{
		Gimmick_Spike,
		Gimmick_Ink,
	};

	Gimmick(GimmickType type, Vec2 pos);
	virtual void update(double deltaTime);
	virtual void draw() const;
	virtual void MoveGimmick(double deltaTime);

	GimmickType GetGimmickType();
	Vec2 GetPosition();
	double GetCount();
	Circle GetCollision();

	void AddCount(double deltaTime);
	void MoveCollision();

protected:
	GimmickType type;
	Vec2 pos;
	double count;
	Circle collision;
};
