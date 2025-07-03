// 7/28/2024
#include "OreMiner.hpp"

#include "Regen.hpp"

#include <Features/FeatureManager.hpp>
#include <Features/Events/BaseTickEvent.hpp>
#include <Features/Events/PacketOutEvent.hpp>
#include <Features/Events/PacketInEvent.hpp>
#include <Features/Modules/Visual/Interface.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Inventory/PlayerInventory.hpp>
#include <SDK/Minecraft/Actor/GameMode.hpp>
#include <SDK/Minecraft/Network/PacketID.hpp>
#include <SDK/Minecraft/Network/MinecraftPackets.hpp>
#include <SDK/Minecraft/Network/LoopbackPacketSender.hpp>
#include <SDK/Minecraft/Network/Packets/InventoryTransactionPacket.hpp>
#include <SDK/Minecraft/Network/Packets/PlayerAuthInputPacket.hpp>
#include <SDK/Minecraft/Network/Packets/MobEquipmentPacket.hpp>
#include <SDK/Minecraft/Network/Packets/AnimatePacket.hpp>
#include <SDK/Minecraft/World/BlockLegacy.hpp>
#include <SDK/Minecraft/World/Level.hpp>
#include <SDK/Minecraft/World/HitResult.hpp>
#include <Features/Modules/Player/ChestStealer.hpp>
#include <Features/Modules/Player/Scaffold.hpp>

void OreMiner::reset() {
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    GameMode* gm = player->getGameMode();

    if (mIsMiningBlock) {
        gm->stopDestroyBlock(mCurrentBlockPos);
    }
    mCurrentBlockPos = { INT_MAX, INT_MAX, INT_MAX };
    mTargettingBlockPos = { INT_MAX, INT_MAX, INT_MAX };
    mCurrentBlockFace = -1;
    mBreakingProgress = 0.f;
    mShouldSpoofSlot = true;
    mIsMiningBlock = false;
    mIsUncovering = false;
    mToolSlot = -1;
    mOffGround = false;
}

bool OreMiner::isValidBlock(glm::ivec3 blockPos, bool oreOnly, bool exposedOnly, bool usePriority, OrePriority priority) {
    auto player = ClientInstance::get()->getLocalPlayer();
    Block* block = ClientInstance::get()->getBlockSource()->getBlock(blockPos);
    if (!block) return false;

    // Air Check
    if (block->mLegacy->isAir()) return false;

    // BlockID Check
    int blockId = block->mLegacy->getBlockId();
    if (oreOnly) {
        bool IsOre = false;
        if (mEmerald && (!usePriority || mEmeraldPriority.mValue == priority) && isOre(mEmeraldIds, blockId)) IsOre = true;
        else if (mDiamond && (!usePriority || mDiamondPriority.mValue == priority) && isOre(mDiamondIds, blockId)) IsOre = true;
        else if (mGold && (!usePriority || mGoldPriority.mValue == priority) && isOre(mGoldIds, blockId)) IsOre = true;
        else if (mIron && (!usePriority || mIronPriority.mValue == priority) && isOre(mIronIds, blockId)) IsOre = true;
        else if (mCoal && (!usePriority || mCoalPriority.mValue == priority) && isOre(mCoalIds, blockId)) IsOre = true;
        else if (mRedstone && (!usePriority || mRedstonePriority.mValue == priority) && isOre(mRedstoneIds, blockId)) IsOre = true;
        else if (mLapis && (!usePriority || mLapisPriority.mValue == priority) && isOre(mLapisIds, blockId)) IsOre = true;

        if (!IsOre) return false;
    }

    // Distance Check
    AABB blockAABB = AABB(blockPos, glm::vec3(1, 1, 1));
    glm::vec3 closestPos = blockAABB.getClosestPoint(*player->getPos());
    if (mRange.mValue < glm::distance(closestPos, *player->getPos())) return false;

    // Exposed Check
    int exposedFace = BlockUtils::getExposedFace(blockPos);
    if (exposedOnly) {
        if (exposedFace == -1) return false;
    }

    return true;
}

