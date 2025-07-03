#pragma once
#include "../Hook.h"

class Hook_GameMode : public Hook {
public:
	Hook_GameMode(Manager* mgr) : Hook(mgr) {};
	auto init(void) -> StatusData override;
};