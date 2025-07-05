#pragma once

#include "Inventory.h"

#include <mutex>
#include <vector>
#include <string>
#include <memory>
#include <xorstr.hpp>
#include <functional>
#include <type_traits>
#include <unordered_map>

#include "Level.h"
#include "GameMode.h"
#include "Dimension.h"
#include "Attributes.h"
#include <Prax/Utils/Misc.h>
#include "../Network/Network.h"
#include <Prax/SDK/SigManager.h>
#include <Prax/Utils/System/Memory.h>
#include <Prax/Hook/Hooks/BaseTick.h>

#include <Prax/SDK/Game/Entity/Components/AABBShapeComponent.h>
#include <Prax/SDK/Game/Entity/Components/JumpComponent.h>
#include <Prax/SDK/Game/Entity/Components/MoveInputComponent.h>
#include <Prax/SDK/Game/Entity/Components/RuntimeIDComponent.h>
#include <Prax/SDK/Game/Entity/Components/RenderPositionComponent.h>
#include <Prax/SDK/Game/Entity/Components/ActorHeadRotationComponent.h>
#include <Prax/SDK/Game/Entity/Components/StateVectorComponent.h>
#include <Prax/SDK/Game/Entity/Components/ActorRotationComponent.h>
#include <Prax/SDK/Game/Network/Other/InventoryTransactionPacket.h>
#include <Prax/SDK/Game/Network/Other/PlayerAuthInputPacket.h>
#include <Prax/Utils/String.h>

#include "ActorFlags.h"
#include "ContainerManagerModel.h"
#include "Components/MaxAutoStepComponent.h"
#include "Components/MobBodyRotationComponent.h"
#include "Components/MobEffectsComponent.h"


enum ArmorSlot
{
    Helmet = 0,
    Chestplate = 1,
    Leggings = 2,
    Boots = 3
};




enum class PlayerAbility : uint8_t {
    BUILD = 0b0000'0001,
    MINE = 0b0000'0010,
    DOORS_AND_SWITCHES = 0b0000'0100,
    OPEN_CONTAINERS = 0b0000'1000,
    ATTACK_PLAYERS = 0b0001'0000,
    ATTACK_MOBS = 0b0010'0000,
    OPERATOR_COMMANDS = 0b0100'0000,
    TELEPORT = 0b1000'0000,

    INVULNERABLE,
    FLYING,
    MAY_FLY,
    INSTABUILD,
    LIGHTNING,
    FLY_SPEED,
    WALK_SPEED,
    MUTED,
    WORLD_BUILDER,
    NO_CLIP,
    PRIVILEGED_BUILDER
};

class CheatTable
{
public:
    std::map<std::string, uint64_t> addresses;

    std::string dumpToXml()
    {
        std::string result = "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
        result += "<CheatTable>";
        int id = 0;
        result += "<CheatEntries>";
        for (auto& [name, address] : addresses)
        {
            id++;
            result += "<CheatEntry>";
            result += "<ID>" + std::to_string(id) + "</ID>";
            result += "<Description>\"" + name + "\"</Description>";
            result += "<ShowAsHex>1</ShowAsHex>";
            result += "<ShowAsSigned>0</ShowAsSigned>";
            result += "<VariableType>8 Bytes</VariableType>";
            result += "<Address>" + String::ToHex(address) + "</Address>";
            result += "</CheatEntry>";
        }
        result += "</CheatEntries>";
        result += "</CheatTable>";

        return result;

    }
};


class ActorDamageSource;

