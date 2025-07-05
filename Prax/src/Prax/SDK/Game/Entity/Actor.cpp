//
// Created by vastrakai on 10/10/2023.
//
#include "Actor.h"

#include <Prax/Hook/Hooks/PacketHandler.h>
#include <Prax/Hook/Hooks/PacketSender.h>

#include "Components/FlagComponents.h"
#include <Prax/Utils/System/Memory.h>
#include <Prax/SDK/Game/Entity/Components/FallDistanceComponent.h>
#include <Prax/SDK/Game/Network/Other/InventoryTransactionPacket.h>
#include <Prax/SDK/Game/Network/Other/SetTitlePacket.h>
#include <Prax/SDK/Game/Network/Other/MovePlayerPacket.h>
#include <Prax/SDK/Game/Network/Other/PlayerAuthInputPacket.h>
#include <Prax/Utils/Math.h>

#include "Components/ActorEquipmentComponent.h"
#include "Components/ActorTypeComponent.h"
#include "Components/DepenetrationComponent.h"
#include "Components/MobEffectsComponent.h"
#include "Prax/SDK/Game/Network/Other/MobEffectPacket.h"


ItemStack *Actor::GetItemInHand() {
    return supplies->GetItemAtSlot(supplies->selectedHotbarSlot);
}

std::string Actor::GetPlatform() {
    try {
        BuildPlatform platform = (BuildPlatform) buildPlatform;
        return BuildPlatformNames.at(platform);
    } catch (std::out_of_range) {
        return "Unknown";
    }
    return "";
}

std::string Actor::GetPlatform(BuildPlatform platform) {
    try {
        return BuildPlatformNames.at(platform);
    } catch (std::out_of_range) {
        return "Unknown";
    }
    return "";
}

std::string Actor::GetEntityId() {
    // return the string from 0x270 (std::string, not a ptr)
    std::string bruh = entityIdString;
    if (bruh.find(".") != std::string::npos) {
        bruh.erase(bruh.find("."), bruh.length());
    }
    return bruh;
}

int64_t Actor::GetRuntimeID() {
    return getRuntimeIDComponent()->runtimeID;
}



float Actor::GetHeightOffset() {
    if (isPlayer()) {
        return 1.62f;
    }
    return 0.f;
}

Vector3 Actor::GetLerpedPos(float deltaTime) {
    auto lp = Minecraft::ClientInstance->ThePlayer();
    if (this != lp) return *GetRenderPositionComponent();

    deltaTime = Minecraft::ClientInstance->GetDeltaTime();
    auto stateVector = GetStateVectorComponent();

    Vector3 pos = stateVector->pos - Vector3(0.f, 1.62f, 0.f);
    Vector3 posOld = stateVector->posOld - Vector3(0.f, 1.62f, 0.f);
    if (!isPlayer()) {
        pos = stateVector->pos;
        posOld = stateVector->posOld;
    }

    // Interpolate the position so it's smoother
    pos = posOld + (pos - posOld) * deltaTime;

    return pos;
}

class AABB Actor::GetAABBForPos(Vector3 pos) {
    auto aabbShapeComponent = GetAABBShapeComponent();
    auto stateVector = GetStateVectorComponent();

    if (!isPlayer()) {
        pos = stateVector->pos;
    }

    // Interpolate the position so it's smoother

    float hitboxWidth = aabbShapeComponent->hitboxWidth;
    float hitboxHeight = aabbShapeComponent->hitboxHeight;
    // since the pos is in the center of the hitbox
    Vector3 aabbMin = Vector3(pos.x - hitboxWidth / 2, pos.y, pos.z - hitboxWidth / 2);
    Vector3 aabbMax = Vector3(pos.x + hitboxWidth / 2, pos.y + hitboxHeight, pos.z + hitboxWidth / 2);


    // Slightly increase the size of the AABB so it matches the actual hitbox
    aabbMin = aabbMin - Vector3(0.1f, 0.1f, 0.1f);
    aabbMax = aabbMax + Vector3(0.1f, 0.1f, 0.1f);



    AABB aabb = AABB(aabbMin, aabbMax, true);

    return aabb;

}

