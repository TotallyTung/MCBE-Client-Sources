#pragma once
#include "Module.h"
#include "../ModuleManager.h"

class BlockEsp : public IModule {
private:
	float thick = 0.4f;
	bool outline = false;
	bool diamond = false;
	bool emerald = false;
	bool gold = false;
	bool redstone = false;
	bool lapis = false;
	bool iron = false;
	bool coal = false;
	bool debris = false;
	bool quartz = false; 
	bool invisBlocks = false;
	bool portalBlocks = false;

	bool regChests = true;
	bool trapChests = true;
	bool shulkers = true;
	bool hoppers = true;
	bool endChests = true;

	int range = 10;
	int Odelay = 0;
	float delay = 0.14f;


	int tickTimeout = 0;
	std::vector<AABB> bufferedChestList;
	std::mutex listLock;

	bool allOfThem = true;

public:
	BlockEsp();
	~BlockEsp();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;

	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onTick(C_GameMode* gm) override;
};