class Actor
{
public:
    unsigned char padding_0x0[0x448 - 8];
    BUILD_ACCESS(this, EntityContext, entityCtx, 0x08) // updated
    BUILD_ACCESS(this, Dimension*, dimension, 0x248); // updated
    BUILD_ACCESS(this, Level*, level, 0x250); // updated
    BUILD_ACCESS(this, float, timeAlongSwing, 0x590);
    BUILD_ACCESS(this, bool, isSwinging, 0x588); // updated
    BUILD_ACCESS(this, int, swingProgress, 0x58C); // updated
    BUILD_ACCESS(this, int*, hurtTime, 0x5A8); // updated
    BUILD_ACCESS(this, int, buildPlatform, 0x62C); // updated
    BUILD_ACCESS(this, ContainerManagerModel*, containerManagerModel, 0x750); // updated // This can be updated with the AutoUpdater, function: Player::getContainerManager
    BUILD_ACCESS(this, PlayerInventoryProxy*, supplies, 0x760); // updated
    BUILD_ACCESS(this, class SerializedSkin*, mSerializedSkin, 0x780); // updated
    BUILD_ACCESS(this, int, ticksUsingItem, 0xB70); // updated
    BUILD_ACCESS(this, GameMode*, gamemode, 0xEC8); // updated

    BUILD_ACCESS(this, bool, isDestroying, 0x19F0); // updated
    BUILD_ACCESS_STRING(this, localName, 0x1D30); // updated
    BUILD_ACCESS_STRING(this, entityIdString, 0x2B8); // updated

    void logAllFieldAddresses()
    {
        CheatTable table;
        table.addresses["entityCtx"] = reinterpret_cast<uintptr_t>(this) + 0x08;
        table.addresses["dimension"] = reinterpret_cast<uintptr_t>(this) + 0x250;
        table.addresses["level"] = reinterpret_cast<uintptr_t>(this) + 0x258;
        table.addresses["timeAlongSwing"] = reinterpret_cast<uintptr_t>(this) + 0x5AC;
        table.addresses["isSwinging"] = reinterpret_cast<uintptr_t>(this) + 0x5B0;
        table.addresses["swingProgress"] = reinterpret_cast<uintptr_t>(this) + 0x5B4;
        table.addresses["hurtTime"] = reinterpret_cast<uintptr_t>(this) + 0x5D0;
        table.addresses["buildPlatform"] = reinterpret_cast<uintptr_t>(this) + 0x654;
        table.addresses["containerManagerModel"] = reinterpret_cast<uintptr_t>(this) + 0x778;
        table.addresses["supplies"] = reinterpret_cast<uintptr_t>(this) + 0x788;
        table.addresses["mSerializedSkin"] = reinterpret_cast<uintptr_t>(this) + 0x7A8;
        table.addresses["ticksUsingItem"] = reinterpret_cast<uintptr_t>(this) + 0xB98;
        table.addresses["gamemode"] = reinterpret_cast<uintptr_t>(this) + 0xEB0;
        table.addresses["isDestroying"] = reinterpret_cast<uintptr_t>(this) + 0x19D8;
        table.addresses["localName"] = reinterpret_cast<uintptr_t>(this) + 0x1D18;
        table.addresses["entityIdString"] = reinterpret_cast<uintptr_t>(this) + 0x2C0;

        Logger::Write(__FUNCTION__, table.dumpToXml(), Logger::Debug);
    }

    template<typename component_t>
    auto getOrAddComponent()
    {
        auto storage = assure_for<component_t>(this->entityCtx.registry);
        if(storage->contains(this->entityCtx.entityId))
        {
            return storage->get(this->entityCtx.entityId);
        }
        return storage->emplace(this->entityCtx.entityId);
    }

    // remove component
    template<typename component_t>
    void removeComponent()
    {
        if(auto storage = assure_for<component_t>(this->entityCtx.registry);
            storage->contains(this->entityCtx.entityId))
        {
            storage->remove(this->entityCtx.entityId);
        }
    }

    template<typename component_t>
    std::string assureForComponent()
    {
        uint32_t hash = entt::type_hash<component_t>::value();
        auto* bytes = (uint8_t*)&hash;

        std::stringstream ss;
        ss << "ba"; //mov (int32_t) instruction

        //Add the hash with this
        for (int i = 0; i < sizeof(uint32_t); i++)
        {
            ss << " " << std::setw(2) << std::setfill('0') << std::hex << (0xFF & bytes[i]);
        }

        return ss.str();
    }

