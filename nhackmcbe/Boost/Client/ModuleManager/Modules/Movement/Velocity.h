#pragma once
#include "../Module.h"

class Velocity : public Module {
public:
	float xzModifier = 0.f;
	float yModifier = 0.f;
	Velocity();

	virtual std::string getModName() override;
};
