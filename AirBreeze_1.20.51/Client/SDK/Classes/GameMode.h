#pragma once
#include <uchar.h>
#include "Actor.h"


class GameMode {

public:
	virtual ~GameMode();
	virtual void startDestroyBlock(Vec3<int> const&, unsigned char blockSide, bool&);
	virtual void destroyBlock(Vec3<int> const&, unsigned char blockSide);
	virtual void continueDestroyBlock(Vec3<int> const&, unsigned char blockSide, Vec3<float> const&, bool&);
	virtual void stopDestroyBlock(Vec3<int> const&);
	virtual void startBuildBlock(Vec3<int> const&, unsigned char blockSide);
	virtual void buildBlock(Vec3<int> const&, unsigned char blockSide, bool);
	virtual void continueBuildBlock(Vec3<int> const&, unsigned char blockSide);
	virtual void stopBuildBlock(void);
	virtual void tick(void);
	virtual int getPickRange(int const&, bool);
	virtual void useItem(__int64&); //ItemStack
	virtual void useItemOn(__int64&, Vec3<int> const&, unsigned char blockSide, Vec3<float> const&, Block const*);
	virtual void interact(Actor&, Vec3<float> const&);
	virtual void attack(Actor&);
	virtual void releaseUsingItem(void);
	virtual void setTrialMode(bool);
	virtual bool isInTrialMode(void);
	virtual void registerUpsellScreenCallback(std::function<void(bool)>);


private:
	uintptr_t** VTable;
public:
	/*auto getLocalPlayer(void) -> class Player* {
		using GetLocalPlayer = Player * (__thiscall*)(ClientInstance*);
		GetLocalPlayer _GetLocalPlayer = (GetLocalPlayer)(this->VTable[25]);
		return _GetLocalPlayer(this);
	}*/
	class Player* player; // 0x0008



};