uintptr_t Actor::GetvTableFunction(int index) {
    // Get the address of the teleport function from the vtable
    uintptr_t* vtable = *(uintptr_t**)this;
    // Each address in the vtable is 8 bytes, so we multiply the index by 8 to get the offset
    uintptr_t func = vtable[index];

    return func;
}

void Actor::SelectSlot(int slot) {
    supplies->selectedHotbarSlot = slot;
}

void Actor::addChatMessage(std::string Message, bool autoReplaceColorCodes) {
    if (this != nullptr) {
        std::string sendText = TextUtils::GetColor(TextUtils::Bold) + TextUtils::GetColor(TextUtils::DarkPurple) + "[" + Prax::ClientName + "] "
                               + TextUtils::GetColor(TextUtils::Bold) + TextUtils::GetColor(TextUtils::Reset) + Message;
        if(autoReplaceColorCodes) sendText = TextUtils::AddColorsToString(sendText);


        // call at vtable index 357
        //Memory::CallVFunc<357, void>(this, sendText);
        displayClientMessage(sendText);

    }
}

void Actor::addErrorMessage(std::string Message, bool autoReplaceColorCodes) {
    if (this != nullptr) {
        std::string sendText = TextUtils::GetColor(TextUtils::Bold) + TextUtils::GetColor(TextUtils::Red) + "[Error] "
                               + TextUtils::GetColor(TextUtils::Bold) + TextUtils::GetColor(TextUtils::Reset) + Message;
        if (autoReplaceColorCodes) sendText = TextUtils::AddColorsToString(sendText);
        displayClientMessage(sendText);
    }
}

void Actor::addChatMessage(std::string title, std::string Message, TextUtils::ChatColor color, bool autoReplaceColorCodes) {
    if (this != nullptr) {
        std::string sendText = TextUtils::GetColor(TextUtils::Bold) + TextUtils::GetColor(color) + "[" + title + "] "
                               + TextUtils::GetColor(TextUtils::Bold) + TextUtils::GetColor(TextUtils::Reset) + Message;
        if (autoReplaceColorCodes) sendText = TextUtils::AddColorsToString(sendText);
        displayClientMessage(sendText);
    }
}

void Actor::addChatMessage(std::string title, std::string Message, bool autoReplaceColorCodes, bool setEntireTitle) {
    if (this != nullptr)
    {
        std::string sendText;
        if (!setEntireTitle) sendText = "§l§f[" + title + "] §l§r" + Message;
        else sendText = title + "§r" + Message;

        if (autoReplaceColorCodes) sendText = TextUtils::AddColorsToString(sendText);
        displayClientMessage(sendText);
    }
}


void Actor::addCustomChatMessage(std::string Message, bool autoReplaceColorCodes) {
    if (this != nullptr) {
        if (autoReplaceColorCodes) Message = TextUtils::AddColorsToString(Message);
        displayClientMessage(Message);
    }
}

float Actor::getDistance(Vector3 c1, Vector3 c2) {
    float dx = abs(c2.x - c1.x);
    float dy = abs(c2.y - c1.y);
    float dz = abs(c2.z - c1.z);

    return dx + dy + dz;
}

uintptr_t **Actor::vTable() {
    if (this == nullptr) {
        return 0x0;
    }
    return *reinterpret_cast<uintptr_t***>(reinterpret_cast<__int64>(this) + 0x0);
}

int Actor::getSwordSlot(PlayerInventoryProxy *supplies) {
    Inventory* inv = supplies->inventory;
    for (int i = 0; i < 9; i++)
    {
        ItemStack* stack = inv->getItem(i);
        if (stack->item != NULL)
        {
            Item* item = *stack->item;
            if (item != nullptr && item->IsSword())
                return i;
        }
    }

    return -1;
}

AABB Actor::GetAABB() {
    auto aabbShapeComponent = this->GetAABBShapeComponent();
    const auto aabb = AABB(Vector3(aabbShapeComponent->AAX, aabbShapeComponent->AAY, aabbShapeComponent->AAZ), Vector3(aabbShapeComponent->BBX, aabbShapeComponent->BBY, aabbShapeComponent->BBZ), true);
    return aabb;
}

