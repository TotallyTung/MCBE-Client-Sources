#pragma once
#include "Module.h"

class NoRender : public IModule {
public:
	bool blockEntities = false;
	bool particles = false;
	bool entities = false;
	bool weather = false;
	bool fire = false;

	NoRender();
	virtual const char* getModuleName();
};