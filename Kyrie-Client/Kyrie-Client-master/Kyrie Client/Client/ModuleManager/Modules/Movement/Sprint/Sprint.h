#pragma once
#include "../../Module.h"

class Sprint : public Module {
public:

	Sprint();
	virtual const char* getModuleName() override;
	virtual const char* getModName() override;
};