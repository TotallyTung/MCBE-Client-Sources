//
// Created by vastrakai on 7/8/2024.
//

#include "Aura.hpp"

#include <Features/FeatureManager.hpp>
#include <Features/Events/BaseTickEvent.hpp>
#include <Features/Events/BobHurtEvent.hpp>
#include <Features/Events/BoneRenderEvent.hpp>
#include <Features/Events/PacketInEvent.hpp>
#include <Features/Events/PacketOutEvent.hpp>
#include <Features/Events/RenderEvent.hpp>
#include <Features/Modules/Misc/Friends.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Options.hpp>
#include <Utils/GameUtils/ActorUtils.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/Actor/GameMode.hpp>
#include <SDK/Minecraft/World/Level.hpp>
#include <SDK/Minecraft/World/HitResult.hpp>
#include <SDK/Minecraft/Inventory/PlayerInventory.hpp>
#include <SDK/Minecraft/Network/LoopbackPacketSender.hpp>
#include <SDK/Minecraft/Network/Packets/MovePlayerPacket.hpp>
#include <SDK/Minecraft/Network/Packets/PlayerAuthInputPacket.hpp>
#include <SDK/Minecraft/Network/Packets/RemoveActorPacket.hpp>
#include <SDK/Minecraft/Rendering/GuiData.hpp>

int Aura::getSword(Actor* target) {
    auto player = ClientInstance::get()->getLocalPlayer();
    auto supplies = player->getSupplies();
    auto container = supplies->getContainer();

    if (gFriendManager->isFriend(target) && mFistFriends.mValue)
    {
        // Look for a TROPICAL_FISH in the hotbar
        int fishSlot = -1;
        for (int i = 0; i < 36; i++)
        {
            if (mHotbarOnly.mValue && i > 8) break;
            auto item = container->getItem(i);
            if (!item->mItem) continue;
            if (item->getItem()->mItemId == 267)
            {
                fishSlot = i;
                break;
            }
        }

        if (fishSlot != -1)
        {
            return fishSlot;
        }

        // Find a empty sot, OR an innert item
        for (int i = 0; i < 36; i++)
        {
            if (mHotbarOnly.mValue && i > 8) break;
            auto item = container->getItem(i);
            if (!item->mItem || item->getItem()->mItemId == 0)
            {
                return i;
            }
        }

        for (int i = 0; i < 36; i++)
        {
            if (mHotbarOnly.mValue && i > 8) break;
            auto item = container->getItem(i);
            if (item->mItem && !ItemUtils::hasItemType(item))
            {
                return i;
            }
        }

        return player->getSupplies()->mSelectedSlot;
    }

    int bestSword = ItemUtils::getBestItem(SItemType::Sword, mHotbarOnly.mValue);

    if (shouldUseFireSword(target))
    {
        return ItemUtils::getFireSword(mHotbarOnly.mValue);
    }

    return bestSword;
}

bool Aura::shouldUseFireSword(Actor* target)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    auto supplies = player->getSupplies();
    auto container = supplies->getContainer();

    int fireSw = ItemUtils::getFireSword(mHotbarOnly.mValue);
#ifdef __PRIVATE_BUILD__ //anyway doesnt bypass without spoof
    if (fireSw != -1 && mAutoFireSword.mValue && !target->isOnFire())
    {
        return true;
    }
    else
    {
        return false;
    }
#else
    return false;
#endif
}

void Aura::onEnable()
{
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &Aura::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->listen<PacketOutEvent, &Aura::onPacketOutEvent>(this);
    gFeatureManager->mDispatcher->listen<PacketInEvent, &Aura::onPacketInEvent>(this);
    gFeatureManager->mDispatcher->listen<RenderEvent, &Aura::onRenderEvent>(this);
    gFeatureManager->mDispatcher->listen<BobHurtEvent, &Aura::onBobHurtEvent, nes::event_priority::FIRST>(this);
    gFeatureManager->mDispatcher->listen<BoneRenderEvent, &Aura::onBoneRenderEvent, nes::event_priority::FIRST>(this);

    if (mThirdPerson.mValue && !mThirdPersonOnlyOnAttack.mValue) ClientInstance::get()->getOptions()->mThirdPerson->value = 1;
}

bool chargingBow = false;

