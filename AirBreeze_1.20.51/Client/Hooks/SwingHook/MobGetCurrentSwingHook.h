#pragma once

#include "../Hook.h"

class Hook_MobGetCurrentSwing : public Hook {
public:
	Hook_MobGetCurrentSwing(Manager* mgr) : Hook(mgr) {};
public:
	auto init(void) -> StatusData override;
};