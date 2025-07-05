//
// Created by vastrakai on 10/10/2023.
//

#pragma once


#include "Actor.h"
#include <Prax/SDK/Game/Network/Network.h>
#include <src/Prax/SDK/Game/Minecraft.h>

enum RaycastTypes : uint32_t
{
    BLOCK = 0,
    ENTITY = 1,
    NOTHING = 3
};

class mceUUID {
public:
    uint64_t mostSignificant;
    uint64_t leastSignificant;

    bool operator==(const mceUUID& other) const {
        return mostSignificant == other.mostSignificant && leastSignificant == other.leastSignificant;
    }

    std::string to_string() const {
        char buf[64];
        sprintf(buf, "%016llX%016llX", mostSignificant, leastSignificant);
        return std::string(buf);
    }
};




struct WeakEntityRef {
private:
    uint8_t pad[0x10]{};

public:
    EntityId id;

private:
    uint8_t pad2[0x4]{};
};

class HitResult
{
public:
    /*
    Vector3 startPos;
    Vector3 relativeEndPos;
    RaycastTypes hitType; // 0 is block, 1 is actor, 3 is air
    uint32_t blockFace;
    BlockPos blockPos;
    Vector3 blockHitPos; // If no block is found it just extends to the max ray position
    uintptr_t someActorHitPointer;
    uintptr_t someOtherActorHitPointer;
    uint32_t actorRuntimeId;
    bool isHittingLiquid;
    uint8_t liquidBlockFace; // If the liquid were a solid block, what block face would it be
    BlockPos liquidBlockPos;
    Vector3 liquidBlockHitPos; // Full of zeroes if there is no liquid block
    bool isExtendedBlockHit; // Is the bedrock bridge mechanic thingy focusing a block
*/
    Vector3       startPos;     // this+0x0
    Vector3       rayDir;       // this+0xC
    RaycastTypes  type;         // this+0x18
    unsigned char facing;       // this+0x1C
    BlockPos      blockPos;     // this+0x20
    Vector3       Pos;          // this+0x2C
    WeakEntityRef entity;       // this+0x38
    bool          isHitLiquid;  // this+0x50
    unsigned char liquidFacing; // this+0x51
    BlockPos      liquid;       // this+0x54
    Vector3       liquidPos;    // this+0x60
    bool          indirectHit;  // this+0x6C
};

enum class BuildPlatform : int {
    Invalid = -1,  // if we have to switch to decimal ig we can
    Android = 0x1,
    iOS = 0x2,
    OSX = 0x3,
    Amazon = 0x4,
    GearVR = 0x5,
    WIN10 = 0x7,
    Win32 = 0x8,
    Dedicated = 0x9,
    PS4 = 0xB,
    Nx = 0xC,
    Xbox = 0xD,
    WindowsPhone = 0xE,
    Linux = 0xF,
    Unknown = 0xFF,
};

const std::map<BuildPlatform, std::string> BuildPlatformNames = {
        {BuildPlatform::Invalid, "Unknown"},
        {BuildPlatform::Android, "Android"},
        {BuildPlatform::iOS, "iOS"},
        {BuildPlatform::OSX, "macOS"},
        {BuildPlatform::Amazon, "Amazon"},
        {BuildPlatform::GearVR, "GearVR"},
        {BuildPlatform::WIN10, "Windows"},
        {BuildPlatform::Win32, "Win32"},
        {BuildPlatform::Dedicated, "Dedicated"},
        {BuildPlatform::PS4, "Playstation"},
        {BuildPlatform::Nx, "Switch"},
        {BuildPlatform::Xbox, "Xbox"},
        {BuildPlatform::WindowsPhone, "WindowsPhone"},
        {BuildPlatform::Linux, "Linux"},
        {BuildPlatform::Unknown, "Unknown"}
};

class PlayerListEntry {
public:
    uint64_t id; // This is the ActorUniqueID
    mceUUID uuid;
    std::string name, XUID, platformOnlineId;
    BuildPlatform buildPlatform;

    PlayerListEntry() {
        id = 0;
        uuid = mceUUID();
        name = "";
        XUID = "";
        platformOnlineId = "";
        buildPlatform = BuildPlatform::Unknown;
    }
};

class LevelData {
public:
    BUILD_ACCESS(this, uint64_t, levelTick, 0x468);
};