void Aura::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &Aura::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketOutEvent, &Aura::onPacketOutEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketInEvent, &Aura::onPacketInEvent>(this);
    gFeatureManager->mDispatcher->deafen<RenderEvent, &Aura::onRenderEvent>(this);
    gFeatureManager->mDispatcher->deafen<BobHurtEvent, &Aura::onBobHurtEvent>(this);
    sHasTarget = false;
    sTarget = nullptr;
    mRotating = false;

    if (mThirdPerson.mValue && !mThirdPersonOnlyOnAttack.mValue) ClientInstance::get()->getOptions()->mThirdPerson->value = 0;
}

void Aura::rotate(Actor* target)
{
    if (mRotateMode.mValue == RotateMode::None) return;
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    mTargetedAABB = target->getAABB();
    mRotating = true;
}

void Aura::shootBow(Actor* target)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

#ifdef __PRIVATE_BUILD__
    if (!mAutoBow.mValue) return;
#else
    return;
#endif

    int bowSlot = -1;
    int arrowSlot = -1;
    for (int i = 0; i < 36; i++)
    {
        auto item = player->getSupplies()->getContainer()->getItem(i);
        if (!item->mItem) continue;
        if (item->getItem()->mName.contains("bow"))
        {
            if (mHotbarOnly.mValue && i > 8) continue;
            bowSlot = i;
        }
        if (item->getItem()->mName.contains("arrow"))
        {
            arrowSlot = i;
        }
        if (bowSlot != -1 && arrowSlot != -1) break;
    }

    if (bowSlot == -1 || arrowSlot == -1) return;

    static int useTicks = 0;
    constexpr int maxUseTicks = 17;

    if (useTicks == 0)
    {
        spdlog::info("Starting to use bow");
        player->getSupplies()->getContainer()->startUsingItem(bowSlot);
        chargingBow = true;
        useTicks++;
    }
    else if (useTicks < maxUseTicks)
    {
        useTicks++;
    }
    else if (useTicks >= maxUseTicks)
    {
        spdlog::info("Releasing bow");
        rotate(target);
        player->getSupplies()->getContainer()->releaseUsingItem(bowSlot);
        chargingBow = false;
        useTicks = 0;
    }
}

void Aura::throwProjectiles(Actor* target)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    static uint64_t lastThrow = 0;
    int64_t throwDelay = mThrowDelay.mValue * 50.f;

    if (NOW - lastThrow < throwDelay) return;

    int snowballSlot = -1;
    if (mThrowProjectiles.mValue)
        for (int i = 0; i < 36; i++)
        {
            auto item = player->getSupplies()->getContainer()->getItem(i);
            if (!item->mItem) continue;
            if (item->getItem()->mName.contains("snowball"))
            {
                snowballSlot = i;
                break;
            }
        }

    if (mHotbarOnly.mValue && snowballSlot > 8) return;

    if (snowballSlot == -1)
    {
        return;
    }

    int oldSlot = player->getSupplies()->mSelectedSlot;
    player->getSupplies()->mSelectedSlot = snowballSlot;
    player->getGameMode()->baseUseItem(player->getSupplies()->getContainer()->getItem(snowballSlot));
    player->getSupplies()->mSelectedSlot = oldSlot;

    lastThrow = NOW;

}

float EaseInOutExpo(float pct)
{
    if (pct < 0.5f) {
        return (pow(2.f, 16.f * pct) - 1.f) / 510.f;
    }
    else {
        return 1.f - 0.5f * pow(2.f, -16.f * (pct - 0.5f));
    }
}

