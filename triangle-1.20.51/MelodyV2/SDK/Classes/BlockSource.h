#pragma once
#include "Block.h"
#include "../../Utils/MemoryUtils.h"
#include "../../Utils/Math.h"
class BlockActor;
class Material;
class Actor;

class BlockSource {
public:
	float getSeenPercent(const Vec3<float>& eyePos, const AABB& targetAABB) {
		using func_t = float(__fastcall*)(BlockSource*, const Vec3<float>&, const AABB&);
		static func_t func = (func_t)findSig(Sigs::blocksource::getSeenPercent);
		return func(this, eyePos, targetAABB);
	}
private:
	virtual void Destructor();
public:
	virtual Block* getBlock(int, int, int);
	virtual Block* getBlock(Vec3<int> const&);
	virtual Block* getBlock(Vec3<int> const&, int);
	virtual BlockActor* getBlockEntity(Vec3<int> const&);
	virtual Block* getExtraBlock(Vec3<int> const&);
	virtual Block* getLiquidBlock(Vec3<int> const&);
	virtual bool hasBlock(Vec3<int> const&);
	virtual bool containsAnyLiquid(AABB const&);
	virtual bool containsMaterial(AABB const&, enum MaterialType);
	virtual Material* getMaterial(Vec3<int> const&);
	virtual Material* getMaterial(int, int, int);
	virtual bool hasChunksAt(__int64 const&, bool);
	virtual bool hasChunksAt(Vec3<int> const&, int, bool);
	virtual bool hasChunksAt(AABB const&, bool);
	virtual int getDimensionId(void);
	virtual void fetchAABBs(std::vector<AABB>&, AABB const&, bool);
	virtual void fetchCollisionShapes(std::vector<AABB>&, AABB const&, float*, bool, __int64 const);
	virtual float getTallestCollisionShape(AABB const&, float*, bool, __int64 const);
	virtual int getBrightness(Vec3<int> const&);
	virtual int getWeakRef(void);
	virtual void addListener(__int64&);
	virtual void removeListener(__int64&);
	virtual void fetchEntities(Actor const*, AABB const&, bool, bool);
	virtual void fetchEntities(struct EntityId, AABB const&, Actor const*, __int64);
	virtual void setBlock(Vec3<int> const&, Block const&, int, __int64 const*, Actor*);
	virtual int16_t getMaxHeight(void);
	virtual int16_t getMinHeight(void);
	virtual class Dimension* getDimension(void);
	virtual Dimension* getDimensionConst(void);
	virtual Dimension* getDimension2(void);
	virtual __int64 getChunkAt(Vec3<int> const&);
	virtual __int64 getILevel(void);
	virtual void fetchAABBs(AABB const&, bool);
	virtual void fetchCollisionShapes(AABB const&, float*, bool, class IPlayerMovementProxy*);
	virtual __int64 getChunkSource(void);
	virtual bool isSolidBlockingBlock(Vec3<int> const&);
	virtual bool isSolidBlockingBlock(int, int, int);
	virtual bool areChunksFullyLoaded(Vec3<int> const&, int);
	virtual bool canDoBlockDrops(void);
	virtual bool canDoContainedItemDrops(void);
};
