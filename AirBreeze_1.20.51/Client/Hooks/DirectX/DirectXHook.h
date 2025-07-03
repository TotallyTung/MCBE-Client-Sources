#pragma once

#include "../Hook.h"

class Hook_DirectX : public Hook {
public:
	Hook_DirectX(Manager* mgr) : Hook(mgr) {};
public:
	auto init(void)->StatusData override;
};