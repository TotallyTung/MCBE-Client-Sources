#pragma once
#include "../Module.h"

class Tracer : public Module {
private:
	UIColor color = UIColor(255, 255, 255, 175);
	bool mobs = false;
public:
	Tracer();
	virtual void onLevelRender() override;
};