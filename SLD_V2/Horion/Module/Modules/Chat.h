#pragma once
#include "Module.h"
class Chat : public IModule {
public:

	std::string message = "SLD V2";

	inline std::string& getMessage() { return message; };

	Chat();
	~Chat();

	// Inherited via IModule
	virtual const char* getModuleName() override;
};
