#pragma once

#include "../Block.h"
#include "../../Actor/Level/Weather.h"
#include "../../Actor/Level/Dimension.h"
#include "../Chunk/ChunkPos.h"
#include "../Chunk/SubChunkStorage.h"
#include "../Chunk/SubChunkBlockStorage.h"
#include "../Chunk/SubChunk.h"
#include "../Chunk/LevelChunk.h"
#include "../Chunk/ChunkSource.h"

class Actor;
class Bounds;

class BlockSource {
public:
	CLASS_FIELD(class Dimension*, Dimension, 0x30); // 1.21.50
public:
	virtual ~BlockSource();
	virtual class Block* getBlock(int, int, int);
	virtual class Block* getBlockL(Vector3<int>);
	virtual class Block* getBlock(Vector3<int>, unsigned int);
    virtual class BlockActor* getBlockEntity(Vector3<int>);
    virtual class Block* getExtraBlock(Vector3<int>);
    virtual class Block* getLiquidBlock(Vector3<int>);
    virtual bool hasBlock(Vector3<int>);
    virtual bool containsAnyLiquid(struct AABB const&);
    virtual bool containsMaterial(struct AABB const&, MaterialType materialType);
    virtual class Material* getMaterial(Vector3<int>);
    virtual class Material* getMaterial(int, int, int);
	virtual bool hasChunksAt(Bounds const&, bool);
	virtual bool hasChunksAt(Vector3<int> const&, int, bool);
	virtual bool hasChunksAt(AABB const&, bool);
	virtual int getDimensionId(void);
	virtual __int64 fetchAABBs(std::vector<AABB>&, AABB const&, bool);
	virtual __int64 fetchCollisionShapes(std::vector<AABB>&, AABB const&, float*, bool, std::optional<class GetCollisionShapeInterface const>);
	virtual __int64 getTallestCollisionShape(AABB const&, float*, bool, class IActorMovementProxy*);
	virtual __int64 getWeakRef(void);
	virtual __int64 addListener(void);
	virtual __int64 removeListener(void);
	virtual __int64 fetchEntities(Actor const*, AABB const&, bool);
	virtual __int64 fetchEntities(int actorId, AABB const&, Actor const*);
	virtual __int64 setBlock(Vector3<int> const&, Block const&, int, class ActorBlockSyncMessage const*, Actor*);
	virtual __int64 getMinHeight(void);
	virtual class Dimension* getDimensionVtable(void);
	virtual class Dimension* getDimensionConst(void);
	virtual class Dimension* getDimension1(void);
	virtual LevelChunk* getChunkAt(Vector3<int> const&);
	virtual __int64 getILevel(void);
	virtual __int64 fetchAABBs(AABB const&, bool);
	virtual __int64 fetchCollisionShapes(AABB const&, float*, bool, IActorMovementProxy*);
	virtual __int64 getChunkSource(void); // BlockSource::Clip
	virtual bool isSolidBlockingBlock(Vector3<int> const&);
	virtual bool isSolidBlockingBlock(int, int, int);
	virtual bool areChunksFullyLoaded(Vector3<int> const&, int);
	virtual bool canDoBlockDrops(void);
	virtual bool canDoContainedItemDrops(void);
public:
	LevelChunk* getChunk(const ChunkPos& pos)
	{
		static int mIndex = 37;
		return Memory::CallVirtualFunc<LevelChunk*, ChunkPos const&>(mIndex, this, pos);
	}

	Block* getBlock(Vector3<int> pos) 
	{
		__int64 v = *(__int64*)this;
		int posz[3] = { pos.x, pos.y, pos.z };
		auto func = reinterpret_cast<__int64(__fastcall**)(BlockSource*, const int*)>(v + 0x10);
		return (Block*)(*func)(this, posz);
	}

public:
	uintptr_t** vTable()
	{
		if (this == nullptr) {
			return 0x0;
		}
		return *reinterpret_cast<uintptr_t***>(reinterpret_cast<__int64>(this) + 0x0);
	}
};