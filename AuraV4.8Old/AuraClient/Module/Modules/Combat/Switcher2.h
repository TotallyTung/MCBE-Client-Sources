#include "../Module.h"

class Switcher2 : public IModule {
private:
	int ffcc = 0;
public:
	bool fastmode = false;
	int slslot = 3;
	Switcher2();

//	virtual void onAttack(C_Entity* attackedEnt);
//	virtual void onPlayerTick(C_GameMode* gm);
	virtual void onPlayerTick(C_Player* plr);
	virtual void onTick(C_GameMode* gm);
	virtual const char* getModuleName();
//	virtual void onEnable();
};
