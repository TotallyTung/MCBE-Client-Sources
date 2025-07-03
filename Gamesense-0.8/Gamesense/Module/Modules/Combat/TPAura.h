#pragma once
#include "../Module.h"
#include "../../ModuleManager.h"
#include "../../Utils/TargetUtil.h"

class TPAura : public IModule
{
private:
	int slotKA = 0;
	int slotKASmart = 0;
	int delay = 0;
	int Odelay = 0;
	void findWeaponSmart2();
	// bool autoweapon = false;
	void findWeapon2();
	bool silent = true;
	int delay1 = 0;
	int delay2 = 0;
	int turn = 0;

public:
	std::string name = "TPAura";
	bool isMobAura = false;
	float range = 6;
	float unlimRange = 50000;
	bool unlim = false;
	bool lerp = false;
	// bool isMulti = true;

	SettingEnum AutoWeapon = this;
	SettingEnum mode = this;

	TPAura();
	~TPAura();

	// Inherited via IModule
	virtual const char *getRawModuleName();
	virtual const char *getModuleName();
	virtual void onTick(C_GameMode *gm) override;
	virtual void onEnable() override;
	virtual void onSendPacket(C_Packet *packet) override;
};