AABB Actor::GetLerpedAABB()
{
    Vector3 pos = *this->GetRenderPositionComponent() - Vector3(0.f, 1.62f, 0.f);
    if (!this->isPlayer())
    {
        pos = *this->GetRenderPositionComponent();
    }

    if (isLocalPlayer()){
        Vector3 pos2 = this->GetStateVectorComponent()->pos - Vector3(0.f, 1.62f, 0.f);
        Vector3 posOld = this->GetStateVectorComponent()->posOld - Vector3(0.f, 1.62f, 0.f);
        if (!this->isPlayer())
        {
            pos2 = this->GetStateVectorComponent()->pos;
            posOld = this->GetStateVectorComponent()->posOld;
        }

        pos = posOld + (pos2 - posOld) * Minecraft::ClientInstance->GetDeltaTime();
    }

    float hitboxWidth = this->GetAABBShapeComponent()->hitboxWidth;
    float hitboxHeight = this->GetAABBShapeComponent()->hitboxHeight;
    // since the pos is in the center of the hitbox
    Vector3 aabbMin = Vector3(pos.x - hitboxWidth / 2, pos.y, pos.z - hitboxWidth / 2);
    Vector3 aabbMax = Vector3(pos.x + hitboxWidth / 2, pos.y + hitboxHeight, pos.z + hitboxWidth / 2);

    // Slightly increase the size of the AABB so it matches the actual hitbox
    aabbMin = aabbMin - Vector3(0.1f, 0.1f, 0.1f);
    aabbMax = aabbMax + Vector3(0.1f, 0.1f, 0.1f);


    return AABB(aabbMin, aabbMax, true);
}

void Actor::SetAABB(AABB aabb) {
    auto aabbShapeComponent = this->GetAABBShapeComponent();
    aabbShapeComponent->AAX = aabb.lower.x;
    aabbShapeComponent->AAY = aabb.lower.y;
    aabbShapeComponent->AAZ = aabb.lower.z;
    aabbShapeComponent->BBX = aabb.upper.x;
    aabbShapeComponent->BBY = aabb.upper.y;
    aabbShapeComponent->BBZ = aabb.upper.z;
}



Vector3 *Actor::GetRenderPositionComponent() {
    RenderPositionComponent* renderPositionComponent = this->entityCtx.getComponent<RenderPositionComponent>();
    if (renderPositionComponent == nullptr) {
        return nullptr;
    }
    return &renderPositionComponent->Position;
}

void Actor::jumpFromGround()
{
    bool noJumpDelay = GetJumpComponent()->noJumpDelay;
    GetJumpComponent()->noJumpDelay = false;
    static auto func = reinterpret_cast<void*>(SigManager::GetSignatureAddress(SignatureEnum::SimulatedPlayer_simulateJump));
    Memory::CallFastcall<void>(func, this);
    GetJumpComponent()->noJumpDelay = noJumpDelay;
}

bool Actor::isOnGround() {
    auto storage = assure_for<OnGroundFlagComponent>(this->entityCtx.registry);
    return storage->contains(this->entityCtx.entityId);
}

bool Actor::isCollidingHorizontal() {
    auto storage = assure_for<FlagComponent<HorizontalCollisionFlag>>(this->entityCtx.registry);
    return storage->contains(this->entityCtx.entityId);
}

void Actor::setOnGround(bool flag){
    // Fuck you mojang
    using bruh = void(__fastcall*)(EntityContext const& ctx, bool flag);
    static auto func = reinterpret_cast<bruh>(Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::ActorCollision_setOnGround)));
    if (!func) return;
    return func(entityCtx, flag);
}

MoveInputComponent *Actor::GetMoveInputComponent() {
    return entityCtx.getComponent<MoveInputComponent>();
}

MobEffectsComponent* Actor::GetMobEffectsComponent()
{
    return entityCtx.getComponent<MobEffectsComponent>();
}

JumpControlComponent *Actor::GetJumpComponent() {
    //using getJumpComponent = JumpControlComponent* (__cdecl*)(void*, EntityId*);
    static auto func = reinterpret_cast<void*>(Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::Mob_getJumpComponent)));
    auto registryBase = reinterpret_cast<void*>(entityCtx.registry);
    auto id = this->entityCtx.entityId;
    return Memory::CallFastcall<JumpControlComponent*>(func, registryBase, &id);
}