void OreMiner::queueBlock(glm::ivec3 blockPos) {
    Block* block = ClientInstance::get()->getBlockSource()->getBlock(blockPos);
    mCurrentBlockPos = blockPos;
    mCurrentBlockFace = BlockUtils::getExposedFace(blockPos);
    if (mCurrentBlockFace == -1) mCurrentBlockFace = 0;
    mIsMiningBlock = true;
    mBreakingProgress = 0.f;
    int bestToolSlot = ItemUtils::getBestBreakingTool(block, mHotbarOnly.mValue);
    if (mShouldSpoofSlot) {
        PacketUtils::spoofSlot(bestToolSlot, false);
        mShouldSpoofSlot = false;
    }
    BlockUtils::startDestroyBlock(blockPos, mCurrentBlockFace);
    mToolSlot = bestToolSlot;
    mShouldSetbackSlot = true;

    if (mFlareonV2.mValue)
    {
        if (mSwingMode.mValue == SwingMode::Normal)
        {
            ClientInstance::get()->getLocalPlayer()->swing();
        }
        else {
            auto pkt = MinecraftPackets::createPacket<AnimatePacket>();
            pkt->mRuntimeID = ClientInstance::get()->getLocalPlayer()->getRuntimeID();
            pkt->mAction = Action::Swing;
            ClientInstance::get()->getPacketSender()->send(pkt.get());
        }
        if (mFirstRotation.mValue)
            mShouldRotate = true;
    }
}

OreMiner::PathFindResult OreMiner::getBestPathToBlock(glm::ivec3 blockPos) {
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return { glm::ivec3(INT_MAX, INT_MAX, INT_MAX), false };

    BlockSource* source = ClientInstance::get()->getBlockSource();
    if (!source) return { glm::ivec3(INT_MAX, INT_MAX, INT_MAX), false };

    if (mUncoverMode.mValue == UncoverMode::PathFind) {
        float closestDist = INT_MAX;
        glm::ivec3 closestBlockPos = glm::ivec3(INT_MAX, INT_MAX, INT_MAX);
        bool foundPath = false;
        std::vector<BlockInfo> blockList = BlockUtils::getBlockList(blockPos, mUncoverRange.mValue);
        for (auto& block : blockList) {
            if (!BlockUtils::isAirBlock(block.mPosition) && BlockUtils::getExposedFace(block.mPosition) != -1) {
                float distFromPlayer = glm::distance(*player->getPos(), glm::vec3(block.mPosition));
                float distFromBlock = glm::distance(glm::vec3(blockPos), glm::vec3(block.mPosition));
                float dist = distFromPlayer + distFromBlock;
                if (dist < closestDist) {
                    closestDist = dist;
                    closestBlockPos = block.mPosition;
                    foundPath = true;
                }
            }
        }
        return { closestBlockPos, foundPath };
    }
#ifdef __PRIVATE_BUILD__
    else if (mUncoverMode.mValue == UncoverMode::UnderGround) {
        for (int i = 0; i < mOffsetList.size(); i++) {
            glm::ivec3 currentPos = blockPos + mOffsetList[i];
            int id = source->getBlock(currentPos)->mLegacy->getBlockId();
            if (id == 73 || id == 74) {
                return { glm::ivec3(INT_MAX, INT_MAX, INT_MAX), false };
            }
        }
        return { blockPos, true };
    }
#endif

    return { glm::ivec3(INT_MAX, INT_MAX, INT_MAX), false };
}

bool OreMiner::isOre(std::vector<int> Ids, int id)
{
    return std::find(Ids.begin(), Ids.end(), id) != Ids.end();
}

void OreMiner::onEnable()
{
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &OreMiner::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->listen<PacketOutEvent, &OreMiner::onPacketOutEvent, nes::event_priority::VERY_LAST>(this);
    gFeatureManager->mDispatcher->listen<RenderEvent, &OreMiner::onRenderEvent>(this);

    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    mShouldRotate = false;
    mIsMiningBlock = false;
    reset();
}

void OreMiner::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &OreMiner::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketOutEvent, &OreMiner::onPacketOutEvent>(this);
    gFeatureManager->mDispatcher->deafen<RenderEvent, &OreMiner::onRenderEvent>(this);

    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    if (mIsMiningBlock) {
        player->getSupplies()->mSelectedSlot = mToolSlot;
        player->getGameMode()->stopDestroyBlock(mCurrentBlockPos);
        mIsMiningBlock = false;
    }
}

