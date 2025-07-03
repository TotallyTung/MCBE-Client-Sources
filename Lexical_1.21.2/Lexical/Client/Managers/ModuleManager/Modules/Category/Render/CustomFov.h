#pragma once
#include "../../ModuleBase/Module.h"

class CustomFov : public Module {
public:
	float fov = 120.f;
	CustomFov();
};