#pragma once

#pragma once
#include <cstdint>


#include <vector>
#include <string>
#include <optional>
#include <Prax/Utils/System/Memory.h>
#include <Prax/SDK/Game/Misc/Structs.h>
#include <Prax/SDK/Game/Entity/Inventory.h>


class Block
{
public:
    BUILD_ACCESS(this, uintptr_t**, vTable, 0x0);
    BUILD_ACCESS(this, class BlockLegacy*, blockLegacy, 0x30);

    virtual ~Block();
    virtual int32_t getRenderLayer();
};


enum BlockRenderLayer : int32_t {
    RENDERLAYER_DOUBLE_SIDED = 0x0,
    RENDERLAYER_BLEND = 0x1,
    RENDERLAYER_OPAQUE = 0x2,
    RENDERLAYER_OPTIONAL_ALPHATEST = 0x3,
    RENDERLAYER_ALPHATEST = 0x4,
    RENDERLAYER_SEASONS_OPAQUE = 0x5,
    RENDERLAYER_SEASONS_OPTIONAL_ALPHATEST = 0x6, // red
    RENDERLAYER_ALPHATEST_SINGLE_SIDE = 0x7, // red
    RENDERLAYER_ENDPORTAL = 0x8,
    RENDERLAYER_BARRIER = 0x9,
    RENDERLAYER_STRUCTURE_VOID = 0xA,
    _RENDERLAYER_COUNT = 0xB,
};



class Material {
public:
    BUILD_ACCESS(this, int, type, 0x0);
    BUILD_ACCESS(this, bool, isFlammable, 0x4);
    BUILD_ACCESS(this, bool, isNeverBuildable, 0x5);
    BUILD_ACCESS(this, bool, isLiquid, 0x6);
    BUILD_ACCESS(this, bool, isBlockingMotion, 0xC);
    BUILD_ACCESS(this, bool, isSuperHot, 0xF);
};


class BlockSource {
public:
    virtual int Destructor();
    virtual Block *getBlock(int, int, int); // 0 (0x0)
    virtual Block *getBlock(BlockPos const &); // 1 (0x8)
    virtual Block *getBlock(BlockPos const &, unsigned int); // 2 (0x10)
    virtual void getBlockEntity(BlockPos const &); // 3 (0x18)
    virtual void getExtraBlock(BlockPos const &); // 4 (0x20)
    virtual void getLiquidBlock(BlockPos const &); // 5 (0x28)
    virtual bool hasBlock(BlockPos const &); // 6 (0x30)
    virtual void containsAnyLiquid(AABB const &); // 7 (0x38)
    virtual void containsMaterial(AABB const &, int MaterialType); // 8 (0x40)
    virtual void getMaterial(BlockPos const &); // 9 (0x48)
    virtual void getMaterial(int, int, int); // 10 (0x50)
    virtual bool hasChunksAt(class Bounds const &, bool); // 11 (0x58)
    virtual bool hasChunksAt(BlockPos const &, int, bool); // 12 (0x60)
    virtual bool hasChunksAt(AABB const &, bool); // 13 (0x68)
    virtual void getDimensionId(void); // 14 (0x70)
    virtual void fetchAABBs(std::vector<AABB> &, AABB const &, bool); // 15 (0x78)
    virtual void fetchCollisionShapes(std::vector<AABB> &, AABB const &, float *,
                                      bool/*, optional_ref<GetCollisionShapeInterface const>*/); // 16 (0x80)
    virtual void getTallestCollisionShape(AABB const &, float *,
                                          bool/*, optional_ref<GetCollisionShapeInterface const>*/); // 17 (0x88)
    virtual void getBrightness(BlockPos const &); // 18 (0x90)
    virtual void getWeakRef(void); // 19 (0x98)
    virtual void addListener(class BlockSourceListener &); // 20 (0xA0)
    virtual void removeListener(BlockSourceListener &); // 21 (0xA8)
    virtual void fetchEntities(Actor const *, AABB const &, bool, bool); // 22 (0xB0)
    virtual void fetchEntities(int ActorType, AABB const &, Actor const *); // 23 (0xB8)
    virtual void setBlock(BlockPos const &, Block*, int, class ActorBlockSyncMessage const *, Actor *); // 24 (0xC0)
    virtual void getMaxHeight(void); // 25 (0xC8)
    virtual void getMinHeight(void); // 26 (0xD0)
    virtual void getDimension(void); // 27 (0xD8)
    virtual void getDimensionConst(void); // 28 (0xE0)
    virtual void getDimension2(void); // 29 (0xE8)
    virtual void getChunkAt(BlockPos const &); // 30 (0xF0)
    virtual void getILevel(void); // 31 (0xF8)
    virtual void fetchAABBs(AABB const &, bool); // 32 (0x100)
    virtual void fetchCollisionShapes(AABB const &, float *, bool, class IActorMovementProxy *); // 33 (0x108)
    virtual void getChunkSource(void); // 34 (0x110)
    virtual bool isSolidBlockingBlock(BlockPos const &); // 35 (0x118)
    virtual bool isSolidBlockingBlock(int, int, int); // 36 (0x120)
    virtual void areChunksFullyLoaded(BlockPos const &, int); // 37 (0x128)