void OreMiner::onBaseTickEvent(BaseTickEvent& event)
{
    mWasMiningBlock = mIsMiningBlock;

    auto player = event.mActor;
    BlockSource* source = ClientInstance::get()->getBlockSource();
    if (!source) return;
    PlayerInventory* supplies = player->getSupplies();
    mPreviousSlot = supplies->getmSelectedSlot();

    auto chestStealer = gFeatureManager->mModuleManager->getModule<ChestStealer>();
    bool mStealing = chestStealer && chestStealer->mEnabled && chestStealer->mIsStealing;

    auto scaffold = gFeatureManager->mModuleManager->getModule<Scaffold>();
    bool isScaffold = scaffold && scaffold->mEnabled;

    int pickaxeSlot = ItemUtils::getBestItem(SItemType::Pickaxe, mHotbarOnly.mValue);
    ItemStack *stack = supplies->getContainer()->getItem(pickaxeSlot);
    bool hasPickaxe = stack->mItem && stack->getItem()->getItemType() == SItemType::Pickaxe;

    if (Regen::mIsMiningBlock || Regen::mWasMiningBlock || player->getStatusFlag(ActorFlags::Noai) || !hasPickaxe || player->isDestroying() || mStealing || isScaffold || (mFlareonV2.mValue && mOnGround.mValue && !player->isOnGround())) {
        reset();
        mShouldSetbackSlot = false;
        return;
    }

    if (isValidBlock(mCurrentBlockPos, (mUncoverMode.mValue == UncoverMode::None), !mIsUncovering)) { // Check if current block is valid
        if (mFlareonV2.mValue) {
            if (mSwingMode.mValue == SwingMode::Normal)
            {
                player->swing();
            }
            else {
                auto pkt = MinecraftPackets::createPacket<AnimatePacket>();
                pkt->mRuntimeID = player->getRuntimeID();
                pkt->mAction = Action::Swing;
                ClientInstance::get()->getPacketSender()->send(pkt.get());
            }
        }
        if (mFlareonV2.mValue) {
            if (mRotationMode.mValue == RotationMode::Normal) {
                mShouldRotate = true;
            }
        }
        Block* currentBlock = source->getBlock(mCurrentBlockPos);
        int exposedFace = BlockUtils::getExposedFace(mCurrentBlockPos);
        int bestToolSlot = ItemUtils::getBestBreakingTool(currentBlock, mHotbarOnly.mValue);
        if (mShouldSpoofSlot) {
            PacketUtils::spoofSlot(bestToolSlot, false);
            mShouldSpoofSlot = false;
            return;
        }
        mToolSlot = bestToolSlot;
        float destroySpeed = ItemUtils::getDestroySpeed(bestToolSlot, currentBlock);

        if (!player->isOnGround()) mOffGround = true;

        if (mCalcMode.mValue == CalcMode::Normal || (mOnGroundOnly.mValue && mOffGround)) {
            mCurrentDestroySpeed = mDestroySpeed.mValue;
        }
        else if (mCalcMode.mValue == CalcMode::Dynamic) {
            std::string blockName = currentBlock->getmLegacy()->getmName();
            bool found = false;
            for (auto& c : BlockUtils::mDynamicSpeeds) {
                if (c.blockName == blockName) {
                    mCurrentDestroySpeed = c.destroySpeed;
                    found = true;
                    break;
                }
            }
            if (!found) {
                mCurrentDestroySpeed = mDestroySpeed.mValue;
            }
        }

        mBreakingProgress += destroySpeed;

        if (mCurrentDestroySpeed <= mBreakingProgress) {
            mShouldRotate = true;
            supplies->mSelectedSlot = bestToolSlot;
            if (mSwing.mValue) player->swing();
            BlockUtils::destroyBlock(mCurrentBlockPos, exposedFace, mInfiniteDurability.mValue);
            supplies->mSelectedSlot = mPreviousSlot;
            mIsMiningBlock = false;
            return;
        }
    }
    else { // Find new block
        reset();
        std::vector<BlockInfo> blockList = BlockUtils::getBlockList(*player->getPos(), mRange.mValue);
        std::vector<BlockInfo> exposedBlockList;
        std::vector<BlockInfo> unexposedBlockList;

        for (int n = 0; n < 3; n++) {
            OrePriority priority = static_cast<OrePriority>(n);

            for (int i = 0; i < blockList.size(); i++) {
                if (isValidBlock(blockList[i].mPosition, true, false, true, priority)) {
                    if (BlockUtils::getExposedFace(blockList[i].mPosition) != -1) exposedBlockList.push_back(blockList[i]);
                    else unexposedBlockList.push_back(blockList[i]);
                }
                else continue;
            }

            glm::vec3 playerPos = *player->getPos();
            glm::ivec3 pos = { 0, 0, 0 };
            glm::ivec3 targettingPos = { 0, 0, 0 };
            if (!exposedBlockList.empty()) {
                float closestDistance = INT_MAX;
                for (int i = 0; i < exposedBlockList.size(); i++) {
                    glm::vec3 blockPos = exposedBlockList[i].mPosition;
                    float dist = glm::distance(playerPos, blockPos);
                    if (dist < closestDistance) {
                        closestDistance = dist;
                        pos = blockPos;
                        targettingPos = blockPos;
                    }
                }
                // queue block
                queueBlock(pos);
                mTargettingBlockPos = targettingPos;
                return;
            }
#ifdef __PRIVATE_BUILD__
            else if ((mUncoverMode.mValue == UncoverMode::PathFind || mUncoverMode.mValue == UncoverMode::UnderGround) && !unexposedBlockList.empty()) {
                float closestDistance = INT_MAX;
                for (int i = 0; i < unexposedBlockList.size(); i++) {
                    glm::vec3 blockPos = unexposedBlockList[i].mPosition;
                    float dist = glm::distance(playerPos, blockPos);
                    if (dist < closestDistance) {
                        closestDistance = dist;
                        pos = blockPos;
                        targettingPos = blockPos;
                    }
                }
                PathFindResult result = getBestPathToBlock(pos);
                if (result.foundPath) {
                    queueBlock(result.blockPos);
                    mIsUncovering = true;
                    mTargettingBlockPos = targettingPos;
                    return;
                }
            }
#else
            else if ((mUncoverMode.mValue == UncoverMode::PathFind) && !unexposedBlockList.empty()) {
                float closestDistance = INT_MAX;
                for (int i = 0; i < unexposedBlockList.size(); i++) {
                    glm::vec3 blockPos = unexposedBlockList[i].mPosition;
                    float dist = glm::distance(playerPos, blockPos);
                    if (dist < closestDistance) {
                        closestDistance = dist;
                        pos = blockPos;
                        targettingPos = blockPos;
                    }
                }
                PathFindResult result = getBestPathToBlock(pos);
                if (result.foundPath) {
                    queueBlock(result.blockPos);
                    mIsUncovering = true;
                    mTargettingBlockPos = targettingPos;
                    return;
                }
            }
#endif
        }

        if (mShouldSetbackSlot) {
            PacketUtils::spoofSlot(mPreviousSlot, false);
            mShouldSetbackSlot = false;
        }
    }
}

