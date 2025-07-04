#pragma once

#include "../Module.h"

class RainMode : public IModule {
public:
	RainMode();
	~RainMode();

	// Inherited via IModule
	const char* getModuleName() override;
};
#pragma once