void Aura::onRenderEvent(RenderEvent& event)
{
    if (mAPSMin.mValue < 0) mAPSMin.mValue = 0;
    if (mAPSMax.mValue < mAPSMin.mValue + 1) mAPSMax.mValue = mAPSMin.mValue + 1;

    if (mStrafe.mValue)
    {
        auto player = ClientInstance::get()->getLocalPlayer();
        if (!player) return;

        if (mRotating)
        {
            glm::vec2 rots = MathUtils::getRots(*player->getPos(), mTargetedAABB);
            auto rot = player->getActorRotationComponent();
            rot->mPitch = rots.x;
            rot->mYaw = rots.y;
            rot->mOldPitch = rots.x;
            rot->mOldYaw = rots.y;

        }
    }

    if (mVisuals.mValue) {
        auto player = ClientInstance::get()->getLocalPlayer();
        if (!player) return;

        auto actor = Aura::sTarget;

        // try to call isPlayer in the try_call
        if (!TRY_CALL([&]() { bool isPlayer = actor->isPlayer(); }))
        {
            Aura::sTarget = nullptr;
            Aura::sHasTarget = false;
            return;
        }

        if (!actor || !actor->isPlayer()) return;

        auto playerPos = player->getRenderPositionComponent()->mPosition;
        auto actorPos = actor->getRenderPositionComponent()->mPosition;
        auto state = actor->getStateVectorComponent();
        auto shape = player->getAABBShapeComponent();

        glm::vec3 pos = actorPos - glm::vec3(0.f, 1.62f, 0.f);
        glm::vec3 pos2 = state->mPos - glm::vec3(0.f, 1.62f, 0.f);
        glm::vec3 posOld = state->mPosOld - glm::vec3(0.f, 1.62f, 0.f);
        pos = posOld + (pos2 - posOld) * ImGui::GetIO().DeltaTime;

        float hitboxWidth = shape->mWidth;
        float hitboxHeight = shape->mHeight;

        glm::vec3 aabbMin = glm::vec3(pos.x - hitboxWidth / 2, pos.y, pos.z - hitboxWidth / 2);
        glm::vec3 aabbMax = glm::vec3(pos.x + hitboxWidth / 2, pos.y + hitboxHeight, pos.z + hitboxWidth / 2);

        aabbMin = aabbMin - glm::vec3(0.1f, 0.1f, 0.1f);
        aabbMax = aabbMax + glm::vec3(0.1f, 0.1f, 0.1f);

        float distance = glm::distance(playerPos, actorPos) + 2.5f;
        if (distance < 0) distance = 0;

        float scaledSphereSize = 1.0f / distance * 100.0f * mSpheresSizeMultiplier.mValue;
        if (scaledSphereSize < 1.0f) scaledSphereSize = 1.0f;
        if (scaledSphereSize < mSpheresMinSize.mValue) scaledSphereSize = mSpheresMinSize.mValue;

        glm::vec3 bottomOfHitbox = aabbMin;
        glm::vec3 topOfHitbox = aabbMax;
        bottomOfHitbox.x = pos.x;
        bottomOfHitbox.z = pos.z;
        topOfHitbox.x = pos.x;
        topOfHitbox.z = pos.z;
        topOfHitbox.y += 0.1f;

        static float pct = 0.f;
        static bool reversed = false;
        static uint64_t lastTime = NOW;

        float speed = mUpDownSpeed.mValue;
        uint64_t visualTime = 800 / (speed - 0.2);

        if (NOW - lastTime > visualTime) {
            reversed = !reversed;
            lastTime = NOW;
            pct = reversed ? 1.f : 0.f;
        }

        pct += !reversed ? (speed * ImGui::GetIO().DeltaTime) : -(speed * ImGui::GetIO().DeltaTime);
        pct = MathUtils::lerp(0.f, 1.f, pct);
        pos = MathUtils::lerp(bottomOfHitbox, topOfHitbox, EaseInOutExpo(pct));

        auto corrected = RenderUtils::transform.mMatrix;

        glm::vec2 screenPos = {0, 0};

        static float angleOffset = 0.f;
        angleOffset += (mUpDownSpeed.mValue * 30.f) * ImGui::GetIO().DeltaTime;
        float radius = (float)mSpheresRadius.mValue;

        for (int i = 0; i < mSpheresAmount.mValue; i++) {
            float angle = (i / (float)mSpheresAmount.mValue) * 360.f;
            angle += angleOffset;
            angle = MathUtils::wrap(angle, -180.f, 180.f);

            float rad = angle * (PI / 180.0f);

            float x = pos.x + radius * cosf(rad);
            float y = pos.y;
            float z = pos.z + radius * sinf(rad);

            glm::vec3 thisPos = { x, y, z };

            if (!corrected.OWorldToScreen(
                    RenderUtils::transform.mOrigin,
                    thisPos, screenPos, MathUtils::fov,
                    ClientInstance::get()->getGuiData()->mResolution))
                continue;

            ImColor color = ColorUtils::getThemedColor(0);
            ImColor glowColor = color;
            glowColor.Value.w = 0.3f;

            ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(screenPos.x, screenPos.y), scaledSphereSize * 1.5f, glowColor);
            ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(screenPos.x, screenPos.y), scaledSphereSize, color);
        }
    }

}

