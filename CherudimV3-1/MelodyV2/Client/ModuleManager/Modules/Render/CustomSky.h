#pragma once
#include "../Module.h"

class CustomSky : public Module {
private:
	
	
public:
	float intensity = 0.01f;
	bool custom = false;
	bool night = true;
	CustomSky();
	UIColor color = UIColor(0, 255, 255, 50);
	//virtual void onRender(MinecraftUIRenderContext* ctx) override;
};