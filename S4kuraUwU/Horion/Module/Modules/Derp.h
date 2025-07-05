#pragma once
#include "Module.h"
class Derp : public IModule {
private:
	bool disableOnThrowableItemHeld = true;
	bool disableOnMine = true;

	bool desync = false;

		bool enableFakePitch = true;
		int fakePitch = -90;  // -90 ~ 90 where -90 is up
		int fakePitchJitter = 0;

		bool enableFakeLBY = false;
		int fakeLBY = -180;  // goes from -180 to 180
		int fakeLBYJitter = 0;

public:
	bool real = true;

		bool enableRealPitch = true;
		int realPitch = -90;  // -90 ~ 90 where -90 is up
		int realPitchJitter = 0;

		bool enableRealLBY = false;
		int realLBY = -180;  // goes from -180 to 180
		int realLBYJitter = 0;

	int pJit = 0;
	int lJit = 0;

	Derp();
	~Derp();

	// Inherited via IModule
	virtual void onTick(C_GameMode* gm) override;
	virtual const char* getModuleName() override;
};
