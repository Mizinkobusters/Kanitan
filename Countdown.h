#pragma once

#include "Common.h"

class Countdown : public App::Scene
{
public:
	Countdown(const InitData& init);
	void update() override;
	void draw() const override;

private:
	double timer = 3.0;
};
