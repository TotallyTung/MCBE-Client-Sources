#pragma once
#include "../Module.h"

class PacketMine : public IModule {
public:
	float range = 7.f;
	SettingEnum Mode = SettingEnum(this);
	SettingEnum RenderMode = SettingEnum(this);
	SettingEnum Continues = SettingEnum(this);
	float MineSpeed = 1.f;
	float ResetValue = 0.f;
	bool silentSwitch = false;
	bool silentBack = true;
	bool multiTask = false;
	//	bool strict = true;
	bool GrowRender = true;
	float ColorR = 1;
	float ColorG = 1;
	float ColorB = 1;
	float Opacity = 1;
	bool rainbow = true;
	vec3_ti blockPos = vec3_ti(0, 0, 0);
private:
	unsigned char face = 0;
	int lastHotbar = -1;
	float animDuration = 0.f;
	float lastDestroyRate = 0.f;


public:
	PacketMine();
	bool DoWeHavePickaxe(C_LocalPlayer* lp);
	bool isHoldingPickaxe(C_LocalPlayer* lp);
	std::pair<int, float> getBestPickaxeSlot(C_LocalPlayer* lp, C_GameMode* gm, C_Block* block);
	//float getMaxDestroyRate(GameMode* gm, int slot);
	void Reset(C_GameMode* gm = nullptr);
	void setBreakPos(C_GameMode* gm, vec3_ti* blockPos, unsigned char face);
	void updateBreak(C_LocalPlayer* lp, C_GameMode* gm);

	virtual const char* getModuleName() override;
	virtual void onDisable() override;
	virtual void onLevelRender() override;
};
