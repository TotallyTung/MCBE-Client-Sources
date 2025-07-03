//
// Created by vastrakai on 11/8/2024.
//

#include "InfiniteAura.hpp"

#include <Features/Modules/Movement/AutoPath.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/Network/MinecraftPackets.hpp>
#include <SDK/Minecraft/Network/Packets/MovePlayerPacket.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <Hook/Hooks/NetworkHooks/PacketSendHook.hpp>
#include <SDK/Minecraft/World/HitResult.hpp>
#include <SDK/Minecraft/Network/MinecraftPackets.hpp>
#include <SDK/Minecraft/Inventory/PlayerInventory.hpp>
#include <SDK/Minecraft/Network/LoopbackPacketSender.hpp>

void InfiniteAura::onEnable()
{
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &InfiniteAura::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->listen<PacketOutEvent, &InfiniteAura::onPacketOutEvent, nes::event_priority::ABSOLUTE_LAST>(this);
    gFeatureManager->mDispatcher->listen<PacketInEvent, &InfiniteAura::onPacketInEvent>(this);
    gFeatureManager->mDispatcher->listen<RenderEvent, &InfiniteAura::onRenderEvent>(this);
}

void InfiniteAura::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &InfiniteAura::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketOutEvent, &InfiniteAura::onPacketOutEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketInEvent, &InfiniteAura::onPacketInEvent>(this);
    gFeatureManager->mDispatcher->deafen<RenderEvent, &InfiniteAura::onRenderEvent>(this);
    mHasTarget = false;
    mNeedsToPathBack = false;
}

std::shared_ptr<MovePlayerPacket> InfiniteAura::createPacketForPos(glm::vec3 pos)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    auto packet = MinecraftPackets::createPacket<MovePlayerPacket>();
    packet->mPos = pos;
    packet->mPlayerID = player->getRuntimeID();
    packet->mRot = { mRots.x, mRots.y };
    packet->mYHeadRot = mRots.z;
    packet->mResetPosition = PositionMode::Teleport;
    packet->mOnGround = true;
    packet->mRidingID = -1;
    packet->mCause = TeleportationCause::Unknown;
    packet->mSourceEntityType = ActorType::Player;
    packet->mTick = 0;

    return packet;
}

std::vector<std::shared_ptr<MovePlayerPacket>> InfiniteAura::pathToPos(glm::vec3 from, glm::vec3 to)
{
    auto ci = ClientInstance::get();
    auto player = ci->getLocalPlayer();
    if (!player) return {};


    float blocksPerPacket = mBlocksPerPacket.mValue;
    glm::vec3 currentPos = from;

    std::vector<std::shared_ptr<MovePlayerPacket>> packetsToSend;

    auto path = AutoPath::findFlightPathGlm(from, to, ci->getBlockSource(), 1.f, true, 1000, false);

    // For each point in the path, step to it in blocksPerPacket increments
    for (size_t i = 0; i < path.size(); i++)
    {
        auto nextPos = path[i];
        auto diff = nextPos - currentPos;
        auto diffLen = glm::length(diff);
        auto diffNorm = glm::normalize(diff);

        while (diffLen > blocksPerPacket)
        {
            auto newPos = currentPos + diffNorm * blocksPerPacket;
            packetsToSend.push_back(createPacketForPos(newPos));
            currentPos = newPos;
            diff = nextPos - currentPos;
            diffLen = glm::length(diff);
            diffNorm = glm::normalize(diff);
        }
    }

    // Add the final packet
    packetsToSend.push_back(createPacketForPos(to));

    auto positions = packetsToSend | std::views::transform([](auto packet) -> glm::vec3 { return packet->mPos; });
    mPacketPositions = std::vector(positions.begin(), positions.end());
    mLastPathTime = NOW;

    return packetsToSend;
}


