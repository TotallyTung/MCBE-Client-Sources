#pragma once
#include "Module.h"

class PacketMine : public IModule {
public:
	float range = 7.f;
	SettingEnum Mode = SettingEnum(this);
	SettingEnum Continues = SettingEnum(this);
	float MineSpeed = 1.f;
	float ResetValue = 0.f;
	bool silentSwitch = false;
	bool silentBack = true;
	bool multiTask = false;
//	bool strict = true;
	bool GrowRender = true;

private:
	Vec3i blockPos = Vec3i(0, 0, 0);
	unsigned char face = 0;
	int lastHotbar = -1;
	float animDuration = 0.f;
	float lastDestroyRate = 0.f;
	

public:
	PacketMine();
	bool DoWeHavePickaxe(LocalPlayer* lp);
	bool isHoldingPickaxe(LocalPlayer* lp);
	std::pair<int, float> getBestPickaxeSlot(LocalPlayer* lp, GameMode* gm, Block* block);
	//float getMaxDestroyRate(GameMode* gm, int slot);
	void Reset(GameMode* gm = nullptr);
	void setBreakPos(GameMode* gm, Vec3i* blockPos, unsigned char face);
	void updateBreak(LocalPlayer* lp, GameMode* gm);

	virtual const char* getModuleName() override;
	virtual const char* getModName() override;
	virtual void onDisable() override;
	virtual void onLevelRender() override;
};
