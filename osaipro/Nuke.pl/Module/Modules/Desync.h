#pragma once

#include "Module.h"

class Desync : public IModule {
public:
	Desync();
	~Desync();
	virtual const char* getModuleName() override;
};
