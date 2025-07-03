#pragma once

#include "Module.h"
#include "../../../Utils/Utils.h"
#include <stdlib.h> /* srand, rand */
class Spammer : public IModule {
private:
	int Odelay = 0;
	bool bypass = true;
	int delay = 2;
	int length = 8;

public:
	Spammer();
	~Spammer();

	inline int& getDelay() { return delay; };
	inline int& getLength() { return length; };
	inline bool& getBypass() { return bypass; };

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
};
