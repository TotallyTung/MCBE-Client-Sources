#pragma once
#include "Module.h"
class MoreFov : public IModule {
public:
	static inline float fovs = 120.f;
	MoreFov() : IModule(0x0, Category::VISUAL, "More Fovs!") {
		registerFloatSetting("Fov", &fovs, fovs, 0.f, 180.f);
	};
	~MoreFov() {};
	virtual const char* getModuleName() override {
		return "MoreFov";
	}
};