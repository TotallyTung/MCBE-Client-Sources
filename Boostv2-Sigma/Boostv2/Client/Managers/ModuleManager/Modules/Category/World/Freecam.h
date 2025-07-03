#pragma once

#include "../../ModuleBase/Module.h"

class Freecam : public Module {
public:
	float speed = 0.325f;
	float yaw = 0;
	Vec2<float> initialViewAngles;
	bool keyPressed = false;
	Vec3<float> oldPos;

	Freecam();
	~Freecam();

	// Inherited via IModule
	virtual void onMCRender(MinecraftUIRenderContext* rcx) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
};