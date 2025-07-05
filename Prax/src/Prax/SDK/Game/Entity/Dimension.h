//
// Created by vastrakai on 10/10/2023.
//

#pragma once

#include <Prax/SDK/Game/World/Block.h>
#include <Prax/SDK/Game/Misc/Structs.h>
#include <Prax/SDK/Game/Entity/Inventory.h>

class Dimension {
public:
    BUILD_ACCESS(this, uintptr_t**, vTable, 0x0);

    // Add 2 to this to get actual index
    virtual bool isNaturalDimension(void);
    virtual void getDimensionId(void);
    virtual void sendPacketForPosition(BlockPos const&, class Packet const&, class Player const*);
    virtual void flushLevelChunkGarbageCollector(void);
    virtual void initializeWithLevelStorageManager(class LevelStorageManager&);
    virtual void init(void);
    virtual void tick(void);
    virtual void tickRedstone(void);
    virtual void Function7();
    virtual void Function8();
    virtual void Function9();
    virtual void Function10();
    virtual bool isValidSpawn(int, int);
    virtual void getBrightnessDependentFogColor(rgb const&, float); // 11 (0x58)
    virtual bool isFoggyAt(int, int); // 12 (0x60)
    virtual bool hasPrecipitationFog(void); // 13 (0x68)
    virtual void getCloudHeight(void); // 14 (0x70)
    virtual void getDefaultBiome(void); // 15 (0x78)
    virtual void mayRespawnViaBed(void); // 16 (0x80)
    virtual bool hasGround(void); // 17 (0x88)
    virtual void getSpawnPos(void); // 18 (0x90)
    virtual void getSpawnYPosition(void); // 19 (0x98)
    virtual bool hasBedrockFog(void); // 20 (0xA0)
    virtual void getClearColorScale(void); // 21 (0xA8)
    virtual void showSky(void); // 22 (0xB0)
    virtual bool isDay(void); // 23 (0xB8)
    virtual void getSunIntensity(float, Vector3 const&, float); // 24 (0xC0)
    virtual void forceCheckAllNeighChunkSavedStat(void); // 25 (0xC8)
    virtual void Function25(); // 26 (0xD0)
    virtual void onChunkLoaded(class ChunkSource&, class LevelChunk&); // 27 (0xD8)
    virtual void deserialize(class CompoundTag const&); // 28 (0xE0)
    virtual void serialize(CompoundTag&); // 29 (0xE8)
    virtual void sendBroadcast(Packet const&, class Actor*); // 30 (0xF0)
    virtual bool is2DPositionRelevantForPlayer(BlockPos const&, Actor&); // 31 (0xF8)
    virtual bool isActorRelevantForPlayer(Actor&, Actor const&); // 32 (0x100)
    virtual void onBlockEvent(BlockSource&, int, int, int, int, int); // 33 (0x108)
    virtual void onBlockChanged(BlockSource&, BlockPos const&, unsigned int, Block const&, Block const&, int, class ActorBlockSyncMessage const*, class BlockChangedEventTarget, Actor*); // 34 (0x110)
    virtual void onLevelDestruction(std::string const&); // 35 (0x118)
    virtual void getLightTextureImageBuilder(void); // 36 (0x120)
    virtual void getBrightnessRamp(void); // 37 (0x128)
    virtual void startLeaveGame(void); // 38 (0x130)
    virtual void getTimeOfDay(int, float); // 39 (0x138)
    virtual void _createChunkBuildOrderPolicy(void); // 40 (0x140)
};