ActorHeadRotationComponent* Actor::GetActorHeadRotationComponent() {
    // Add the component if its not there
    return entityCtx.getComponent<ActorHeadRotationComponent>();
}

float Actor::GetFallDistance() {
    return entityCtx.getComponent<FallDistanceComponent>()->fallDistance;
}

void Actor::SetFallDistance(float distance) {
    entityCtx.getComponent<FallDistanceComponent>()->fallDistance = distance;
}

RuntimeIDComponent* Actor::getRuntimeIDComponent() {
    return entityCtx.getComponent<RuntimeIDComponent>();
}

AABBShapeComponent *Actor::GetAABBShapeComponent() {
    return this->entityCtx.getComponent<AABBShapeComponent>();
}

StateVectorComponent* Actor::GetStateVectorComponent() {
    return this->entityCtx.getComponent<StateVectorComponent>();
}

ActorRotationComponent *Actor::GetActorRotationComponent() {
    return this->entityCtx.getComponent<ActorRotationComponent>();
}

MobBodyRotationComponent* Actor::GetMobBodyRotationComponent()
{
    return this->entityCtx.getComponent<MobBodyRotationComponent>();
}

MaxAutoStepComponent* Actor::GetMaxAutoStepComponent()
{
    return this->entityCtx.getComponent<MaxAutoStepComponent>();
}

InventoryTransactionManager* Actor::GetInventoryTransactionManager()
{
    // transaction manager is not a pointer. the start of it is at this+0xEE8
    return reinterpret_cast<InventoryTransactionManager*>(reinterpret_cast<uintptr_t>(this) + 0xEE8);
}

Vector3 Actor::GetClosestAABBPoint()
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    return GetClosestAABBPoint(*player->getPos());
}

Vector3 Actor::GetClosestAABBPoint(Vector3 pos)
{
    auto aabb = GetAABB();
    return aabb.GetClosestPoint(pos);
}

void Actor::applyTurnDelta(Vector2 rots)
{
    static auto func = reinterpret_cast<void*>(SigManager::GetSignatureAddress(SignatureEnum::Player_applyTurnDelta));
    Memory::CallFastcall<void>(func, this, &rots);
}

float Actor::PredictNextMotion(float val) {
    return (val - 0.08f) * 0.98f;
}

Vector3 Actor::PredictNextMotion(Vector3 val) {
    return {PredictNextMotion(val.x), PredictNextMotion(val.y), PredictNextMotion(val.z)};
}

bool Actor::IsStuckInCollider()
{
    return entityCtx.getComponent<DepenetrationComponent>()->isPenetrating();
}

float Actor::GetDistanceToGround()
{
    // Get the closest block below the player
    Vector3 pos = *this->getPos();
    Vector3 blockPos = Vector3(floor(pos.x), floor(pos.y), floor(pos.z));
    Block* block = Minecraft::ClientInstance->GetBlockSource()->_getBlock(blockPos);

    // If the block isn't air, then we are on the ground
    if (block->blockLegacy->id != 0) {
        return 0.f;
    }

    return 1.f;


}

float Actor::GetGroundY()
{
    // Get the closest block below the player
    /*Vector3 pos = *this->getPos();
    Vector3 blockPos = Vector3(floor(pos.x), floor(pos.y), floor(pos.z));
    Block* block = Minecraft::ClientInstance->GetBlockSource()->_getBlock(blockPos);

    // If the block isn't air, then we are on the ground
    if (block->blockLegacy->id != 0) {
        return blockPos.y;
    }*/

    for (float y = getPos()->y; y > -64.f; y -= 1.f) {
        Vector3 pos = *getPos();
        pos.y = y;
        Vector3 blockPos = Vector3(floor(pos.x), floor(pos.y), floor(pos.z));
        Block* block = Minecraft::ClientInstance->GetBlockSource()->_getBlock(blockPos);

        if (block->blockLegacy->id != 0) {
            return blockPos.y;
            break;
        }
    }

    return -64.f;
}

