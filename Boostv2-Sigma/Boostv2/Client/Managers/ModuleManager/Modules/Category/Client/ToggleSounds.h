#pragma once
#include "../../ModuleBase/Module.h"

class ToggleSounds : public Module {
public:

	ToggleSounds();
	~ToggleSounds();

	bool isVisible() override;
};