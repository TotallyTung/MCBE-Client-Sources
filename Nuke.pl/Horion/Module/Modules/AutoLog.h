#pragma once
#pragma once
#include "Module.h"
class AutoLog : public IModule {
public:
	bool CustomMsg = true;
	int health = 6;
	SettingEnum ByeByeMessage;
	SettingEnum LogType;
	bool message = true;
	bool OnAnvil = false;
	float height = 1.0f;
	bool caidelljthe = false;
	Vec3i blockpos3 = { 0, 0, 0 };
	int slottd2 = 0;
	bool silenttd2 = false;
	AutoLog();
	~AutoLog();
	std::string name = "AutoLog";
	bool isFallingAnvilAboveinPlayer(Player* plr);
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
};