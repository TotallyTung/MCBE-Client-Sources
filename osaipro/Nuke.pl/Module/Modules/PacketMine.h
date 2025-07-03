#pragma once
#include "Module.h"

class PacketMine : public IModule {
public:
	float range = 7.f;
	SettingEnum ViewMode = SettingEnum(this);
	float MineSpeed = 1.f;
	float ResetValue = 0.f;
	bool silentSwitch = false;
	bool silentBack = true;
	bool multiTask = false;
//	bool strict = true;
	bool GrowRender = true;

private:
	unsigned char face = 0;
	int lastHotbar = -1;
	float animDuration = 0.f;
	float lastDestroyRate = 0.f;
	

public:
	bool FastCancel = true;
	Vec3i blockPos = Vec3i(0, 0, 0);
	PacketMine();
	bool DoWeHavePickaxe(LocalPlayer* lp);
	bool isHoldingPickaxe(LocalPlayer* lp);
	std::pair<int, float> getBestPickaxeSlot(LocalPlayer* lp, GameMode* gm, Block* block);
	//float getMaxDestroyRate(GameMode* gm, int slot);
	void Reset(GameMode* gm = nullptr);
	void setBreakPos(GameMode* gm, Vec3i* blockPos, unsigned char face);
	void updateBreak(LocalPlayer* lp, GameMode* gm);

	virtual const char* getModuleName() override;
	virtual void onDisable() override;
	virtual void onLevelRender() override;
	virtual void onPreRender(MinecraftUIRenderContext* renderCtx) override;
};
