#pragma once
#include "Module.h"
class Greeter : public IModule {
public:
	SettingEnum mode = this;
	bool AntiCollapseMode = true;
	Greeter() : IModule(0x0, Category::MISC, u8"Ž©“®ˆ¥ŽA") {
		mode
			.addEntry("Nuke.jp", 0)
			.addEntry("Banana", 1)
			.addEntry("Nova", 2);
		registerEnumSetting("Mode", &mode, 0);
		registerBoolSetting("Anti-Collapse", &AntiCollapseMode, true);
	};
	~Greeter() {};
	virtual const char* getModuleName() override {
		return "Greeter";
	}
};