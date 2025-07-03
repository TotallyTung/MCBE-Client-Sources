#pragma once

#include "../Hook.h"

class Hook_RakPeer : public Hook {
public:
	Hook_RakPeer(Manager* mgr) : Hook(mgr) {};
public:
	auto init(void) -> StatusData override;
};