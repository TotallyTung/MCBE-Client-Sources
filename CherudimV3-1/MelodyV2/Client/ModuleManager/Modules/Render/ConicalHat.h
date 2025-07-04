#pragma once
#include "../Module.h"

class ConicalHat : public Module {
private:
	UIColor chColor = UIColor(255, 255, 255, 255);
	bool rainbow = false;
public:
	ConicalHat();
	virtual void onLevelRender() override;
};