void OreMiner::onRenderEvent(RenderEvent& event)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    if (mRenderBlock.mValue) renderBlock();
}

void OreMiner::renderBlock()
{
    if (!mIsMiningBlock) return;

    auto player = ClientInstance::get()->getLocalPlayer();

    static float lastProgress = 0.f;
    float progress = 1.f;

    progress = mBreakingProgress;
    progress /= mCurrentDestroySpeed;
    if (progress < lastProgress) lastProgress = progress;
    progress = MathUtils::lerp(lastProgress, progress, ImGui::GetIO().DeltaTime * 30.f);
    lastProgress = progress;

    // clamp the progress to 0-1
    progress = MathUtils::clamp(progress, 0.f, 1.f);

    if (progress < 0.01f) return;

    auto size = glm::vec3(progress, progress, progress);
    glm::vec3 blockPos = mCurrentBlockPos;
    blockPos.x += 0.5f - (progress / 2.f);
    blockPos.y += 0.5f - (progress / 2.f);
    blockPos.z += 0.5f - (progress / 2.f);
    auto blockAABB = AABB(blockPos, size);

    static ImColor color = ImColor(255, 255, 0, 255);
    ImColor targetColor = ImColor(255, 255, 0, 255);

    if (mIsUncovering) targetColor = ImColor(255, 255, 0, 255);
    else targetColor = ImColor(0, 255, 0, 255);

    // lerping the color
    color = ImColor(MathUtils::lerp(color.Value, targetColor.Value, ImGui::GetIO().DeltaTime * 10.f));

    RenderUtils::drawOutlinedAABB(blockAABB, true, color);
};

void OreMiner::onPacketOutEvent(PacketOutEvent& event)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    if (event.mPacket->getId() == PacketID::PlayerAuthInput)
    {
        auto paip = event.getPacket<PlayerAuthInputPacket>();
        if (mShouldRotate)
        {
            const glm::vec3 blockPos = mCurrentBlockPos;
            auto blockAABB = AABB(blockPos, glm::vec3(1, 1, 1));
            glm::vec2 rotations = MathUtils::getRots(*player->getPos(), blockAABB);
            paip->mRot = rotations;
            paip->mYHeadRot = rotations.y;
            if (mFlareonV2.mValue && mHeadYaw.mValue) paip->mYHeadRot += 90.0f;
            mShouldRotate = false;
        }
    }
    else if (event.mPacket->getId() == PacketID::MobEquipment) {
        auto mpkt = event.getPacket<MobEquipmentPacket>();
        if (mpkt->mSlot == mToolSlot) mShouldSpoofSlot = false;
        else mShouldSpoofSlot = true;
    }
}