    template<typename component_t>
    void* resolveAssureFunc()
    {
        //This assure sig finds any reference to the hash
        std::string assureSig = assureForComponent<component_t>();
        std::string componentName = typeid(component_t).name();
        uintptr_t resultAddr = Memory::SearchAOB(assureSig.c_str());
        // check if the address is valid and show a message box if not

        if (resultAddr == 0)
        {
            std::string assureText = "Assure Sig not valid (" + componentName + ")";
            MessageBoxA(nullptr, assureSig.c_str(), assureText.c_str(), MB_OK | MB_ICONERROR);
            return nullptr;
        }
        assert(resultAddr != 0);
        resultAddr += 5;

        //Search for the next call
        uint32_t relCallOffset = 0;
        uint64_t assureAddr = 0;
        do
        {
            // check if the byte at the current address is not E8 or not CC and if so, break
            uint8_t byte = *(uint8_t*)resultAddr;
            if (byte != 0xE8 && byte != 0xCC)
            {
                resultAddr += 1;
                continue;
            }

            // If the bytes is 0xE8 then it is a call, so we need to get the offset
            if (byte == 0xE8)
            {
                //Get the offset
                relCallOffset = *(uint32_t*)(resultAddr + 1);
                assureAddr = resultAddr + 5 + relCallOffset;
                std::span<std::byte> text = hat::process::get_section_data(hat::process::get_process_module(), ".text");

                // Check if the assure address is divisible by 16
                if (assureAddr % 16 != 0)
                {
                    resultAddr += 1;
                    continue;
                }

                // check if the address is in the .text section
                if (assureAddr >= reinterpret_cast<uintptr_t>(text.data()) && assureAddr < reinterpret_cast<uintptr_t>(text.data()) + text.size())
                {
                    break;
                }

                resultAddr += 1;
            }
        }
        while(true); //While the instruction is not a call

        // Log this

        Logger::Write(__FUNCTION__, "Assure function for " + componentName + " found at " + Misc::GetModuleOfAddress(assureAddr) + " Sig: " + assureSig, Logger::Debug);

        return reinterpret_cast<void*>(assureAddr);
    }

    template<typename component_t>
    auto* assure_for(entt::basic_registry<EntityId>* registry)
    {
        using assure_t = entt::basic_storage<component_t, EntityId>* (__fastcall *)(entt::basic_registry<EntityId>*, uint32_t);
        static auto assureFunc = reinterpret_cast<assure_t>(resolveAssureFunc<component_t>());

        return assureFunc(registry, entt::type_hash<component_t>::value());
    }


    template<typename... type_t>
    [[nodiscard]] auto try_get() {
        if constexpr(sizeof...(type_t) == 1u) {
            auto* pool = assure_for<std::remove_const_t<type_t>...>(entityCtx.registry);
            return (pool && pool->contains(entityCtx.entityId)) ? std::addressof(pool->get(entityCtx.entityId)) : nullptr;
        } else {
            return std::make_tuple(try_get<type_t>(entityCtx.entityId)...);
        }
    }

