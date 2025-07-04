#pragma once
#include "../../ModuleBase/Module.h"

class UIBlur : public Module {
private:
public:
	UIBlur();
	virtual void onD2DRender() override;
};