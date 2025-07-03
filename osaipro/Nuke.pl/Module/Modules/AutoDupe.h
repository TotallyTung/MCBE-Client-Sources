#pragma once
#include "../ModuleManager.h"
#include "Module.h"
class AutoDupe : public IModule {
public:
	int packet = 5;
	float delay = 1.f;
	float nowtickdelay = 0.f;
//	bool _2e = true;
	AutoDupe() : IModule(0x0, Category::COMBAT, "AutoDupe") {
		registerIntSetting("Packet", &packet, 5, 0, 40);
		registerFloatSetting("Delay", &delay, 1.f, 0.f, 10.f);
//		registerBoolSetting("2b2e", &_2e, true);
	};
	~AutoDupe() {};
	virtual void onTick(GameMode* gm) override;
	virtual const char* getModuleName() override {
		return "AutoDupe";
	}
};
#pragma once