void Actor::AddEffectClientside(int effectDurationTicks, unsigned int effectId, int amplifier, bool particles)
{
    auto packet = MinecraftPackets::CreatePacket<MobEffectPacket>();
    packet->mRuntimeId = GetRuntimeID();
    packet->mEffectDurationTicks = effectDurationTicks;
    packet->mEffectId = (EffectType)effectId;
    packet->mEffectAmplifier = amplifier;
    packet->mShowParticles = particles;
    packet->mEventId = MobEffectPacket::Event::Add;
    PacketHandler::HandlePacket(packet);

}

std::shared_ptr<InventoryTransactionPacket> Actor::MakeAttackTransaction(Actor* actor)
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    auto pkt = MinecraftPackets::CreatePacket<InventoryTransactionPacket>();

    auto cit = std::make_unique<ItemUseOnActorInventoryTransaction>();
    cit->slot = player->supplies->selectedHotbarSlot;
    cit->itemInHand = NetworkItemStackDescriptor(*player->GetItemInHand());
    cit->actorId = actor->entityCtx.getComponent<ActorUniqueIDComponent>()->uniqueID;
    cit->actionType = ItemUseOnActorInventoryTransaction::ActionType::Attack;
    cit->clickPos = actor->GetAABB().GetClosestPoint(*player->getPos());
    cit->playerPos = *player->getPos();

    pkt->mTransaction = std::move(cit);

    return pkt;
}

Vector3 *Actor::getPos() {
    return &GetStateVectorComponent()->pos;
}

Vector3 *Actor::getPosPrev() {
    return &GetStateVectorComponent()->posOld;
}

bool Actor::canSee(Actor *actor) {
    using canSee = bool(__fastcall*)(Actor*, Actor*);
    static uintptr_t func = Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::Actor_canSee));
    return reinterpret_cast<canSee>(func)(this, actor);
}

AttributesComponent* Actor::getAttributesComponent() {
    return entityCtx.getComponent<AttributesComponent>();
}

//AttributeInstance* Actor::getAttribute(Attribute* attribute) {
//    using getAttribute = AttributeInstance* (__fastcall*)(Actor*, Attribute*);
//    static uintptr_t func = Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::Actor_getAttribute));
//    return reinterpret_cast<getAttribute>(func)(this, attribute);
//}


float Actor::getMaxHealth()
{
    return getAttribute(AttributeId::Health)->maximumValue;
}

float Actor::getHealth()
{
    return getAttribute(AttributeId::Health)->currentValue;
}

float Actor::getAbsorption()
{
    return getAttribute(AttributeId::Absorption)->currentValue;
}

float Actor::getMaxAbsorption()
{
    return getAttribute(AttributeId::Absorption)->maximumValue;
}

AttributeInstance* Actor::getAttribute(AttributeId id)
{
    return getAttribute((int)id);
}

AttributeInstance* Actor::getAttribute(int id) {
    // Directly access the map
    auto& map = getAttributesComponent()->baseAttributeMap.attributes;
    auto it = map.find(id);
    if (it != map.end()) {
        return &it->second;
    }
    return nullptr;
}

std::string Actor::_getNameTag() {
    using getNameTag = std::string*(__fastcall*)(void*);
    static uintptr_t func = Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::Actor_getNameTag));
    return *reinterpret_cast<getNameTag>(func)(this);
}

void Actor::setNameTag(const std::string& name) {
    using setNameTag = void(__fastcall*)(Actor*, std::string);
    static uintptr_t func = SigManager::GetSignatureAddress(SignatureEnum::Actor_setNameTag);
    reinterpret_cast<setNameTag>(func)(this, name);
}

SimpleContainer* Actor::getArmorContainer() {
    return entityCtx.getComponent<ActorEquipmentComponent>()->mArmorContainer;
}

class ItemStack *Actor::getArmor(ArmorSlot slot) {
    return getArmorContainer()->getItem(slot);
}

void Actor::sendInput() {
    auto inputPacket = getInputPacket();
    bool canc = false;
    PacketSender::HandleAuthInputTick(inputPacket.get(), &canc);
    Minecraft::ClientInstance->packetSender->sendToServer(inputPacket.get());
}