        // new vTable
    virtual bool getStatusFlag(ActorFlags) = 0;
    virtual void setStatusFlag(ActorFlags, bool) = 0;
    virtual void outOfWorld(void) = 0;
    virtual void reloadHardcoded(/*Actor::InitializationMethod, VariantParameterList const&*/) = 0;
    virtual void reloadHardcodedClient(/*Actor::InitializationMethod, VariantParameterList const&*/) = 0;
    virtual void initializeComponents(/*Actor::InitializationMethod, VariantParameterList const&*/) = 0;
    virtual void reloadComponents(/*Actor::InitializationMethod, VariantParameterList const&*/) = 0;
    virtual void _serverInitItemStackIds(void) = 0;
    virtual void _doInitialMove(void) = 0;
    virtual void hasComponent(class HashedString* hashedstr) = 0;
    virtual ~Actor() = 0;
    virtual void resetUserPos(bool) = 0;
    virtual int getOwnerEntityType(void) = 0;
    virtual void remove(void) = 0;
    virtual int getFiringPos(void) = 0;
    virtual float getInterpolatedBodyRot(float) = 0;
    virtual float getInterpolatedHeadRot(float) = 0;
    virtual float getInterpolatedBodyYaw(float) = 0;
    virtual float getYawSpeedInDegreesPerSecond(void) = 0;
    virtual float getInterpolatedRidingOffset(float) = 0;
    virtual bool isFireImmune(void) = 0;
    virtual void blockedByShield(class ActorDamageSource const&, Actor&) = 0;
    virtual bool canDisableShield(void) = 0;
    virtual void teleportTo(Vector3 const&, bool, int, int, bool) = 0;
    virtual void lerpMotion(Vector3 const&) = 0;
    virtual void tryCreateAddActorPacket(void) = 0;
    virtual void normalTick(void) = 0;
    virtual void baseTick(void) = 0;
    virtual void passengerTick(void) = 0;
    virtual void startRiding(Actor&) = 0;
    virtual void addPassenger(Actor&) = 0;
    virtual int getExitTip(std::string const&, class InputMode, class NewInteractionModel) = 0;
    virtual int getEntityLocNameString(void) = 0;
    virtual bool isInWall(void) = 0;
    virtual bool isInvisible(void) = 0;
    virtual bool canShowNameTag(void) = 0;
    virtual std::string getFormattedNameTag(void) = 0;
    virtual bool getAlwaysShowNameTag(void) = 0;
    virtual float getShadowRadius(void) = 0;
    virtual float getHeadLookVector(float) = 0;
    virtual void canInteractWithOtherEntitiesInGame(void) = 0;
    virtual int getBrightness(float, class IConstBlockSource const&) = 0;
    virtual int playerTouch(class Player&) = 0;
    virtual bool _isImmobile(void) = 0;
    virtual bool isSilentObserver(void) = 0;
    virtual bool isSleeping(void) = 0;
    virtual void setSleeping(bool) = 0;
    virtual void setSneaking(bool) = 0;
    virtual bool isBlocking(void) = 0;
    virtual bool isDamageBlocked(uintptr_t ActorDamageSource) = 0;
    virtual bool isAlive(void) = 0;
    virtual bool isOnFire(void) = 0;
    virtual bool isSurfaceMob(void) = 0;
    virtual bool isTargetable(void) = 0;
    virtual bool canAttack(Actor*, bool) = 0;
    virtual void setTarget(Actor*) = 0;
    virtual bool isValidTarget(Actor*) = 0;
    virtual int attack(Actor&, class ActorDamageCause const&) = 0;
    virtual void performRangedAttack(Actor&, float) = 0;
    virtual int setOwner(class ActorUniqueID) = 0;
    virtual void setSitting(bool) = 0;
    virtual void onTame(void) = 0;
    virtual void onFailedTame(void) = 0;
    virtual void setStanding(bool) = 0;
    virtual bool canPowerJump(void) = 0;
    virtual bool isEnchanted(void) = 0;
    virtual void shouldRender(void) = 0;
    virtual void playAmbientSound(void) = 0;
    virtual int getAmbientSound(void) = 0;
    virtual bool isInvulnerableTo(uintptr_t ActorDamageSource) = 0;
    virtual int getBlockDamageCause(Block const&) = 0;
    virtual void doFireHurt(int) = 0;
    virtual void onLightningHit(void) = 0;
    virtual void feed(int) = 0;
    virtual void handleEntityEvent(uintptr_t ActorEvent, int) = 0;
    virtual int getActorRendererId(void) = 0;
    virtual int despawn(void) = 0;
    virtual int setArmor(ArmorSlot, ItemStack const&) = 0;
    virtual int getArmorMaterialTypeInSlot(ArmorSlot) = 0;
    virtual int getArmorMaterialTextureTypeInSlot(ArmorSlot) = 0;
    virtual int getArmorColorInSlot(ArmorSlot, int) = 0;
    virtual int setEquippedSlot(int EquipmentSlot, ItemStack const&) = 0;
    virtual int setCarriedItem(ItemStack const&) = 0;
    virtual int getCarriedItem(void) = 0;
    virtual void setOffhandSlot(ItemStack const&) = 0;
    virtual ItemStack* getEquippedTotem(void) = 0;
    virtual void consumeTotem(void) = 0;
    virtual void save(void* compoundtag) = 0;
    virtual void load() = 0;
    virtual void queryEntityRenderer(void) = 0;
    virtual int getSourceUniqueID(void) = 0;
    virtual bool canFreeze(void) = 0;
    virtual int getLiquidAABB(int MaterialType) = 0;
    virtual void handleInsidePortal(BlockPos const&) = 0;
    virtual bool canChangeDimensionsUsingPortal(void) = 0;
    virtual void changeDimension(class Dimension, bool) = 0;
    virtual void changeDimension(uintptr_t ChangeDimensionPacket) = 0;
    virtual class Actor* getControllingPlayer(void) = 0;
    virtual void causeFallDamage(float, float, uintptr_t ActorDamageSource) = 0;
    virtual void onSynchedDataUpdate(int) = 0;
    virtual bool canAddPassenger(Actor&) = 0;
    virtual bool canPickupItem(ItemStack const&) = 0;
    virtual bool canBePulledIntoVehicle(void) = 0;
    virtual void inCaravan(void) = 0;
    virtual void sendMotionPacketIfNeeded(void) = 0;
    virtual void canSynchronizeNewEntity(void) = 0;
    virtual void startSwimming(void) = 0;
    virtual void stopSwimming(void) = 0;
    virtual void buildDebugInfo(std::basic_string<char, std::char_traits<char>, std::allocator<char>>&) = 0;
    virtual int getCommandPermissionLevel(void) = 0;
    virtual int getDeathTime(void) = 0;
    virtual bool canBeAffected(unsigned int) = 0;
    virtual bool canBeAffectedByArrow(class MobEffectInstance const&) = 0;
    virtual int onEffectRemoved(MobEffectInstance&) = 0;
    virtual bool canObstructSpawningAndBlockPlacement(void) = 0;
    virtual int getAnimationComponent(void) = 0;
    virtual int openContainerComponent(Player&) = 0;
    virtual int swing(void) = 0;
    virtual int useItem(ItemStack*, int ItemUseMethod, bool) = 0;
    virtual int getDebugText(std::vector<std::string>&) = 0;
    virtual int getMapDecorationRotation(void) = 0;
    virtual int getPassengerYRotation(Actor const&) = 0;
    virtual void add(ItemStack&) = 0;
    virtual void drop(ItemStack const&, bool) = 0;
    virtual int getInteraction(Actor&, uintptr_t ActorInteraction, struct Vector3 const&) = 0;
    virtual bool canDestroyBlock(class Block const&) = 0;
    virtual void setAuxValue(int) = 0;
    virtual void startSpinAttack(void) = 0;
    virtual void stopSpinAttack(void) = 0;
    virtual void renderDebugServerState(class Options const&) = 0;
    virtual void kill(void) = 0;
    virtual void die(uintptr_t ActorDamageSource) = 0;
    virtual void shouldDropDeathLoot(void) = 0;
    virtual void applySnapshot(class EntityContext const&, EntityContext const&) = 0;
    virtual float getNextStep(float) = 0;
    virtual void onPush(Actor&) = 0;
    virtual Vector3 getLastDeathPos(void) = 0;
    virtual int getLastDeathDimension(void) = 0;
    virtual bool hasDiedBefore(void) = 0;
    virtual void doEnterWaterSplashEffect(void) = 0;
    virtual void doExitWaterSplashEffect(void) = 0;
    virtual void doWaterSplashEffect(void) = 0;
    virtual void _shouldProvideFeedbackOnHandContainerItemSet(int HandSlot, ItemStack const&) = 0;
    virtual void _shouldProvideFeedbackOnArmorSet(ArmorSlot, ItemStack const&) = 0;
    virtual void updateEntitySpecificMolangVariables(uintptr_t RenderParams) = 0;
    virtual void shouldTryMakeStepSound(void) = 0;
    virtual void _hurt(uintptr_t ActorDamageSource, int, bool, bool) = 0;
    virtual void readAdditionalSaveData(void* compoundtag, uintptr_t DataLoadHelper) = 0;
    virtual void addAdditionalSaveData(void* compoundtag) = 0;
    virtual void _playStepSound(BlockPos const&, Block const&) = 0;
    virtual void _doAutoAttackOnTouch(Actor&) = 0;
    virtual void knockback(Actor*, int, float, float, float, float, float) = 0;
    virtual void spawnAnim(void) = 0;
    virtual void setSprinting(bool) = 0;
    virtual int getDeathSound(void) = 0;
    virtual float getSpeed(void) = 0;
    virtual void setSpeed(float) = 0;
    virtual void hurtEffects(class ActorDamageSource const&, float, bool, bool) = 0;
    virtual void aiStep(void) = 0;
    virtual void pushActors(void) = 0;
    virtual void checkSpawnRules(bool) = 0;
    virtual void checkSpawnObstruction(void) = 0;
    virtual int getItemUseDuration(void) = 0;
    virtual int getItemUseStartupProgress(void) = 0;
    virtual int getItemUseIntervalProgress(void) = 0;
    virtual int getMaxHeadXRot(void) = 0;
    virtual bool isAlliedTo(Actor*) = 0;
    virtual void doHurtTarget(Actor*, uintptr_t ActorDamageCause) = 0;
    virtual int getArmorValue(void) = 0;
    virtual void hurtArmorSlots() = 0;
    virtual void setDamagedArmor(int, ItemStack const&) = 0;
    virtual void sendArmorDamage() = 0;
    virtual void sendArmor() = 0;
    virtual class Item* getAllHand(void) = 0;
    virtual class Item* getAllEquipment(void) = 0;
    virtual void dropEquipmentOnDeath(uintptr_t ActorDamageSource, int) = 0;
    virtual void dropEquipmentOnDeath(void) = 0;
    virtual void clearVanishEnchantedItemsOnDeath(void) = 0;
    virtual void sendInventory(bool) = 0;
    virtual float getDamageAfterEnchantReduction(int ActorDamageSource, float) = 0;
    virtual void createAIGoals(void) = 0;
    virtual void onBorn(Actor&, Actor&) = 0;
    virtual void setItemSlot(int, ItemStack const&) = 0;
    virtual void setTransitioningSitting(bool) = 0;
    virtual void _getWalkTargetValue(BlockPos const&) = 0;
    virtual void canExistWhenDisallowMob(void) = 0;
    virtual void initBodyControl(void) = 0;
    virtual void newServerAiStep(void) = 0;
    virtual void tickDeath(void) = 0;
    virtual void _getAdjustedAABBForSpawnCheck(AABB const&, struct Vector3 const&) = 0;
    virtual void prepareRegion(class ChunkSource&) = 0;
    virtual void destroyRegion(void) = 0;
    virtual void suspendRegion(void) = 0;
    virtual void _fireDimensionChanged(void) = 0;
    virtual void changeDimensionWithCredits(Dimension, int) = 0;
    virtual void tickWorld(class Tick const&) = 0;
    virtual void Function331() = 0;
    virtual int getTickingOffsets(void) = 0;
    virtual void moveView(void) = 0;
    virtual void moveSpawnView(struct Vector3 const&, Dimension, int) = 0;
    virtual void checkMovementStats(struct Vector3 const&) = 0;
    virtual int getCurrentStructureFeature(void) = 0;
    virtual bool isAutoJumpEnabled(void) = 0;
    virtual void respawn(void) = 0;
    virtual void resetRot(void) = 0;
    virtual bool isInTrialMode(void) = 0;
    virtual void openPortfolio(void) = 0;
    virtual void openBook(int, bool, int, class BlockActor*) = 0;
    virtual void openTrading(uint64_t const&, bool) = 0;
    virtual void openChalkboard(class ChalkboardBlockActor&, bool) = 0;
    virtual void openNpcInteractScreen(std::shared_ptr<class INpcDialogueData>) = 0;
    virtual void openInventory(void) = 0;
    virtual void displayChatMessage(std::string const&, std::string const&) = 0;
    virtual void displayClientMessage(std::string const&) = 0;
    virtual void displayTextObjectMessage(uintptr_t TextObjectRoot, const std::string&, const std::string&) = 0;
    virtual void displayTextObjectWhisperMessage(uintptr_t ResolvedTextObject, const std::string&, const std::string&) = 0;
    virtual void displayTextObjectWhisperMessage(std::string const&, std::string const&, std::string const&) = 0;
    virtual void displayWhisperMessage(const std::string&, const std::string&, const std::string&, const std::string&) = 0;
    virtual void startSleepInBed(BlockPos const&) = 0;
    virtual void stopSleepInBed(bool, bool) = 0;
    virtual bool canStartSleepInBed(void) = 0;
    virtual void openSign(BlockPos const&) = 0;
    virtual void playEmote(const std::string&) = 0;
    virtual bool isLoading(void) = 0;
    virtual bool isPlayerInitialized(void) = 0;
    virtual void stopLoading(void) = 0;
    virtual void setPlayerGameType(int GameType) = 0;
    virtual void initHUDContainerManager(void) = 0;
    virtual void _crit(Actor&) = 0;
    virtual int getEventing(void) = 0;
    virtual int getUserId(void) = 0;
    virtual void addExperience(int) = 0;
    virtual void addLevels(int) = 0;
    virtual void Function232() = 0;
    virtual void Function233() = 0;
    virtual void Function234() = 0;
    virtual void deleteContainerManager(void) = 0;
    virtual bool isActorRelevant(Actor const&) = 0;
    virtual void Function237() = 0;
    virtual void onSuspension(void) = 0;
    virtual void onLinkedSlotsChanged(void) = 0;
    virtual void Function240() = 0;
    virtual void Function241() = 0;
    virtual void sendNetworkPacket(class Packet&) = 0;
    virtual void Function243() = 0;
    virtual bool isSimulated(void) = 0;
    virtual void* _getXuid(std::string&) = 0;
    virtual void getMovementSettings(void) = 0;
    virtual void requestMissingSubChunk(class SubChunkPos const&) = 0;
    virtual void getMaxChunkBuildRadius(void) = 0;
    virtual void onMovePlayerPacketNormal(Vector3 const&, Vector2 const&, float) = 0;
    virtual void _createChunkSource(ChunkSource&) = 0;
    virtual void setAbilities(class LayeredAbilities const&) = 0;
    virtual void Function252() = 0;
    virtual void Function253() = 0;

