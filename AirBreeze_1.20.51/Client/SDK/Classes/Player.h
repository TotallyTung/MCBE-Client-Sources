#pragma once

#include "Actor.h"
#include "GameMode.h"

class Player : public Actor {
public:
	BUILD_ACCESS(GameMode*, gamemode, 0x0E70);
public: //Player
	virtual void prepareRegion(__int64&);
	virtual void destroyRegion(void);
	virtual void suspendRegion(void);
	virtual void resendAllChunks(void);
	virtual void _fireWillChangeDimension(void);
	virtual void _fireDimensionChanged(void);
	virtual void changeDimensionWithCredits();
	virtual void tickWorld(__int64 const&);
private:
	virtual void Function8();
public:
	virtual int getTickingOffsets(void);
	virtual void moveView(void);
	virtual void moveSpawnView(Vec3<float> const&, __int64);
	virtual void setName(std::string const&);
	virtual int getTravelledMethod(void);
	virtual void checkMovementStats(Vec3<float> const&);
	virtual int getCurrentStructureFeature(void);
	virtual bool isAutoJumpEnabled(void);
	virtual void respawn(void);
	virtual void resetRot(void);
	virtual bool isInTrialMode(void);
	virtual void hasResource(int);
	virtual void completeUsingItem(void);
	virtual void startDestroying(void);
	virtual void stopDestroying(void);
	virtual void openPortfolio(void);
	virtual void openBook(int, bool, int, __int64*); //BlockActor
	virtual void openTrading(bool);
	virtual void canOpenContainerScreen(void);
	virtual void openChalkboard();
	virtual void openNpcInteractScreen();
	virtual void openInventory(void);
	virtual void displayChatMessage(std::string const&, std::string const&);
	virtual void displayClientMessage(TextHolder* const&);
	virtual void displayTextObjectMessage();
	virtual void displayTextObjectWhisperMessage();
	virtual void displayTextObjectWhisperMessage2();
	virtual void displayWhisperMessage(std::string const&, std::string const&, std::string const&, std::string const&);
	virtual void startSleepInBed(Vec3<int> const&);
	virtual void stopSleepInBed(bool, bool);
	virtual void canStartSleepInBed(void);
	virtual int getSleepTimer(void);
	virtual int getPreviousTickSleepTimer(void);
	virtual void openSign(Vec3<int> const&, bool);
	virtual void playEmote(std::string const&, bool);
	virtual bool isHostingPlayer(void);
	virtual bool isLoading(void);
	virtual bool isPlayerInitialized(void);
	virtual void stopLoading(void);
	virtual void registerTrackedBoss(unsigned int64_t);
	virtual void unRegisterTrackedBoss(unsigned int64_t);
	virtual void setPlayerGameType(__int64);
	virtual void initHUDContainerManager(void);
	virtual void _crit(Actor&);
	virtual int getEventing(void);
	virtual int getUserId(void);
	virtual void sendEventPacket();
	virtual void addExperience(int);
	virtual void addLevels(int);
private:
	virtual void Function58();
	virtual void Function59();
public:
	virtual void inventoryChanged();
private:
	virtual void Function61();
public:
	virtual void deleteContainerManager(void);
	virtual bool isActorRelevant(Actor const&);
private:
	virtual void Function64();
public:
	virtual void onSuspension(void);
	virtual void onLinkedSlotsChanged(void);
	virtual void startCooldown(/*Item const*, bool*/);
	virtual int getItemCooldownLeft(/*HashedString const&*/);
	virtual int getItemCooldownLeft2(/*ulong*/);
	virtual int getMaxItemCooldownLeft(void);
	virtual bool isItemOnCooldown(/*HashedString const&*/);
private:
	virtual void Function72();
	virtual void Function73();
public:
	virtual void sendNetworkPacket(Packet&);
private:
	virtual void Function75();
public:
	virtual void reportMovementTelemetry(__int64);
	virtual bool isSimulated(void);
	virtual int getXuid(void);
	virtual int getMovementSettings(void);
	virtual void addSavedChunk(__int64 const&);
	virtual int getMaxChunkBuildRadius(void);
	virtual void onMovePlayerPacketNormal(Vec3<float> const&, Vec2<float> const&, float);
	virtual void _createChunkSource(__int64&);
	virtual void setAbilities(__int64 const&);
private:
	virtual void Function85();
	virtual void Function86();
};