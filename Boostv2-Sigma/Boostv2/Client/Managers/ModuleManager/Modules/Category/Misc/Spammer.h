#pragma once
#include "../../ModuleBase/Module.h"

class Spammer : public Module {
private:
	int mode = 1;
	int delay = 2;
	std::string message = "Boost On Top Niggers izizi";

public:
	Spammer();
	~Spammer();

	inline std::string& getMessage() { return message; };
	inline int& getDelay() { return delay; };

	// Inherited via IModule
	virtual std::string getModeText() override;
	virtual void onNormalTick(LocalPlayer* gm) override;
};