    ItemStack* GetItemInHand();
    std::string GetPlatform();
    std::string GetPlatform(BuildPlatform platform);
    std::string GetEntityId();
    int64_t GetRuntimeID();
    bool isCollidingHorizontal();
    bool isOnGround();
    void setOnGround(bool flag);
    float GetHeightOffset();
    Vector3 GetLerpedPos(float deltaTime = 0);
    class AABB GetAABBForPos(Vector3 pos);
    uintptr_t GetvTableFunction(int index);
    void SelectSlot(int slot);
    void addChatMessage(std::string Message, bool autoReplaceColorCodes = true);
    void addErrorMessage(std::string Message, bool autoReplaceColorCodes = true);
    void addChatMessage(std::string title, std::string Message, TextUtils::ChatColor color, bool autoReplaceColorCodes = true);
    void addChatMessage(std::string title, std::string Message, bool autoReplaceColorCodes = true, bool setEntireTitle = false);
    void addCustomChatMessage(std::string Message, bool autoReplaceColorCodes = false);
    float getDistance(Vector3 c1, Vector3 c2);
    uintptr_t** vTable();
    int getSwordSlot(PlayerInventoryProxy* supplies);
    AABB GetAABB();
    AABB GetLerpedAABB();
    void SetAABB(AABB aabb);
    bool canSee(Actor* actor);
    AttributesComponent* getAttributesComponent();
    //AttributeInstance* getAttribute(Attribute *attribute);
    AttributeInstance* getAttribute(int id);
    AttributeInstance* getAttribute(AttributeId id);
    float getMaxHealth();
    float getHealth();
    float getAbsorption();
    float getMaxAbsorption();
    void displayActionBarMessage(std::string msg, bool autoReplaceColorCodes, int fadeIn = 1, int fadeOut = 1, int duration = 3);
    std::string _getNameTag();
    void setNameTag(const std::string& name);
    SimpleContainer* getArmorContainer();
    class ItemStack* getArmor(ArmorSlot slot);
    void sendInput();
    std::shared_ptr<PlayerAuthInputPacket> getInputPacket();
    void setPosition(Vector3 pos);
    void sendPosition();

