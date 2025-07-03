#pragma once
#include "../Hook.h"

class Hook_Actor : public Hook {
public:
	Hook_Actor(Manager* mgr) : Hook(mgr) {};
public:
	auto init(void) -> StatusData override;
};