void Aura::onBaseTickEvent(BaseTickEvent& event)
{
    auto player = event.mActor; // Local player
    if (!player) return;
    if(player->getHealth() <= 0) return; // kai plz add actor->isAlive() thankz
    auto supplies = player->getSupplies();

    auto actors = ActorUtils::getActorList(false, true);
    static std::unordered_map<Actor*, int64_t> lastAttacks = {};
    bool isMoving = Keyboard::isUsingMoveKeys();

    // Sort actors by lastAttack if mode is switch
    if (mMode.mValue == Mode::Switch)
    {
        std::ranges::sort(actors, [&](Actor* a, Actor* b) -> bool
        {
            return lastAttacks[a] < lastAttacks[b];
        });
    }
    else if (mMode.mValue == Mode::Multi)
    {
        // Sort actors by distance if mode is multi
        std::ranges::sort(actors, [&](Actor* a, Actor* b) -> bool
        {
            return a->distanceTo(player) < b->distanceTo(player);
        });
    }

    static int64_t lastAttack = 0;
    int64_t now = NOW;
    float aps = mAPS.mValue;
    if (mRandomizeAPS.mValue)
    {
        // Validate min and max APS
        if (mAPSMin.mValue < 0) mAPSMin.mValue = 0;
        if (mAPSMax.mValue < mAPSMin.mValue + 1) mAPSMax.mValue = mAPSMin.mValue + 1;

        aps = MathUtils::random(mAPSMin.mValue, mAPSMax.mValue);
    }
    int64_t delay = 1000 / aps;

    bool foundAttackable = false;

    for (auto actor : actors)
    {
        if (actor == player) continue;
        float range = mDynamicRange.mValue && !isMoving ? mDynamicRangeValue.mValue : mRange.mValue;
        if (actor->distanceTo(player) > range) continue;
        if (!mAttackThroughWalls.mValue && !player->canSee(actor)) continue;

        if (actor->isPlayer() && gFriendManager->mEnabled)
        {
            if (gFriendManager->isFriend(actor) && !mFistFriends.mValue)
            {
                continue;
            }
        }

        if (mRotateMode.mValue == RotateMode::Normal)
            rotate(actor);
        foundAttackable = true;
        sTarget = actor;
        sTargetRuntimeID = actor->getRuntimeID();

        throwProjectiles(actor);
        shootBow(actor);

        if (now - lastAttack < delay) break;

        if (mSwing.mValue)
        {
            if (mSwingDelay.mValue)
            {
                if (now - mLastSwing >= mSwingDelayValue.mValue * 1000)
                {
                    player->swing();
                }
            }
            else player->swing();
        }
        int slot = -1;
        int bestWeapon = getSword(actor);
        mHotbarOnly.mValue ? slot = bestWeapon : slot = player->getSupplies()->mSelectedSlot;

        auto ogActor = actor;
        actor = findObstructingActor(player, actor);

        if (mSwitchMode.mValue == SwitchMode::Full)
        {
            supplies->mSelectedSlot = bestWeapon;
        }

        if (mRotateMode.mValue == RotateMode::Flick)
        {
            rotate(actor);
        }

        player->getLevel()->getHitResult()->mType = HitType::ENTITY;


        if (mAttackMode.mValue == AttackMode::Synched)
        {

            std::shared_ptr<InventoryTransactionPacket> attackTransaction = ActorUtils::createAttackTransaction(actor, mSwitchMode.mValue == SwitchMode::Spoof ? bestWeapon : -1);

            bool shouldUseFire = shouldUseFireSword(ogActor) && mLastTransaction + 200 < NOW;
            bool spoofed = false;
            int oldSlot = mLastSlot;

#ifdef __PRIVATE_BUILD__
            if (mFireSwordSpoof.mValue && shouldUseFire)
            {
                spoofed = true;
                auto pkt = PacketUtils::createMobEquipmentPacket(bestWeapon);
                PacketUtils::queueSend(pkt, true);
            }
#endif

            PacketUtils::queueSend(attackTransaction, false);

            if (spoofed)
            {
                auto pkt = PacketUtils::createMobEquipmentPacket(oldSlot);
                PacketUtils::queueSend(pkt, false);
            }
        } else {
            int oldSlot = supplies->mSelectedSlot;
            if (mSwitchMode.mValue == SwitchMode::Spoof)
            {
                supplies->mSelectedSlot = bestWeapon;
            }

            bool shouldUseFire = shouldUseFireSword(ogActor) && mLastTransaction + 200 < NOW;
            bool spoofed = false;
            int oldPktSlot = mLastSlot;

#ifdef __PRIVATE_BUILD__
            if (mFireSwordSpoof.mValue && shouldUseFire)
            {
                auto pkt = PacketUtils::createMobEquipmentPacket(bestWeapon);
                ClientInstance::get()->getPacketSender()->sendToServer(pkt.get());
                spoofed = true;
            }
#endif

            player->getGameMode()->attack(actor);
            supplies->mSelectedSlot = oldSlot;

            if (spoofed)
            {
                auto pkt = PacketUtils::createMobEquipmentPacket(oldPktSlot);
                ClientInstance::get()->getPacketSender()->send(pkt.get());
            }
        }

        lastAttack = now;
        lastAttacks[actor] = now;
        if (mMode.mValue == Mode::Single || mMode.mValue == Mode::Switch) break;
    }

    if (!foundAttackable)
    {
        mRotating = false;
        sTarget = nullptr;
    }
    sHasTarget = foundAttackable;

    if (mThirdPerson.mValue && mThirdPersonOnlyOnAttack.mValue && sHasTarget) {
        if (!mIsThirdPerson) {
            ClientInstance::get()->getOptions()->mThirdPerson->value = 1;
            mIsThirdPerson = true;
        }
    }
    else if (mThirdPerson.mValue && mThirdPersonOnlyOnAttack.mValue && !sHasTarget) {
        if (mIsThirdPerson) {
            ClientInstance::get()->getOptions()->mThirdPerson->value = 0;
            mIsThirdPerson = false;
        }
    }
}

