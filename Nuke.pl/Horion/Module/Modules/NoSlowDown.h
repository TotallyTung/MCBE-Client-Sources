#pragma once

#include "Module.h"

class NoSlowDown : public IModule {
protected:
	void* cancelSlowDown1 = (void*)(Utils::getBase() + 0x1900D73);
	void* cancelSlowDown2 = (void*)(Utils::getBase() + 0x1900D85);
	void* cancelSlowDown3 = (void*)(Utils::getBase() + 0x29465E9);
	void* cancelSlowDown4 = (void*)(Utils::getBase() + 0x29465FB);
public:
	NoSlowDown();
	~NoSlowDown();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onEnable() override;
	virtual void onDisable() override;
};
