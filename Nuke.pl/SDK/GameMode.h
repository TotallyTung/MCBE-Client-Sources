#pragma once

#include <stdint.h>

#include "../Utils/HMath.h"
#include "ClientInstance.h"
#include "Entity.h"

class GameMode {
private:
	virtual __int64 destructorGameMode();
	// Duplicate destructor
public:
	virtual __int64 startDestroyBlock(Vec3i const &pos, unsigned char blockSide, bool &isDestroyedOut);
	virtual __int64 destroyBlock(Vec3i *, unsigned char);
	virtual __int64 continueDestroyBlock(Vec3i*, unsigned char blockSide, Vec3*, bool*);
	virtual __int64 stopDestroyBlock(Vec3i const &);
	virtual __int64 startBuildBlock(Vec3i const &, unsigned char);
	virtual __int64 buildBlock(Vec3i *, unsigned char, bool);
	virtual __int64 continueBuildBlock(Vec3i const &, unsigned char);
	virtual __int64 stopBuildBlock(void);
	virtual __int64 tick(void);

public:
	virtual __int64 getPickRange(__int64 const &, bool);
	virtual __int64 useItem(ItemStack &);
	virtual __int64 useItemOn(__int64 &, Vec3i const &, unsigned char, Vec3 const &, __int64 const *);
	virtual __int64 interact(Entity &, Vec3 const &);

public:
	virtual __int64 attack(Entity *);

public:
	virtual __int64 releaseUsingItem(void);

public:
	virtual void setTrialMode(bool);
	virtual bool isInTrialMode(void);

private:
	virtual __int64 registerUpsellScreenCallback(__int64);

public:
	Player *player;
	char pad_0008[8];       // 0x0010
	char pad_0010[8];       // 0x0018
	char pad_0018[4];       // 0x0020
	float destroyProgress;  // 0x0024
	bool tryPlaceBlock(const Vec3i& BlockPos);

public:
	float getDestroyRate(Block* block) {
		using getDestroyRateFunc = float(__fastcall *)(GameMode*, Block*);
		static getDestroyRateFunc Func = (getDestroyRateFunc)(Utils::getBase() + 0x1DFA970);
		return Func(this, block);	
	}
	void destroyBlockBypass(Vec3i* pos, unsigned char face) {
		int prev = player->gamemode;

		player->gamemode = 1;
		destroyBlock(pos, face);
		player->gamemode = prev;
	}
	void startDestroyBlockBypass(Vec3i const& pos, unsigned char blockSide, bool& isDestroyedOut) {
		int prev = player->gamemode;

		player->gamemode = 1;
		startDestroyBlock(pos, blockSide, isDestroyedOut);
		player->gamemode = prev;
	}
};