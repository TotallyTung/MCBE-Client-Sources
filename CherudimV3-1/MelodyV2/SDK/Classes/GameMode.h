#pragma once
#include "../../Utils/MemoryUtils.h"
#include "../../Utils/Math.h"

class Actor;
class Player;
class Dimenson;
class ItemStack;
class Block;

class GameMode {
public:
	Player* player; // 0x8
	BUILD_ACCESS(this, Vec3<int>, breakPos, 0x10);
	BUILD_ACCESS(this, float, destroyProgress, 0x24);
public:
	float getDestroyRate(const Block* block) {
		using func_t = float(__thiscall*)(GameMode*, const Block*);
		static func_t func = (func_t)findSig(Sigs::gamemode::getDestroyRate);
		return func(this, block);
	}
	void baseUseItem(ItemStack* itemStack) {
		using func_t = __int64(__thiscall*)(GameMode*, ItemStack*);
		static func_t func = (func_t)findSig(Sigs::gamemode::baseUseItem);
		func(this, itemStack);
	}
	bool tryPlaceBlock(const Vec3<int>& blockPos);
public:
	virtual ~GameMode();
	virtual void startDestroyBlock(Vec3<int> const&, char, bool&);
	virtual void destroyBlock(Vec3<int> const&, char);
	virtual void continueDestroyBlock(Vec3<int> const&, char, Vec3<float> const&, bool&);
	virtual void stopDestroyBlock(Vec3<int> const&);
	virtual void startBuildBlock(Vec3<int> const&, char);
	virtual void buildBlock(Vec3<int> const&, char, bool);
	virtual void continueBuildBlock(Vec3<int> const&, char);
	virtual void stopBuildBlock(void);
	virtual void tick(void);
	virtual __int64 getPickRange(__int64 const&, bool);
	virtual void useItem(ItemStack&);
	virtual void useItemOn(ItemStack*, Vec3<int> const&, unsigned char, Vec3<float> const&, Block const*);
	virtual void interact(Actor&, Vec3<float> const&);
	virtual __int64 attack(Actor*);
	virtual void releaseUsingItem(void);
	virtual void setTrialMode(bool);
	virtual bool isInTrialMode(void);
};