void Aura::onPacketOutEvent(PacketOutEvent& event)
{
    if (!mRotating) return;

    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    if (event.mPacket->getId() == PacketID::PlayerAuthInput) {
        auto pkt = event.getPacket<PlayerAuthInputPacket>();
        glm::vec2 rots = MathUtils::getRots(*player->getPos(), mTargetedAABB);
        pkt->mRot = rots;
        pkt->mYHeadRot = rots.y;
        if (mHeadYaw.mValue) pkt->mYHeadRot += 90.0f;
        if (mRotateMode.mValue == RotateMode::Flick) mRotating = false;
    } else if (event.mPacket->getId() == PacketID::MovePlayer) {
        auto pkt = event.getPacket<MovePlayerPacket>();
        glm::vec2 rots = MathUtils::getRots(*player->getPos(), mTargetedAABB);
        pkt->mRot = rots;
        pkt->mYHeadRot = rots.y;
        if (mHeadYaw.mValue) pkt->mYHeadRot += 90.0f;
    } else if (event.mPacket->getId() == PacketID::Animate)
    {
        mLastSwing = NOW;
    } else if (event.mPacket->getId() == PacketID::InventoryTransaction)
    {
        auto pkt = event.getPacket<InventoryTransactionPacket>();
        auto cit = pkt->mTransaction.get();

        if (cit->type == ComplexInventoryTransaction::Type::ItemUseTransaction)
        {
            const auto iut = reinterpret_cast<ItemUseInventoryTransaction*>(cit);
            if (iut->mActionType == ItemUseInventoryTransaction::ActionType::Place)
                mLastTransaction = NOW;
        }

        if (cit->type == ComplexInventoryTransaction::Type::ItemUseOnEntityTransaction)
        {
            const auto iut = reinterpret_cast<ItemUseOnActorInventoryTransaction*>(cit);
            if (iut->mActionType == ItemUseOnActorInventoryTransaction::ActionType::Attack)
            {
                spdlog::info("clickpos: {}/{}/{}", iut->mClickPos.x, iut->mClickPos.y, iut->mClickPos.z);
            }
        }
    } else if (event.mPacket->getId() == PacketID::MobEquipment)
    {
        auto pkt = event.getPacket<MobEquipmentPacket>();
        mLastSlot = pkt->mSlot;
    }

}

