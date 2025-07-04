#pragma once
#include "../Module.h"

class ESP : public Module {
private:
	std::vector<Actor*> espList;

	bool mobs = false;
	int renderMode = 0;
	UIColor color = UIColor(0, 255, 255, 50);
	UIColor lineColor = UIColor(0, 255, 255, 255);
public:
	ESP();

	virtual void onRender(MinecraftUIRenderContext* ctx) override;
};