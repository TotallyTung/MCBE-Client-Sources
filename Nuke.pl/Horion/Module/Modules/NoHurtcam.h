#pragma once

#include "Module.h"

class NoHurtcam : public IModule{
protected:
	void* cancelHurtcam = (void*)(Utils::getBase() + 0x2001781);
public:
	NoHurtcam();
	~NoHurtcam();
	
	virtual const char* getModuleName() override;
	virtual void onEnable() override;
	virtual void onDisable() override;
};