void Aura::onPacketInEvent(PacketInEvent& event)
{
    if (event.mPacket->getId() == PacketID::RemoveActor)
    {
        auto packet = event.getPacket<RemoveActorPacket>();
        if (sTarget && sTargetRuntimeID == packet->mRuntimeID)
        {
            spdlog::critical("Active target was removed from world!! This may lead to a crash!");
            sHasTarget = false;
            sTarget = nullptr;
        }
    }

    if (event.mPacket->getId() == PacketID::ChangeDimension)
    {
        if (mDisableOnDimensionChange.mValue)
        {
            this->setEnabled(false);
        }
    }
}

void Aura::onBobHurtEvent(BobHurtEvent& event)
{
    if (sHasTarget)
    {
        event.mDoBlockAnimation = true;
    }
}

void Aura::onBoneRenderEvent(BoneRenderEvent& event)
{
    if (sHasTarget)
    {
        event.mDoBlockAnimation = true;
    }
}

Actor* Aura::findObstructingActor(Actor* player, Actor* target)
{
    if (mBypassMode.mValue == BypassMode::None) return target;

    bool isMoving = Keyboard::isUsingMoveKeys();
    auto actors = ActorUtils::getActorList(false, false);
    std::ranges::sort(actors, [&](Actor* a, Actor* b) -> bool
    {
        return a->distanceTo(player) < b->distanceTo(player);
    });

    if (mBypassMode.mValue == BypassMode::Raycast)
    {
        std::map<Actor*, AABBShapeComponent> actorHitboxes;
        glm::vec3 rayStart = *player->getPos();
        glm::vec3 rayEnd = *target->getPos();
        glm::vec3 currentRayPos = rayStart;

        // Check for obstructing actors
        for (auto actor : actors)
        {
            if (actor == player || actor == target) continue;
            float range = mDynamicRange.mValue && !isMoving ? mDynamicRangeValue.mValue : mRange.mValue;
            if (actor->distanceTo(player) > range) continue;

            auto hitbox = *actor->getAABBShapeComponent();
            actorHitboxes[actor] = hitbox;

            if ((hitbox.mWidth != 0.86f || hitbox.mHeight != 2.32f) && actor->mEntityIdentifier == "hivecommon:shadow")
            {
                continue;
            }

            if (MathUtils::rayIntersectsAABB(currentRayPos, rayEnd, hitbox.mMin, hitbox.mMax))
            {
                if (mDebug.mValue)
                {
                    spdlog::info("Found obstructing actor: {}", actor->mEntityIdentifier);
                    float hbWidth = hitbox.mWidth;
                    float hbHeight = hitbox.mHeight;
                    float distFromPlayer = actor->distanceTo(player);
                    ChatUtils::displayClientMessage("Attacking obstructing actor: " + actor->mEntityIdentifier + " | Width: " + std::to_string(hbWidth) + " | Height: " + std::to_string(hbHeight) + " | Distance: " + std::to_string(distFromPlayer));
                }
                target = actor;
                break;
            }
        }

        return target;
    } else if (mBypassMode.mValue != BypassMode::None)
    {
        auto actors = ActorUtils::getActorList(false, false);
        std::ranges::sort(actors, [&](Actor* a, Actor* b) -> bool
        {
            return a->distanceTo(player) < b->distanceTo(player);
        });

        for (auto actor : actors)
        {
            if (actor == player || actor == target) continue;
            float distance = actor->distanceTo(target);
            if (distance > 3.f) continue;

            std::string id = actor->mEntityIdentifier;
            float hitboxWidth = actor->getAABBShapeComponent()->mWidth;
            float hitboxHeight = actor->getAABBShapeComponent()->mHeight;

            if (id == "hivecommon:shadow" && distance < 3.f && mBypassMode.mValue == BypassMode::FlareonV2)
            {
                if (hitboxWidth != 0.86f || hitboxHeight != 2.32f) // Identify Correct Shadow
                {
                    continue;
                }

                if (mDebug.mValue)
                {
                    spdlog::info("Found shadow: {}", actor->mEntityIdentifier);
                    ChatUtils::displayClientMessage("Found shadow: " + actor->mEntityIdentifier);
                }


                return actor;
            }
        }

        return target;
    }

    return target;
}