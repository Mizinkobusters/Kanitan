#pragma once

#include "Common.h"

class Finish : public App::Scene
{
public:
	Finish(const InitData& init);
	void update() override;
	void draw() const override;

private:
	double timer = 2.0;
};