    uintptr_t **vTable();

    // Function for the second getBlock that caches the address to call faster
    Block* _getBlock(BlockPos const& pos);

    void clearBlock(BlockPos pos);
};

class BlockData {
public:
    BlockPos position = BlockPos(0, 0, 0);
    int face = 0;

    BlockData(BlockPos position, int face) {
        this->position = position;
        this->face = face;
    }
};

class BlockLegacy {
public:
    char pad_0008[32]; //0x0008
    std::string tileName;

    BUILD_ACCESS(this, uintptr_t**, vTable, 0x0);
    BUILD_ACCESS_STRING(this, name, 0x28);
    BUILD_ACCESS_STRING(this, itemNamespace, 0xA0);
    BUILD_ACCESS(this, uint16_t, id, 0x1A6);
    BUILD_ACCESS(this, BlockRenderLayer*, renderLayer, 0x0B4);
    BUILD_ACCESS(this, Material*, material, 0x128);
    BUILD_ACCESS(this, bool, isSolid, 0x164);

    // new vTable
    virtual ~BlockLegacy();
    virtual void newBlockEntity(BlockPos const&, Block const&);
    virtual void getNextBlockPermutation(Block const&);
    virtual void clip(BlockSource const&, BlockPos const&, Vector3 const&, Vector3 const&, bool);
    virtual void getCollisionShape(/*AABB&, Block const&, IConstBlockSource const&, BlockPos const&, optional_ref<GetCollisionShapeInterface const>*/);
    virtual void addCollisionShapes(/*Block const&, BlockSource const&, BlockPos const&, AABB const*, std::vector<AABB>&, optional_ref<GetCollisionShapeInterface const>*/);
    virtual void getAABB(class IConstBlockSource const&, BlockPos const&, Block const&, AABB&, bool);
    virtual void addAABBs(Block const&, BlockSource const&, BlockPos const&, AABB const*, std::vector<AABB>&);
    virtual void getOutline(BlockSource&, BlockPos const&, AABB&);
    virtual void getVisualShapeInWorld(Block const&, class IConstBlockSource const&, BlockPos const&, AABB&);
    virtual void getVisualShape(Block const&, AABB&);
    virtual void getUIShape(Block const&, AABB&);
    virtual void getLiquidClipVolume(BlockSource&, BlockPos const&, AABB&);
    virtual bool isObstructingChests(BlockSource&, BlockPos const&, Block const&);
    virtual void randomlyModifyPosition(BlockPos const&, int&);
    virtual void randomlyModifyPosition(BlockPos const&);
    virtual void onProjectileHit(BlockSource&, BlockPos const&, Actor const&);
    virtual void onLightningHit(BlockSource&, BlockPos const&);
    virtual bool hasVariableLighting(void);
    virtual bool isStrippable(Block const&);
    virtual void getStrippedBlock(Block const&);
    virtual bool canProvideSupport(Block const&, unsigned char, class BlockSupportType);
    virtual bool canProvideMultifaceSupport(Block const&, unsigned char);
    virtual bool canConnect(Block const&, unsigned char, Block const&);
    virtual bool isMovingBlock(void);
    virtual void tryGetCopperBehavior(void);
    virtual bool canDamperVibrations(void);
    virtual bool canOccludeVibrations(void);
    virtual bool isStemBlock(void);
    virtual bool isContainerBlock(void);
    virtual bool isCraftingBlock(void);
    virtual bool isWaterBlocking(void);
    virtual bool isFenceBlock(void);
    virtual bool isFenceGateBlock(void);
    virtual bool isThinFenceBlock(void);
    virtual bool isWallBlock(void);
    virtual bool isStairBlock(void);
    virtual bool isSlabBlock(void);
    virtual bool isDoubleSlabBlock(void);
    virtual bool isDoorBlock(void);
    virtual bool isRailBlock(void);
    virtual bool isButtonBlock(void);
    virtual bool isLeverBlock(void);
    virtual bool isCandleCakeBlock(void);
    virtual bool isMultifaceBlock(void);
    virtual bool isSignalSource(void);
    virtual bool canBeOriginalSurface(void);
    virtual bool isSilentWhenJumpingOff(void);
    virtual bool isValidAuxValue(int);
    virtual bool canFillAtPos(BlockSource&, BlockPos const&, Block const&);
    virtual void sanitizeFillBlock(BlockSource&, BlockPos const&, Block const&);
    virtual void onFillBlock(BlockSource&, BlockPos const&, Block const&);
    virtual void getDirectSignal(BlockSource&, BlockPos const&, int);
    virtual bool canBeDestroyedByWaterSpread(void);
    virtual void waterSpreadCausesSpawn(void);
    virtual bool canContainLiquid(void);
    virtual void getRequiredMedium(void);
    virtual void shouldConnectToRedstone(BlockSource&, BlockPos const&, int);
    virtual void handlePrecipitation(BlockSource&, BlockPos const&, float, float);
    virtual bool canBeUsedInCommands(class BaseGameVersion const&);
    virtual void checkIsPathable(Actor&, BlockPos const&, BlockPos const&);
    virtual void shouldDispense(BlockSource&, Container&);
    virtual void dispense(BlockSource&, Container&, int, Vector3 const&, unsigned char);
    virtual void transformOnFall(BlockSource&, BlockPos const&, Actor*, float);
    virtual void onRedstoneUpdate(BlockSource&, BlockPos const&, int, bool);
    virtual void onMove(BlockSource&, BlockPos const&, BlockPos const&);
    virtual void detachesOnPistonMove(BlockSource&, BlockPos const&);
    virtual void movedByPiston(BlockSource&, BlockPos const&);
    virtual void onStructureBlockPlace(BlockSource&, BlockPos const&);
    virtual void onStructureNeighborBlockPlace(BlockSource&, BlockPos const&);
    virtual void setupRedstoneComponent(BlockSource&, BlockPos const&);
    virtual void getRedstoneProperty(BlockSource&, BlockPos const&);
    virtual void updateEntityAfterFallOn(BlockPos const&, class UpdateEntityAfterFallOnInterface&);
    virtual bool isBounceBlock(void);
    virtual bool isPreservingMediumWhenPlaced(BlockLegacy const*);
    virtual bool isFilteredOut(BlockRenderLayer);
    virtual bool canRenderSelectionOverlay(BlockRenderLayer);
    virtual void ignoreEntitiesOnPistonMove(Block const&);
    virtual void onFertilized(BlockSource&, BlockPos const&, Actor*, class FertilizerType);
    virtual void mayConsumeFertilizer(BlockSource&);
    virtual bool canBeFertilized(BlockSource&, BlockPos const&, Block const&);
    virtual void mayPick(void);
    virtual void mayPick(BlockSource const&, Block const&, bool);
    virtual void mayPlace(BlockSource&, BlockPos const&, unsigned char);
    virtual bool mayPlace(BlockSource&, BlockPos const&);
    virtual bool mayPlaceOn(BlockSource&, BlockPos const&);
    virtual void tryToPlace(BlockSource&, BlockPos const&, Block const&, ActorBlockSyncMessage const*);
    virtual void tryToTill(BlockSource&, BlockPos const&, Actor&, ItemStack&);
    virtual void breaksFallingBlocks(Block const&, BaseGameVersion);
    virtual void destroy(BlockSource&, BlockPos const&, Block const&, Actor*);
    virtual void getIgnoresDestroyPermissions(Actor&, BlockPos const&);
    virtual void neighborChanged(BlockSource&, BlockPos const&, BlockPos const&);
    virtual void getSecondPart(BlockSource const&, BlockPos const&, BlockPos&);
    virtual void playerWillDestroy(Actor&, BlockPos const&, Block const&);
    virtual void asItemInstance(Block const&, class BlockActor const*);
    virtual void trySpawnResourcesOnExplosion(BlockSource&, BlockPos const&, Block const&, class Randomize&, float);
    virtual void getPlacementBlock(Actor const&, BlockPos const&, unsigned char, Vector3 const&, int);
    virtual void calcVariant(BlockSource&, BlockPos const&, rgb const&);
    virtual bool isAttachedTo(BlockSource&, BlockPos const&, BlockPos&);
    virtual void attack(Actor*, BlockPos const&);
    virtual void shouldTriggerEntityInside(BlockSource&, BlockPos const&, Actor&);
    virtual bool canBeBuiltOver(BlockSource&, BlockPos const&, class BlockItem const&);
    virtual bool canBeBuiltOver(BlockSource&, BlockPos const&);
    virtual void triggerEvent(BlockSource&, BlockPos const&, int, int);
    virtual void executeEvent(BlockSource&, BlockPos const&, Block const&, std::string const&, Actor&);
    virtual bool hasTag(BlockSource&, BlockPos const&, Block const&, std::string const&);
    virtual void getMobToSpawn(class SpawnConditions const&, BlockSource&);
    virtual void shouldStopFalling(Actor&);
    virtual void pushesUpFallingBlocks(void);
    virtual bool canHaveExtraData(void);
    virtual bool hasComparatorSignal(void);
    virtual void getComparatorSignal(BlockSource&, BlockPos const&, Block const&, unsigned char);
    virtual bool canSlide(BlockSource&, BlockPos const&);
    virtual bool canInstatick(void);
    virtual bool canSpawnAt(BlockSource const&, BlockPos const&);
    virtual void notifySpawnedAt(BlockSource&, BlockPos const&);
    virtual void causesFreezeEffect(void);
    virtual void getIconYOffset(void);
    virtual void buildDescriptionId(Block const&);
    virtual bool isAuxValueRelevantForPicking(void);
    virtual void getColor(Block const&);
    virtual void getColorAtPos(BlockSource&, BlockPos const&);
    virtual void getColor(BlockSource&, BlockPos const&, Block const&);
    virtual void getColorForParticle(BlockSource&, BlockPos const&, Block const&);
    virtual bool isSeasonTinted(Block const&, BlockSource&, BlockPos const&);
    virtual void onGraphicsModeChanged(class BlockGraphicsModeChangeContext const&);
    virtual void getShadeBrightness(Block const&);
    virtual void telemetryVariant(BlockSource&, BlockPos const&);
    virtual void getVariant(Block const&);
    virtual bool canSpawnOn(Actor*);
    virtual void getRenderBlock(void);
    virtual void getMappedFace(unsigned char, Block const&);
    virtual void getFaceFlip(unsigned char, Block const&);
    virtual void animateTickBedrockLegacy(BlockSource&, BlockPos const&, class Random&);
    virtual void animateTick(BlockSource&, BlockPos const&, class Random&);
    virtual void init(void);
    virtual void tryLegacyUpgrade(unsigned short);
    virtual void dealsContactDamage(Actor const&, Block const&, bool);
    virtual void tryGetInfested(Block const&);
    virtual void tryGetUninfested(Block const&);
    virtual void _addHardCodedBlockComponents(void);
    virtual void onRemove(BlockSource&, BlockPos const&);
    virtual void onExploded(BlockSource&, BlockPos const&, Actor*);
    virtual void onStandOn(class EntityContext&, BlockPos const&);
    virtual void onPlace(BlockSource&, BlockPos const&);
    virtual void shouldTickOnSetBlock(void);
    virtual void tick(BlockSource&, BlockPos const&, Random&);
    virtual void randomTick(BlockSource&, BlockPos const&, Random&);
    virtual bool isInteractiveBlock(void);
    virtual void use(Actor&, BlockPos const&, unsigned char, std::optional<Vector3>);
    virtual void use(Actor&, BlockPos const&, unsigned char);
    virtual void allowStateMismatchOnPlacement(Block const&, Block const&);
    virtual bool canSurvive(BlockSource&, BlockPos const&);
    virtual void getRenderLayer(void);
    virtual void getRenderLayer(Block const&, BlockSource&, BlockPos const&);
    virtual void getExtraRenderLayers(void);
    virtual void getLight(Block const&);
    virtual void getLightEmission(Block const&);
    virtual void getEmissiveBrightness(Block const&);
    virtual void getMapColor(BlockSource&, BlockPos const&, Block const&);
    virtual void _onHitByActivatingAttack(BlockSource&, BlockPos const&, Actor*);
    virtual void entityInside(BlockSource&, BlockPos const&, Actor&);

    inline bool equalToBlockName(std::string block) {
        auto text = this->name.c_str();
        if (strcmp(text, block.c_str()) == NULL)
            return true;
        else
            return false;
    }

    inline bool containsString(std::string string) {
        auto text = this->name.c_str();
        return (strstr(text, string.c_str()) != NULL);
    }

    inline bool IsAir() {
        return id == 0;
    }

    inline bool isSolidBlock() {
        return this->isSolid;
    }

    inline bool isOre() {
        return containsString("ore");
    }

    inline bool isFullBlock() {
        return !this->isStemBlock() && !this->isFenceBlock() && !this->isFenceGateBlock() &&
               !this->isThinFenceBlock() && !this->isRailBlock() && !this->isButtonBlock() && !this->isLeverBlock() && !this->isCandleCakeBlock() &&
                   !this->isWallBlock() && !this->isStairBlock() && !this->isSlabBlock() && !this->isDoorBlock();
    }
};
