#pragma once

#include "Module.h"

class Silent : public IModule {
public:
	Silent();
	~Silent();
	virtual const char* getModuleName() override;
};