class Level {
private:
    char pad_0x0[0x1BC8 - 0x8];  // 0x0
public:
    std::unordered_map<mceUUID, PlayerListEntry> playerList; // 0x1EA8

// new vtable
        virtual ~Level();
    virtual void initialize();
    virtual void startLeaveGame(void);
    virtual bool isLeaveGameDone(void);
    virtual int getOrCreateDimension();
    virtual int getDimension();
    virtual int getLastOrDefaultSpawnDimensionId();
    virtual void forEachDimension();
    virtual void forEachDimension2();
    virtual int getDimensionManager(void);
    virtual int getDimensionManager2(void);
    virtual int getChunkTickRange(void);
    virtual int getChunkTickRangeManager(void);
    virtual int getPortalForcer(void);
    virtual void requestPlayerChangeDimension();
    virtual void entityChangeDimension();
    virtual int getSpawner(void);
    virtual int getBossEventSubscriptionManager(void);
    virtual int getProjectileFactory(void);
    virtual int getEntityDefinitions(void);
    virtual int getActorAnimationGroup(void);
    virtual int getActorAnimationControllerGroup(void);
    virtual int getBlockDefinitions(void);
    virtual int getBlockComponentFactory(void);
    virtual int getBlockComponentFactory2(void);
    virtual int getActorPropertyGroup(void);
    virtual int getCameraPresets(void);
    virtual int getCameraPresets2(void);
    virtual int getDisablePlayerInteractions(void);
    virtual void setDisablePlayerInteractions(bool);
    virtual int getAutomationBehaviorTreeGroup(void);
    virtual int getBehaviorFactory(void);
    virtual int getDifficulty(void);
    virtual int getInternalComponentRegistry(void);
    virtual int getDimensionConversionData(void);
    virtual int getSpecialMultiplier();
    virtual void hasCommandsEnabled(void);
    virtual void useMsaGamertagsOnly(void);
    virtual void setMsaGamertagsOnly(bool);
    virtual void addEntity(BlockSource&);
    virtual void addGlobalEntity(BlockSource&);
    virtual void addAutonomousEntity(BlockSource&);
    virtual void addUser();
    virtual void addDisplayEntity(BlockSource&);
    virtual void removeDisplayEntity(unsigned __int64 WeakEntityRef);
    virtual void suspendPlayer(Actor&);
    virtual void resumePlayer(Actor&);
    virtual bool isPlayerSuspended(Actor&);
    virtual void removeActorAndTakeEntity(unsigned __int64 WeakEntityRef);
    virtual void removeActorFromWorldAndTakeEntity(unsigned __int64 WeakEntityRef);
    virtual void takeEntity(unsigned __int64 WeakEntityRef, unsigned __int64 LevelChunk);
    virtual void fetchStrictEntity(unsigned int64_t,bool);
    virtual void fetchEntity(uint64_t, bool);
    virtual int getRuntimeEntity(unsigned __int64 ActorRuntimeID, bool);
    virtual int getMob(uint64_t);
    virtual int getPlayer(std::string const&);
    virtual int getPlayer(uint64_t);
    virtual int getPlayerByXuid(std::string const&);
    virtual int getPlayer2(uint64_t);
    virtual int getPlatformPlayer(std::string const&);
    virtual int getPlayerFromServerId(std::string const&);
    virtual int getRuntimePlayer(unsigned __int64 ActorRuntimeID);
    virtual int getNumRemotePlayers(void);
    virtual int getPrimaryLocalPlayer(void);
    virtual int getEventing(void);
    virtual int getPlayerColor(Actor const&);
    virtual int getCurrentTick(void);
    virtual int getCurrentServerTick(void);
    virtual int getBiomeRegistry(void);
    virtual int getBiomeRegistry2(void);
    virtual class BlockPalette* getBlockPalette(void);
    virtual int getBlockPalette2(void);
    virtual int getFeatureRegistry(void);
    virtual int getFeatureRegistry2(void);
    virtual int getFeatureTypeFactory(void);
    virtual int getFeatureTypeFactory2(void);
    virtual int getJigsawStructureRegistry(void);
    virtual int getJigsawStructureRegistry2(void);
    virtual int getStructureManager(void);
    virtual int getStructureManager2(void);
    virtual int getBiomeComponentFactory(void);
    virtual int getBiomeComponentFactory2(void);
    virtual int getSurfaceBuilderRegistry(void);
    virtual int getSurfaceBuilderRegistry2(void);
    virtual int getBiomeManager(void);
    virtual int getBiomeManager2(void);
    virtual int getDimensionFactory(void);
    virtual int getDimensionFactory2(void);
    virtual int getLightTextureImageBuilderFactory(void);
    virtual int getLightTextureImageBuilderFactory2(void);
    virtual int getWorldRegistriesProvider(void);
    virtual int getWorldRegistriesProvider2(void);
    virtual void addListener(class LevelListener &);
    virtual void removeListener(class LevelListener &);
    virtual void tickEntities(void);
    virtual void tickEntitySystems(void);
    virtual int getPauseManager(void);
    virtual int getPauseManager2(void);
    virtual void onPlayerDeath(Actor&, unsigned __int64 ActorDamageSource);
    virtual void tick(void);
    virtual void explode(BlockSource &,Actor *,Vector3 const&,float,bool,bool,float,bool);
    virtual void explode(class Explosion &);
    virtual void spawnParticleEffect(std::string const&,Vector3 const&,class Dimension *);
    virtual void denyEffect(BlockSource &,Vector3 const&);
    virtual void potionSplash(Vector3 const&,class rgb const&,bool);
    virtual void extinguishFire(BlockSource &,BlockPos const&,unsigned char,Actor *);
    virtual void findPath(Actor &,int,int,int,class NavigationComponent &);
    virtual void findPath(Actor &,Actor &,class NavigationComponent &);
    virtual void updateSleepingPlayerList(void);
    virtual void setSleepStatus(int PlayerSleepStatus);
    virtual int getSleepStatus(void);
    virtual int getTime(void);
    virtual void setTime(int);
    virtual int getSeed(void);
    virtual int getSharedSpawnPos(void);
    virtual void setDefaultSpawn(BlockPos const&);
    virtual int getDefaultSpawn2(void);
    virtual void setDefaultGameType(int GameType);
    virtual int getDefaultGameType(void);
    virtual void setDifficulty(int Difficulty);
    virtual void setMultiplayerGameIntent(bool);
    virtual int getMultiplayerGameIntent(void);
    virtual void setMultiplayerGame(bool);
    virtual bool isMultiplayerGame(void);
    virtual void setLANBroadcastIntent(bool);
    virtual int getLANBroadcastIntent(void);
    virtual void setLANBroadcast(bool);
    virtual int getLANBroadcast(void);
    virtual void setXBLBroadcastIntent();
    virtual int getXBLBroadcastIntent(void);
    virtual void hasXBLBroadcastIntent(void);
    virtual void setXBLBroadcastMode();
    virtual int getXBLBroadcastMode(void);
    virtual void hasXBLBroadcast(void);
    virtual void setPlatformBroadcastIntent();
    virtual int getPlatformBroadcastIntent(void);
    virtual void hasPlatformBroadcastIntent(void);
    virtual void setPlatformBroadcastMode();
    virtual int getPlatformBroadcastMode(void);
    virtual void hasPlatformBroadcast(void);
    virtual void setHasLockedBehaviorPack(bool);
    virtual void setHasLockedResourcePack(bool);
    virtual void setCommandsEnabled(bool);
    virtual void setWorldTemplateOptionsUnlocked(void);
    virtual void hasLevelStorage(void);
    virtual int getLevelStorage(void);
    virtual int getLevelStorage2(void);
    virtual LevelData* getLevelData(void);
    virtual int getLevelData2(void);
    virtual int getPhotoStorage(void);
    virtual void createPhotoStorage(void);
    virtual void setEducationLevelSettings(int EducationLevelSettings);
    virtual int getEducationLevelSettings(void);
    virtual void save(void);
    virtual void saveLevelData(void);
    virtual void saveGameData(void);
    virtual void requestTimedStorageDeferment(void);
    virtual int getTickingAreasMgr(void);
    virtual void addTickingAreaList(/*AutomaticID<Dimension,int>,std::shared_ptr<TickingAreaList> const&*/);
    virtual void sendServerLegacyParticle(class ParticleType,Vector3 const&,Vector3 const&,int);
    virtual void playSound(uint64_t LevelSoundEvent,Vector3 const&,int,class ActorDefinitionIdentifier const&,bool,bool);
    virtual void playSoun2(uint64_t LevelSoundEvent,Vector3 const&,float,float);
    virtual void playSound(std::string const&,Vector3 const&,float,float);
    virtual void playSound(class IConstBlockSource const&,uint64_t LevelSoundEvent,Vector3 const&,int, class ActorDefinitionIdentifier const&,bool,bool);
    virtual void playSound(/*AutomaticID<Dimension,int>,LevelSoundEvent,Vector3 const&,int,ActorDefinitionIdentifier const&,bool,bool*/);
    virtual int getRemotePlayerEventCoordinator(void);
    virtual int getServerPlayerEventCoordinator(void);
    virtual int getClientPlayerEventCoordinator(void);
    virtual int getActorEventCoordinator(void);
    virtual int getBlockEventCoordinator(void);
    virtual int getItemEventCoordinator(void);
    virtual int getServerNetworkEventCoordinator(void);
    virtual int getScriptingEventCoordinator(void);
    virtual int getScriptDeferredEventCoordinator(void);
    virtual int getLevelEventCoordinator(void);
    virtual void handleLevelEvent(unsigned __int64 LevelEvent, unsigned __int64 CompoundTag);
    virtual void handleLevelEvent(unsigned __int64 LevelEvent, struct Vec3 const&, int);
    virtual void handleStopSoundEvent(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&);
    virtual void handleStopAllSounds(void);
    virtual void broadcastLevelEvent(unsigned __int64 LevelEvent, unsigned __int64 CompoundTag, unsigned __int64 UserEntityIdentifierComponent);
    virtual void broadcastLevelEvent(unsigned __int64 LevelEvent, struct Vec3 const&, int, unsigned __int64 UserEntityIdentifierComponent);
    virtual void broadcastLocalEvent(BlockSource&, unsigned __int64 LevelEvent, struct Vec3 const&, Block const&);
    virtual void broadcastLocalEvent(BlockSource&, unsigned __int64 LevelEvent, struct Vec3 const&, int);
    virtual void broadcastSoundEvent(class Dimension&, unsigned __int64 LevelSoundEvent, struct Vec3 const&, int, unsigned __int64 ActorDefinitionIdentifier, bool, bool);
    virtual void broadcastSoundEvent(BlockSource&, unsigned __int64 LevelSoundEvent, struct Vec3 const&, int, unsigned __int64 ActorDefinitionIdentifier, bool);
    virtual void broadcastSoundEvent(BlockSource&, unsigned __int64 LevelSoundEvent, struct Vec3 const&, Block const&, unsigned __int64 ActorDefinitionIdentifier, bool, bool);
    virtual void broadcastActorEvent(Actor&, unsigned __int64 ActorEvent, int);
    virtual void addChunkViewTracker();
    virtual void onChunkReload(unsigned __int64 Bounds);
    virtual void onChunkReloaded(unsigned __int64 ChunkSource, unsigned __int64 LevelChunk);
    virtual int getActivePlayerCount(void);
    virtual int getActiveUsersCount(void);
    virtual void forEachPlayer(std::function<bool(Actor const&)>);
    virtual void forEachPlayer2(std::function<bool(Actor&)>);
    virtual void forEachUser(std::function<bool(unsigned __int64 EntityContext)>);
    virtual void forEachUser2(std::function<bool(unsigned __int64 EntityContext)>);
    virtual void findPlayer(std::function<bool(unsigned __int64 WeakEntityRef)>);
    virtual void findPlayer2(std::function<bool(Actor const&)>);
    virtual int getUserCount(void);
    virtual void countUsersWithMatchingNetworkId(class NetworkIdentifier const&);
    virtual int getUsers(void);
    virtual int getEntities(void);
    virtual void onSubChunkLoaded(class ChunkSource &,class LevelChunk &,short,bool);
    virtual void onChunkLoaded(unsigned __int64 ChunkSource,unsigned __int64 LevelChunk);
    virtual void onChunkDiscarded(unsigned __int64 LevelChunk);
    virtual int getLevelChunkMetaDataManager(void);
    virtual void queueEntityDestruction(unsigned __int64 OwnerPtrTEntityRefTraits);
    virtual void removeEntity(unsigned __int64 WeakEntityRef);
    virtual void removeEntity(Actor&);
    virtual void forceRemoveEntity(Actor&);
    virtual void forceRemoveEntityfromWorld(Actor&);
    virtual void forceFlushRemovedPlayers(void);
    virtual void loadFunctionManager(void);
    virtual void levelCleanupQueueEntityRemoval(unsigned __int64 OwnerPtrTEntityRefTraits);
    virtual void registerTemporaryPointer(unsigned __int64 _TickPtr);
    virtual void unregisterTemporaryPointer(unsigned __int64 _TickPtr);
    virtual void destroyBlock(BlockSource&, BlockPos const&, bool);
    virtual void upgradeStorageVersion(unsigned __int64 StorageVersion);
    virtual void suspendAndSave(void);
    virtual void addParticle(int ParticleType, struct Vec3 const&, struct Vec3 const&, int, unsigned __int64 CompoundTag, bool);
    virtual void _destroyEffect(BlockPos const&, Block const&, int);
    virtual void addParticleEffect(unsigned __int64 HashedString, struct Vec3 const&, unsigned __int64 MolangVariableMap);
    virtual void addTerrainParticleEffect(BlockPos const&, Block const&, struct Vec3 const&, float, float, float);
    virtual void addTerrainSlideEffect(BlockPos const&, Block const&, struct Vec3 const&, float, float, float);
    virtual void addBreakingItemParticleEffect(struct Vec3 const&, int ParticleType, unsigned __int64 ResolvedItemIconInfo);
    virtual int getNewUniqueID(void);
    virtual int getNextRuntimeID(void);
    virtual int getTickingOffsets(void);
    virtual int getClientTickingOffsets(void);
    virtual int getSortedPositionsFromClientOffsets(/*std::vector<ChunkPos> const&*/);
    virtual bool isExporting(void);
    virtual void setIsExporting(bool);
    virtual int getSavedData(void);
    virtual int getMapSavedData(uint64_t);
    virtual int getMapSavedData(CompoundTag const&);
    virtual int getMapSavedData(CompoundTag const*);
    virtual void requestMapInfo(uint64_t,bool);
    virtual void expandMapByID(uint64_t,bool);
    virtual void copyAndLockMap(uint64_t,uint64_t);
    virtual void createMapSavedData(/*uint64_t const&,BlockPos const&,AutomaticID<Dimension,int>,int*/);
    virtual void createMapSavedData2(/*std::vector<uint64_t> const&,BlockPos const&,AutomaticID<Dimension,int>,int*/);
    virtual int getScreenshotsFolder(void);
    virtual int getLevelId(void);
    virtual void setLevelId(std::string);
    virtual int getSyncTasksGroup(void);
    virtual int getIOTasksGroup(void);
    virtual int getClientResourcePackManager(void);
    virtual int getServerResourcePackManager(void);
    virtual int getTradeTables(void);
    virtual void decrementTagCache(/*std::string const&,TagRegistry<IDType<class LevelTagIDType>,IDType<LevelTagSetIDType>> &*/);
    virtual void incrementTagCache(/*std::string const&,TagRegistry<IDType<class LevelTagIDType>,IDType<LevelTagSetIDType>> &*/);
    virtual bool isEdu(void);
    virtual int getActorFactory(void);
    virtual int getActorFactory2(void);
    virtual int getActorInfoRegistry(void);
    virtual int getEntityRegistry(void);
    virtual int getEntityRegistry2(void);
    virtual int getEntitySystems(void);
    virtual int getLevelEntity(void);
    virtual void runCommand(HashedString const&,class CommandOrigin &,class CommandOriginSystem,class CurrentCmdVersion);
    virtual void runCommand(class Command &,class CommandOrigin &,class CommandOriginSystem);
    virtual int getCapabilities(void);
    virtual int getTagRegistry(void);
    virtual int getPlayerMovementSettings(void);
    virtual void setPlayerMovementSettings(class PlayerMovementSettings const&);
    virtual void canUseSkin(class SerializedSkin const&,class NetworkIdentifier const&,uint64_t uuid,uint64_t const&);
    virtual int getPositionTrackerDBClient(void);
    virtual int getPositionTrackerDBServer(void);
    virtual void flushRunTimeLighting(void);
    virtual void loadBlockDefinitionGroup(class Experiments const&);
    virtual void initializeBlockDefinitionGroup(void);
    virtual int getUnknownBlockTypeRegistry(void);
    virtual bool isClientSide(void);
    virtual std::unordered_map<mceUUID, PlayerListEntry> getPlayerList(void);
    virtual int getPlayerList2(void);
    virtual int getPlayerXUID(unsigned __int64 UUID);
    virtual int getPlayerPlatformOnlineId(unsigned __int64 UUID);
    virtual int getActiveUsers(void);
    virtual class std::vector<class Actor*> getRuntimeActorList(void);
    virtual void notifySubChunkRequestManager(class SubChunkPacket const&);
    virtual int getSubChunkRequestManager(void);
    virtual int getPacketSender(void);
    virtual void setPacketSender(LoopbackPacketSender*);
    virtual int getNetEventCallback(void);
    virtual void setNetEventCallback(unsigned __int64 NonOwnerPointer);
    virtual int getIRandom(void);
    virtual int getRandom(void);
    virtual int getRandom2(void);
    virtual HitResult* getHitResult(void);
    virtual int getLiquidHitResult(void);
    virtual int getImmersiveReaderString(void);
    virtual void setImmersiveReaderString(std::string);
    virtual int getAdventureSettings(void);
    virtual int getAdventureSettings2(void);
    virtual int getGameRules(void);
    virtual int getGameRules2(void);
    virtual void hasStartWithMapEnabled(void);
    virtual bool isEditorWorld(void);
    virtual int getDefaultAbilities(void);
    virtual int getDefaultPermissions(void);
    virtual int getDefaultPermissions2(void);
    virtual int getTearingDown(void);
    virtual void takePicture(unsigned __int64 ImageBuffer, Actor*, Actor*, unsigned __int64 ScreenshotOptions);
    virtual int getLevelSoundManager(void);
    virtual int getSoundPlayer(void);
    virtual void setSimPaused(bool);
    virtual int getSimPaused(void);
    virtual void setFinishedInitializing(void);
    virtual int getLootTables(void);
    virtual void updateWeather(float, int, float, int);
    virtual int getNetherScale(void);
    virtual int getScoreboard(void);
    virtual int getScoreboard2(void);
    virtual void tryGetScoreboard(void);
    virtual int getPlayerAbilities(uint64_t const&);
    virtual void setPlayerAbilities(uint64_t const&, unsigned __int64 LayeredAbilities);
    virtual void sendAllPlayerAbilities(Actor const&);
    virtual int getRecipes(void);
    virtual int getBlockReducer(void);
    virtual int getTrimPatternRegistry(void);
    virtual int getTrimPatternRegistry2(void);
    virtual int getTrimMaterialRegistry(void);
    virtual int getTrimMaterialRegistry2(void);
    virtual void digestServerItemComponents(unsigned __int64 ItemComponentPacket);
    virtual int getRegisteredBorderBlock(void);
    virtual void use3DBiomeMaps(void);
    virtual void addBlockSourceForValidityTracking(BlockSource *);
    virtual void removeBlockSourceFromValidityTracking(BlockSource *);
    virtual void asLevel(void);
    virtual void asMultiPlayerLevel(void);
    virtual bool isClientSideGenerationEnabled(void);
    virtual void blockNetworkIdsAreHashes(void);
    virtual int getItemRegistry(void);
    virtual int getBlockRegistry(void);
    virtual void pauseAndFlushTaskGroups(void);
public:
    char pad_0001[88]; //0x0000
    int64_t listStart; //0x0058
    int64_t listEnd; //0x0060
    char pad_0068[2264]; //0x0068
    int64_t lookingState55; //0x0940
    char pad_0948[1784]; //0x0948

    HitResult* getHitResult2() {
        return Memory::CallVFunc<306, HitResult*>(this);
    }

    inline std::vector<Actor*> ForEachPlayer() {
        std::vector<Actor*> listOut;

        if (this) {
            std::function<bool __cdecl(Actor&)> functor = [&listOut](Actor& p) -> bool {
                listOut.push_back(&p);
                return true;
            };

            Memory::CallVFunc<208, void>(this, &functor); // Level::ForEachPlayer
        }
        return listOut;
    }

    PlayerListEntry* GetPlayerById(std::string xuid) {
        PlayerListEntry* player = nullptr;
        // find player by uuid in this->playerList using find
        for (auto entry : playerList) {
            if (entry.second.XUID == xuid) {
                player = &entry.second;
                break;
            }
        }

        return player;
    }

    inline HitResult* getRayTraceResult()
    {
        return this->getHitResult();
    }

    __forceinline uintptr_t** vTable()
    {
        return reinterpret_cast<uintptr_t**>((uintptr_t*)(this));
    }


};;