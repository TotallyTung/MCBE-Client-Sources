#pragma once
#include "../Module.h"
#include "../../ModuleManager.h"

class PacketMine : public IModule {
public:
	const char* name = "PacketMine";
	vec3_ti breakPos = vec3_ti(0, 0, 0);
	float progress = 0.f;
	bool silent = false;
	bool growRender = true;
	int face = 0;
	float increase = 0.f;
	bool increaseOnStart = false;
	bool renderProgress = true;
	SettingEnum render = this;
	SettingEnum anim = this;
	SettingEnum swap = this;
	SettingEnum server = this;
	SettingEnum mineType = this;
	bool hasSwapped = false;
	bool obisdianOnly = false;
	int silentMine = 0;
	int origSlot;
	int swapDel = 10;
	int sDelay = 0;
	float opacity = 0.2f;
	void findAPickaxe();
	void returnSlot();
	PacketMine();
	virtual const char* getRawModuleName() override;
	virtual const char* getModName();
	virtual void onTick(C_GameMode* gm) override;
	virtual void onWorldTick(C_GameMode* gm) override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx);
};
