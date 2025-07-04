#pragma once
#include "../../Module.h"

class Swing : public Module {
public:
	int speed = 15;
	Swing();
	virtual const char* getModuleName() override;
};