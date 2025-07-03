#pragma once

#include "../../DrawUtils.h"
#include "../ModuleManager.h"
#include "Module.h"

class InstaBreak : public IModule {
public:
	InstaBreak();
	~InstaBreak();
	bool wookmc = true;
	// Inherited via IModule
	virtual const char* getModuleName() override;
	static void onMouseClickUpdate(int key, bool isDown);
};