    // Component-related functions
    bool isPlayer();
    bool isLocalPlayer();
    bool isImmobile();
    void setImmobile(bool flag);
    std::string getXuid();
    std::string getHashedXuid();
    Vector3* getPos();
    Vector3* getPosPrev();
    AABBShapeComponent* GetAABBShapeComponent();
    JumpControlComponent* GetJumpComponent();
    MoveInputComponent* GetMoveInputComponent();
    MobEffectsComponent* GetMobEffectsComponent();
    RuntimeIDComponent* getRuntimeIDComponent();
    Vector3* GetRenderPositionComponent();
    ActorHeadRotationComponent *GetActorHeadRotationComponent();
    StateVectorComponent *GetStateVectorComponent();
    ActorRotationComponent* GetActorRotationComponent();
    MobBodyRotationComponent* GetMobBodyRotationComponent();
    MaxAutoStepComponent* GetMaxAutoStepComponent();
    InventoryTransactionManager* GetInventoryTransactionManager();
    Vector3 GetClosestAABBPoint();
    Vector3 GetClosestAABBPoint(Vector3 pos);
    void applyTurnDelta(Vector2 rots);
    float GetFallDistance();
    void SetFallDistance(float distance);
    bool wasOnGround();
    void setWasOnGround(bool flag);
    void jumpFromGround();
    float PredictNextMotion(float val);
    Vector3 PredictNextMotion(Vector3 val);
    bool IsStuckInCollider();
    float GetDistanceToGround();
    float GetGroundY();
    void AddEffectClientside(int effectDurationTicks, unsigned int effectId, int amplifier, bool particles = true);
    std::shared_ptr<InventoryTransactionPacket> MakeAttackTransaction(Actor* actor);


};

class ItemActor : public Actor
{
public:
    ItemStack mItem;          // this+Actor+0
    int       mAge;           // this+Actor+160
    int       mPickupDelay;   // this+Actor+164
    int       mThrowTime;     // this+Actor+168
    float     mBobOffs;       // this+Actor+172
    int       mHealth;        // this+Actor+176
    int       mLifeTime;      // this+Actor+180
    bool      mIsInItemFrame; // this+Actor+184
    bool      mIsFromFishing; // this+Actor+185
};