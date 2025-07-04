#pragma once
#include "../../Module.h"

class BlockReach : public Module {
public:
	float blockReach = 7.f;

	BlockReach();
	
	virtual const char* getModuleName() override;
};
