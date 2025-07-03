#pragma once

#include "Module.h"
#include "../../../Utils/Utils.h"
#include <stdlib.h> /* srand, rand */
class Spammer : public IModule {
private:
	int Packets = 150;
	int Odelay = 0;
	bool bypass = true;
	int delay = 2;
	int length = 8;
	bool rp = false;
	bool rl = true;
	bool rd = false;

	int rpmi = 0;
	int rpma = 3;

	int rlmi = 5;
	int rlma = 25;

	int rdmi = 0;
	int rdma = 3;

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
