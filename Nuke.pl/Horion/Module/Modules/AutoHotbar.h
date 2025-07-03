#pragma once
#include "../ModuleManager.h"
#include "Module.h"

class AutoHotbar : public IModule {
private:
    bool Swordhb = false;
    bool Pickaxehb = false;
    bool obsidianhb = false;
	bool anvilhb = false;
	bool Sandhb = false;
	bool Gravelhb = true;
	bool concPowhb = false;
	bool crystalhb = false;
	bool gapplehb = false;
	int SwordSlot = 1;
	int PickaxeSlot = 2;
	int obsidianSlot = 3;
	int anvilSlot = 8;
	int SandSlot = 8;
	int GravelSlot = 8;
	int concPowSlot = 8;
	int crystalSlot = 5;
	int gappleSlot = 9;
	bool dasword = false;
	bool dapickaxe = false;
	bool daob = false;
	bool daanvil = false;
	bool daSand = false;
	bool daGravel = false;
	bool daconcPow = false;
	bool dacry = false;
	bool dagapple = false;
	bool Nodelay = false;
public:
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	AutoHotbar();
};
