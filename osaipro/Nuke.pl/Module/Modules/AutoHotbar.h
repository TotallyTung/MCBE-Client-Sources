#pragma once
#include "../ModuleManager.h"
#include "Module.h"

class AutoHotbar : public IModule {
private:
	bool Swordhb = false;
	bool Pickaxehb = false;
	bool potionshb = false;
	bool obsidianhb = false;
	bool anvilhb = false;
	bool crystalhb = false;
	bool gapplehb = false;
	bool bedhb = false;
	int SwordSlot = 1;
	int PickaxeSlot = 2;
	int potionSlot = 5;
	int obsidianSlot = 8;
	int anvilSlot = 6;
	int crystalSlot = 4;
	int gappleSlot = 3;
	int bedSlot = 9;
	bool dasword = false;
	bool dapickaxe = false;
	bool daob = false;
	bool daanvil = false;
	bool dacry = false;
	bool dapot = false;
	bool dagapple = false;
	bool dabed = false;
	bool Nodelay = false;

public:
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onJoiningServer() override;
	AutoHotbar();
};
