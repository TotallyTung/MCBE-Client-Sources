#pragma once
#include "../../Module.h"

class CustomFov : public Module {
public:
	float fov = 120.f;
	CustomFov();
	virtual const char* getModuleName() override;
};