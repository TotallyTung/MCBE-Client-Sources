#pragma once
#include "Mob.h"
#include "GameMode.h"
#include "PlayerInventory.h"
#include "InventoryTransactionManager.h"
#include "IContainerManager.h"
#include <string>
#include <memory>
class ChunkSource;
class Tick;
class ChalkboardBlockActor;
class INpcDialogueData;
class TextObjectRoot;
class ResolvedTextObject;
enum GameType;
class Packet;
enum MovementEventType;
enum LayeredAbilities;

class Player : public Mob {
public:
	BUILD_ACCESS(this, std::weak_ptr<IContainerManager>, mContainerManager, 0x7B0);
public:
	float getDestroyProgress(const Block* block) {
		using func_t = float(__thiscall*)(Player*, const Block*);
		static func_t func = (func_t)findSig(Sigs::player::getDestroyProgress);
		return func(this, block);
	}
	GameMode* getGameMode() {
		return (GameMode*)*((uintptr_t*)this + 0x1E2);
	}
	PlayerInventory* getPlayerInventory() {
		return (PlayerInventory*)*((uintptr_t*)this + 0xFD);
	}
	InventoryTransactionManager* getInventoryTransactionManager() {
		return (InventoryTransactionManager*)*((uintptr_t*)this + 0xEE0);
	}

public:
	virtual void prepareRegion(ChunkSource&);
	virtual void destroyRegion(void);
	virtual void suspendRegion(void);
	virtual void _fireDimensionChanged(void);
	virtual void changeDimensionWithCredits(__int64);
	virtual void tickWorld(Tick const&);
	virtual void Function5();
	virtual void getTickingOffsets(void);
	virtual void moveView(void);
	virtual void moveSpawnView(Vec3<float> const&, __int64);
	virtual void checkMovementStats(Vec3<float> const&);
	virtual void getCurrentStructureFeature(void);
	virtual bool isAutoJumpEnabled(void);
	virtual void respawn(void);
	virtual void resetRot(void);
	virtual bool isInTrialMode(void);
	virtual void openPortfolio(void);
	virtual void openBook(int, bool, int, BlockActor*);
	virtual void openTrading(ActorUniqueID const&, bool);
	virtual void openChalkboard(ChalkboardBlockActor&, bool);
	virtual void openNpcInteractScreen(std::shared_ptr<INpcDialogueData>);
	virtual void openInventory(void);
	virtual void displayChatMessage(std::string const&, std::string const&);
	virtual void displayClientMessage(std::string const&);
	virtual void displayTextObjectMessage(TextObjectRoot const&, std::string const&, std::string const&);
	virtual void displayTextObjectWhisperMessage(ResolvedTextObject const&, std::string const&, std::string const&);
	virtual void displayTextObjectWhisperMessage(std::string const&, std::string const&, std::string const&);
	virtual void displayWhisperMessage(std::string const&, std::string const&, std::string const&, std::string const&);
	virtual void startSleepInBed(Vec3<int> const&);
	virtual void stopSleepInBed(bool, bool);
	virtual bool canStartSleepInBed(void);
	virtual void openSign(Vec3<int> const&, bool);
	virtual void playEmote(std::string const&, bool);
	virtual bool isHostingPlayer(void);
	virtual bool isLoading(void);
	virtual bool isPlayerInitialized(void);
	virtual void stopLoading(void);
	virtual void setPlayerGameType(GameType);
	virtual void initHUDContainerManager(void);
	virtual void _crit(Actor*);
	virtual void getEventing(void);
	virtual void getUserId(void);
	virtual void addExperience(int);
	virtual void addLevels(int);
	virtual void Function43();
	virtual void Function44();
	virtual void Function45();
	virtual void deleteContainerManager(void);
	virtual bool isActorRelevant(Actor const&);
	virtual void Function48();
	virtual void onSuspension(void);
	virtual void onLinkedSlotsChanged(void);
	virtual void Function51();
	virtual void Function52();
	virtual void sendNetworkPacket(Packet&);
	virtual void Function54();
	virtual void reportMovementTelemetry(MovementEventType);
	virtual bool isSimulated(void);
	virtual void getXuid(void);
	virtual void getMovementSettings(void);
	virtual void getMaxChunkBuildRadius(void);
	virtual void onMovePlayerPacketNormal(Vec3<float> const&, Vec2<float> const&, float);
	virtual void _createChunkSource(ChunkSource&);
	virtual void setAbilities(LayeredAbilities const&);
	virtual void Function63();
	virtual void Function64();
};