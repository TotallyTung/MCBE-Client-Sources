#pragma once
#include "Module.h"
class LiquidMove : public IModule {
private:
	vec3_t oldPos;
	vec3_t oldOffset = {};

public:
	LiquidMove();
	~LiquidMove();
	float speed;
	const float constantM = 0.0418411f;
	float MS = 4.31f;
	// Inherited via IModule
	virtual const char* getModuleName() override;
	//virtual void onTick(C_GameMode* gm) override;
	virtual void onMove(C_MoveInputHandler* input) override;
	//virtual void onEnable() override;
	//virtual void onDisable() override;
};