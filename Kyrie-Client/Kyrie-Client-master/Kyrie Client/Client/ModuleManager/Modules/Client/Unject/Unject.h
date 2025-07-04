#pragma once
#include "../../Module.h"

class Unject : public Module {
public:
	Unject();
	virtual const char* getModuleName() override;
	virtual bool isVisible() override;
	virtual void onEnable() override;
};