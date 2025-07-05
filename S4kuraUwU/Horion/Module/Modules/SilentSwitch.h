#include "Module.h"

class SilentSwitch: public IModule {
private:
	int prevSlot = 0;

public:
	SettingEnum mode = this;
	SilentSwitch();
	~SilentSwitch();
	virtual void onTick(C_GameMode* gm);
	virtual const char* getModuleName();
	virtual void onEnable();
};
