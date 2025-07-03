#include "../Module.h"

class AutoSwitch : public IModule
{
private:
	int switchSlot = 0;

public:
	int slotMin = 2;
	int slotMax = 3;
	int slot = 3;
	bool silent = true;
	int silentSlot = 0;
	AutoSwitch();

	virtual void onTick(C_GameMode *gm);
	virtual const char *getRawModuleName();
};
