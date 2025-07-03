#pragma once
#include "../Hook.h"

class Hook_ActorRender : public Hook {
public:
	Hook_ActorRender(Manager* mgr) : Hook(mgr) {};
public:
	auto init(void) -> StatusData override;
};