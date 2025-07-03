#pragma once

#include "LevelChunkBuilderData.h"

class ChunkSource : Bedrock::EnableNonOwnerReferences {
    enum LoadMode : int32_t {
        None = 0,
        Deferred = 1
    };

    /*These member variables were obtained by analyzing BDS-WIN 1.20.61 using IDA.*/

    int                                                     mChunkSide;             // this+0x18
    Level*                                                  level;                  // this+0x20
    Dimension*                                              dimension;              // this+0x28
    ChunkSource*                                            mParent;                // this+0x30
    std::unique_ptr<ChunkSource>                            mOwnedParent;           // this+0x38
    LevelChunkBuilderData*                                  mLevelChunkBuilderData; // this+0x40
    std::atomic_bool                                        mShuttingDown;          // this+0x48
    char                                                    pad_0x0049[0x17];       // this+0x49 (0x48 + 0x17 = 0x60)
    std::unordered_map<ChunkPos, std::weak_ptr<LevelChunk>> chunkStorage;           // this+0x60

public:
    // prevent constructor by default
    ChunkSource& operator=(ChunkSource const&);
    ChunkSource(ChunkSource const&);
    ChunkSource();
public:
    // vIndex: 0
    virtual ~ChunkSource();

    // vIndex: 1
    virtual void shutdown();

    // vIndex: 2
    virtual bool isShutdownDone();

    // vIndex: 3
    virtual std::shared_ptr<class LevelChunk> getExistingChunk(class ChunkPos const&);

    // vIndex: 4
    virtual std::shared_ptr<class LevelChunk> getRandomChunk(class Random& random);

    // vIndex: 5
    virtual bool isChunkKnown(class ChunkPos const& chunkPos);

    // vIndex: 6
    virtual bool isChunkSaved(class ChunkPos const& chunkPos);

    // vIndex: 7
    virtual std::shared_ptr<class LevelChunk> createNewChunk(class ChunkPos const& cp, ::ChunkSource::LoadMode lm, bool readOnly);

    // vIndex: 8
    virtual std::shared_ptr<class LevelChunk> getOrLoadChunk(class ChunkPos const& cp, ::ChunkSource::LoadMode lm, bool readOnly);

    // vIndex: 9
    virtual bool postProcess(class ChunkViewSource& neighborhood);

    // vIndex: 10
    virtual void checkAndReplaceChunk(class ChunkViewSource& neighborhood, class LevelChunk& lc);

    // vIndex: 11
    virtual void loadChunk(class LevelChunk& lc, bool forceImmediateReplacementDataLoad);

    // vIndex: 12
    virtual void postProcessMobsAt(class BlockSource& region, int chunkWestBlock, int chunkNorthBlock, class Random& random);

    // vIndex: 13
    virtual bool saveLiveChunk(class LevelChunk& lc);

    // vIndex: 14
    virtual void writeEntityChunkTransfer(class LevelChunk& levelChunk);

    // vIndex: 15
    virtual void writeEntityChunkTransfersToUnloadedChunk(class ChunkKey const&, std::vector<struct ActorUnloadedChunkTransferEntry> const&);

    // vIndex: 16
    virtual void deserializeActorStorageToLevelChunk(class LevelChunk&);

    // vIndex: 17
    virtual void hintDiscardBatchBegin();

    // vIndex: 18
    virtual void hintDiscardBatchEnd();

    // vIndex: 19
    virtual void acquireDiscarded(std::unique_ptr<class LevelChunk, struct LevelChunkFinalDeleter> ptr);

    // vIndex: 20
    virtual void compact();

    // vIndex: 21
    virtual void flushPendingDiscardedChunkWrites();

    // vIndex: 22
    virtual void flushThreadBatch();

    // vIndex: 23
    virtual bool isWithinWorldLimit(class ChunkPos const& cp) const;

    // vIndex: 24
    virtual std::unordered_map<class ChunkPos, std::weak_ptr<class LevelChunk>> const* getChunkMap();

    // vIndex: 25
    virtual std::unordered_map<class ChunkPos, std::weak_ptr<class LevelChunk>> const& getStorage() const;

    // vIndex: 26
    virtual void clearDeletedEntities();

    // vIndex: 27
    virtual bool canCreateViews() const;

    // vIndex: 28
    virtual std::unique_ptr<class BlendingDataProvider> tryGetBlendingDataProvider();

    // vIndex: 29
    virtual std::shared_ptr<class LevelChunkMetaDataDictionary> loadLevelChunkMetaDataDictionary();

    // vIndex: 30
    virtual void setLevelChunk(std::shared_ptr<class LevelChunk>);

    // vIndex: 31
    virtual bool canLaunchTasks() const;

    // vIndex: 32
    virtual bool chunkPosNeedsBlending(class ChunkPos const& cp);
};