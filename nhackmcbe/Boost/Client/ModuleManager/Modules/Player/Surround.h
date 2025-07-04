#pragma once
#include "../Module.h"

class Surround : public Module {
private:
	bool disableComplete = false;
	bool center = true;
	bool render = true;

	int switchMode = 0;

	UIColor color = UIColor(255, 0, 0, 60);
	UIColor lineColor = UIColor(244, 0, 0, 255);

	int getObsidian();
	void tryBuildBlock(Vec3<int> tryBuildPos);
	void predictBlock(Vec3<float> pos);
public:
	Surround();
	virtual void onNormalTick(Actor* actor) override;
	virtual void onRender(MinecraftUIRenderContext* ctx) override;
	virtual void onEnable() override;
};