std::shared_ptr<PlayerAuthInputPacket> Actor::getInputPacket() {
    auto paip = MinecraftPackets::CreatePacket<PlayerAuthInputPacket>();
    auto rotComponent = this->GetActorRotationComponent();
    Vector2 rots = Vector2(rotComponent->pitch, rotComponent->yaw);
    paip->rotation = rots;
    auto actorHeadRotations = GetActorHeadRotationComponent();
    paip->headYaw = actorHeadRotations->headRot;
    paip->position = *this->getPos();
    paip->inputMode = AuthInputMode::MOUSE;
    paip->clientPlayMode = ClientPlayMode::SCREEN;
    paip->interactionModel = AuthInteractionModel::TOUCH;
    paip->ticksAlive = -1;

    return paip;
}

void Actor::setPosition(Vector3 pos) {
    static void* func = reinterpret_cast<void*>(Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::Actor_setPosition)));
    Memory::CallFastcall<void>(func, this, &pos);
}

bool Actor::wasOnGround() {
    auto storage = assure_for<FlagComponent<WasOnGroundFlag>>(this->entityCtx.registry);
    return storage->contains(this->entityCtx.entityId);
}

void Actor::setWasOnGround(bool flag) {
    if (flag) getOrAddComponent<FlagComponent<WasOnGroundFlag>>();
    else removeComponent<FlagComponent<WasOnGroundFlag>>();
}

void Actor::sendPosition() {
    std::shared_ptr<MovePlayerPacket> mpp = MinecraftPackets::CreatePacket<MovePlayerPacket>();
    mpp->mode = 0;
    mpp->onGround = isOnGround();
    mpp->position = *getPos();
    mpp->tick = 0;
    mpp->rotation = { GetActorRotationComponent()->pitch, GetActorRotationComponent()->yaw };
    mpp->headYaw = mpp->rotation.y;
    mpp->actorRuntimeID = GetRuntimeID();
    Minecraft::ClientInstance->packetSender->send(mpp.get());
}

bool Actor::isPlayer()
{
    //return this->entityCtx.getComponent<ActorTypeComponent>()->type == ActorType::Player;
    // use the flag component instead
    auto storage = assure_for<FlagComponent<PlayerComponentFlag>>(this->entityCtx.registry);
    return storage->contains(this->entityCtx.entityId);
}

bool Actor::isLocalPlayer() {
    auto storage = assure_for<FlagComponent<LocalPlayerComponentFlag>>(this->entityCtx.registry);
    return storage->contains(this->entityCtx.entityId);
}

bool Actor::isImmobile() {
    return getStatusFlag(ActorFlags::Noai);
}

void Actor::setImmobile(bool flag) {
    setStatusFlag(ActorFlags::Noai, flag);
}

std::string Actor::getXuid()
{
    std::string xuid = "";
    _getXuid(xuid);
    return xuid;
}

std::string Actor::getHashedXuid() {
    const char* xuid = getXuid().c_str();
    uint32_t hash = 2166136261U; // 32 bit offset_basis = 2166136261U

    for (uint32_t idx = 0; xuid[idx] != 0; ++idx) {
        // 32 bit FNV_prime = 224 + 28 + 0x93 = 16777619
        hash = 16777619U * (hash ^ static_cast<unsigned char>(xuid[idx]));
    }

    std::string xuidStr = String::ToLower(String::ToHex(hash));
    // If the xuidStr isnt 8 characters, add 0 to the beginning until it is
    while (xuidStr.length() < 8) {
        xuidStr = "0" + xuidStr;
    }

    return xuidStr;
}

void Actor::displayActionBarMessage(std::string msg, bool autoReplaceColorCodes, int fadeIn, int fadeOut, int duration) {
    if (autoReplaceColorCodes) msg = TextUtils::AddColorsToString(msg);
    auto pkt = SetTitlePacket::CreatePacket(SetTitlePacket::TitleType::Actionbar, msg, fadeIn, duration, fadeOut);
    pkt->mXuid = "";
    pkt->mPlatformOnlineId = localName;
    PacketHandler::HandlePacket(pkt);
}