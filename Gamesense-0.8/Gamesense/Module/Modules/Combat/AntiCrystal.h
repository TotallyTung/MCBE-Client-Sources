#pragma once
#include "../Module.h"

class AntiCrystal : public IModule
{
private:
	bool Bypass = false;
	bool endzone = false;
	float timer = 0.f;
	float height = 0.1f;

public:
	const char* name = "AntiCrystal";
	bool sink = true;

	AntiCrystal();
	~AntiCrystal();

	virtual const char *getRawModuleName() override;
	virtual const char* getModName();
	virtual void onSendPacket(C_Packet *packet) override;
};
