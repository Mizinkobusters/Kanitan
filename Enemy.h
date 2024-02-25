#pragma once

class Enemy
{
public:
	enum EnemyType
	{
		Enemy_Fish,
		Enemy_Tropical,
		Enemy_Blow,
		Enemy_Octopus,
	};

	Enemy(EnemyType type, Vec2 pos);
	virtual void update(double deltaTime);
	virtual void draw() const;
	virtual void MoveEnemy(double deltaTime);

	EnemyType GetEnemyType();
	Vec2 GetPosition();
	double GetCount();
	Circle GetCollision();

	void AddCount(double deltaTime);

protected:
	void MoveCollision();

	EnemyType type;
	Vec2 pos;
	double count;
	Circle collision;
};