void InfiniteAura::onBaseTickEvent(BaseTickEvent& event)
{
    std::lock_guard<std::mutex> lock(mMutex);
    auto player = event.mActor; // Local player
    if (!player) return;
    auto supplies = player->getSupplies();
    auto bs = ClientInstance::get()->getBlockSource();

    if (mNeedsToPathBack)
    {
        auto packetsBack = pathToPos(mLastPos, *player->getPos());
        for (auto packet : packetsBack)
        {
            ClientInstance::get()->getPacketSender()->sendToServer(packet.get());
        }
        mNeedsToPathBack = false;
        return;
    }

    auto actors = ActorUtils::getActorList(true, true);

    static std::unordered_map<Actor*, int64_t> lastAttacks = {};

    float aps = mAPS.mValue;
    uint64_t delay = 1000 / aps;

    if (NOW - mLastAttack < delay) return;

    for (auto it = actors.begin(); it != actors.end();)
    {
        Actor* actor = *it;
        if (!actor || !actor->isValid()) {
            ++it;
            continue;
        }

        auto hitResult = bs->checkRayTrace(*player->getPos(), *actor->getPos() - PLAYER_HEIGHT_VEC, player);
        if (hitResult.mType == HitType::BLOCK && mRayTrace.mValue)
        {
            it = actors.erase(it);
        }
        else
        {
            ++it;
        }
    }

    if (mMode.mValue == Mode::Switch)
    {
        std::ranges::sort(actors, [&](Actor* a, Actor* b) -> bool
        {
            return lastAttacks[a] < lastAttacks[b];
        });
    } else if (mMode.mValue == Mode::Single)
    {
        std::ranges::sort(actors, [&](Actor* a, Actor* b) -> bool
        {
            return a->distanceTo(player) < b->distanceTo(player);
        });
    }

    bool targetFound = false;

    for (auto actor : actors)
    {
        if (actor == player) continue;
        if (!actor) continue;
        if (!actor->isValid()) continue;

        if (actor->distanceTo(player) > mRange.mValue) continue;

        auto packets = pathToPos(*player->getPos(), *actor->getPos());
        for (auto packet : packets)
        {
            ClientInstance::get()->getPacketSender()->sendToServer(packet.get());
        }

        player->swing();
        std::shared_ptr<InventoryTransactionPacket> attackTransaction = ActorUtils::createAttackTransaction(actor, supplies->mSelectedSlot);
        ClientInstance::get()->getPacketSender()->sendToServer(attackTransaction.get());
        lastAttacks[actor] = NOW;

        mLastPos = *actor->getPos();
        mNeedsToPathBack = true;
        mLastAttack = NOW;

        targetFound = true;

        if (mMode.mValue == Mode::Single || mMode.mValue == Mode::Switch)
            break;
    }

    mHasTarget = targetFound;

};

void InfiniteAura::onPacketOutEvent(PacketOutEvent& event)
{
    if (event.mPacket->getId() == PacketID::MovePlayer)
    {
        auto packet = event.getPacket<MovePlayerPacket>();
        mRots = { packet->mRot.x, packet->mRot.y, packet->mYHeadRot };
        if (mHasTarget && packet->mResetPosition != PositionMode::Teleport)
        {
            event.cancel();
        }
    }
}

void InfiniteAura::onPacketInEvent(PacketInEvent& event)
{
    if (event.mPacket->getId() == PacketID::MovePlayer && mSilentAccept.mValue)
    {
        auto player = ClientInstance::get()->getLocalPlayer();
        if (!player) return;

        auto packet = event.getPacket<MovePlayerPacket>();
        //if (packet->mResetPosition != PositionMode::Teleport) return;
        if (packet->mPlayerID != player->getRuntimeID()) return;

        // reset packet and cancel event
        event.cancel();
        ClientInstance::get()->getPacketSender()->sendToServer(packet.get());
        ChatUtils::displayClientMessage("attempting to silently accept teleport [mResetPosition: {}]", std::string(magic_enum::enum_name(packet->mResetPosition)));
    }
}

void InfiniteAura::onRenderEvent(RenderEvent& event)
{
    float alphaMultiplier = 1.0f;
    uint64_t fadeTime = 500;
    uint64_t currentTime = NOW;

    if (mLastPathTime + fadeTime < currentTime)
    {
        mPacketPositions.clear();
    }

    if (mLastPathTime + fadeTime > currentTime)
    {
        alphaMultiplier = 1.0f - static_cast<float>(currentTime - mLastPathTime) / fadeTime;
        alphaMultiplier = std::clamp(alphaMultiplier, 0.0f, 1.0f);
    }

    if (mRenderMode.mValue == RenderMode::Lines)
    {
        auto ci = ClientInstance::get();
        auto player = ci->getLocalPlayer();
        if (!player) return;

        std::lock_guard<std::mutex> lock(mMutex);

        auto drawList = ImGui::GetBackgroundDrawList();

        std::vector<ImVec2> points;

        for (auto pos : mPacketPositions)
        {
            ImVec2 point;
            if (!RenderUtils::worldToScreen(pos, point)) continue;
            points.emplace_back(point);
        }

        // Connect each point to form a line
        if (!points.empty())
            for (int i = 0; i < points.size() - 1; i++)
            {
                drawList->AddLine(points[i], points[i + 1], IM_COL32(255, 0, 0, 255 * alphaMultiplier), 2.0f);
            }
    } else
    {
        auto player = ClientInstance::get()->getLocalPlayer();
        if (!player) return;

        std::lock_guard<std::mutex> lock(mMutex);

        for (auto pos : mPacketPositions)
        {
            auto drawList = ImGui::GetBackgroundDrawList();

            AABB aabb = AABB(pos, glm::vec3(0.2f, 0.2f, 0.2f));
            auto points = MathUtils::getImBoxPoints(aabb);

            drawList->AddConvexPolyFilled(points.data(), points.size(), IM_COL32(255, 0, 0, 100 * alphaMultiplier));
            drawList->AddPolyline(points.data(), points.size(), IM_COL32(255, 0, 0, 255 * alphaMultiplier), true, 2.f);
        }
    }
}

