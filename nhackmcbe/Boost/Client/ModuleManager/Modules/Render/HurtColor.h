#pragma once
#include "../Module.h"

class HurtColor : public Module {
public:
	bool fade = false;
	UIColor hurtColor = (255, 0, 0, 175);
	HurtColor();
};