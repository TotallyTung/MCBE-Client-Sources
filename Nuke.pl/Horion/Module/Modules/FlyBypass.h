#pragma once
#include "Module.h"
#include <chrono>
class FlyBypass : public IModule {
private:
	float glideMod = -0.001f;
	int enabledTicks;
	float speed = 0.413;
	float disableDuration = 50;

	float glideModEffective = 0;
	std::chrono::steady_clock::time_point enabledTime;

public:
	FlyBypass();
	~FlyBypass();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onMove(MoveInputHandler* input) override;
	virtual void onEnable() override;
	virtual void onDisable() override;

	//virtual const char* getRawModuleName() override;
};
