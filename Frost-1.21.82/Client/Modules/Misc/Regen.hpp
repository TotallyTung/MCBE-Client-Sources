#pragma once

class Regen : public Module
{
public:
    Regen() :
        Module("Regen", "Misc", "Automatically heals you.")
    {
        /*
        BoolSetting mOnCovered = BoolSetting("On Covered", "Send message on your ore got covered", false);
        BoolSetting mOnEnemyUncover = BoolSetting("On Enemy Uncover", "Send message on enemy uncovered ore", false);
        BoolSetting mPingCompensation = BoolSetting("Ping Compensation", "Use ping compensation", false);*/
        SliderSetting("Range", "The max range for destroying blocks", &mRange, 0, 10);
        SliderSetting("Destroy Speed", "The destroy speed for Regen", &mDestroySpeed, 0.01f, 1.f, SliderType::DoubleFloat);
        BoolSetting("Hotbar Only", "Only switch to tools in the hotbar", &mHotbarOnly);
        BoolSetting("Always Mine", "Keep mining ore", &mAlwaysMine);
        EnumSetting("Rotation Mode", "The rotation mode", { "Old", "Always", "Percentage" }, &mRotationMode);
        SliderSetting("Rotation Percentage", "The minium percentage to rotate", &mRotationPercentage, 0.01f, 1.f, SliderType::DoubleFloat);
        BoolSetting("Dynamic Rotation", "Rotate for same ticks as redstone if mining block can be broken faster than redstone", &mDynamicRotation);
        BoolSetting("Head Yaw", "Avoids movement check", &mHeadYaw);
        EnumSetting("Swing Mode", "The swing mode", { "Normal", "Silent" }, &mSwingMode);
        BoolSetting("Uncover", "Uncover redstone if nothing around you is already exposed", &mUncover);
        EnumSetting("Uncover Mode", "The uncover mode", { "Normal", "Under Ground" }, &mUncoverMode);
        BoolSetting("Dynamic Uncover", "Disables uncover if enemy is uncovering ores", &mDynamicUncover);
        BoolSetting("Transaction", "Use transaction to destroy block", &mTransaction);
        BoolSetting("Steal", "Steal the enemy's ore", &mSteal);
        SliderSetting("Stealer Timeout", "The max duration for stealer", &mStealerTimeout, 500, 5000, SliderType::Int);
        BoolSetting("Delayed", "Delay stealer to avoid detected by anti steal", &mDelayed);
        EnumSetting("Steal Mode", "The mode for steal", { "Normal", "Instant", }, &mStealMode);
        BoolSetting("Cover", "Covers the enemy's ore", &mCover);
        BoolSetting("Block Stealer", "Covers the mining ore if enemy is targettting same ore", &mBlockStealer);
        BoolSetting("Use Non Solid", "Use non solid block to cover ore if you have non solid block", &mUseNonSolid);
        BoolSetting("Confuse", "Confuse enemy's regen", &mConfuse);
        EnumSetting("Confuse Mode", "The confuser mode", { "Ore Blocker", "Stealer" }, &mConfuseMode);
        BoolSetting("Anti Steal", "Prevents enemy steal your ore", &mAntiSteal);
        EnumSetting("Anti Steal Mode", "The anti steal mode", { "Normal", "Smart" }, &mAntiStealMode);
        BoolSetting("Anti Confuse", "Prevents enemy confuse your regen", &mAntiConfuse);
        BoolSetting("Block Filter", "Apply advanced block filter for checking exposed block", &mBlockFilter);
        BoolSetting("Debug", "Send debug messages in chat", &mDebug);
        BoolSetting("On Steal", "Send message on steal", &mOnSteal);
        BoolSetting("On Cancel Steal", "Send message on cancel steal", &mOnCancelSteal);
        BoolSetting("On Cancel Uncover", "Send message on cancel uncover", &mOnCancelUncover);
        BoolSetting("On Cover", "Send message on cover", &mOnCover);
        BoolSetting("On Ghost Place", "Send message on failed covering", &mOnGhostPlace);
        BoolSetting("On Covered", "Send message on your ore got covered", &mOnCovered);
        BoolSetting("On Enemy Uncover", "Send message on enemy uncovered ore", &mOnEnemyUncover);
        BoolSetting("Ping Compensation", "Use ping compensation", &mPingCompensation);
    }
private:
    enum class Status {
        None,
        Mining,
        Uncovering,
        Stealing
    };

    // Settings
    /*enum UncoverMode
    {
        Normal = 0,
        UnderGround = 1,
    };
    enum StealMode 
    {
        Normal = 0,
        Instant = 1
    };
    enum SwingMode 
    {
        Normal = 0,
        Silent = 1
    };
    enum RotationMode 
    {
        Old = 0,
        Always = 1,
        Percent = 2
    };
    enum ConfuseMode 
    {
        OreBlocker = 0,
        Stealer = 1
    };
    enum AntiStealMode 
    {
        Normal = 0,
        Smart = 1
    };*/
    float mRange = 5.f;
    float mDestroySpeed = 1.f;
    bool mHotbarOnly = true;
    bool mAlwaysMine = false;
    int mRotationMode = 1;
    float mRotationPercentage = 0.4f;
    bool mDynamicRotation = false;
    bool mHeadYaw = false;
    int mSwingMode = 0;
    bool mUncover = false;
    int mUncoverMode = 0;
    bool mDynamicUncover = false;
    bool mTransaction = false;
    bool mSteal = false;
    float mStealerTimeout = 1500;
    bool mDelayed = false;
    int mStealMode = 0;
    bool mCover = false;
    bool mBlockStealer = true;
    bool mUseNonSolid = false;
    bool mConfuse = false;
    int mConfuseMode = 0;
    bool mAntiSteal = false;
    int mAntiStealMode = 0;
    bool mAntiConfuse = false;
    bool mBlockFilter = false;
    bool mDebug = false;
    bool mOnSteal = false;
    bool mOnCancelSteal = false;
    bool mOnCancelUncover = false;
    bool mOnCover = false;
    bool mOnGhostPlace = false;
    bool mOnCovered = false;
    bool mOnEnemyUncover = false;
    bool mPingCompensation = false;
    
    // Variables
    struct PathFindingResult 
    {
        Vector3<int> blockPos;
        float breakTime;
    };
    Status mCurrentStatus = Status::None;
    Vector3<int> mMiningPos = { INT_MAX, INT_MAX, INT_MAX };
    Vector3<int> mRedstonePos = { INT_MAX, INT_MAX, INT_MAX };
    Vector3<int> mCurrentStealQueued = { INT_MAX, INT_MAX, INT_MAX };
    Vector3<int> mServerMiningPos = { INT_MAX, INT_MAX, INT_MAX };
    Vector3<int> mLastStealerQueuedPos = { INT_MAX, INT_MAX, INT_MAX };
    std::vector<Vector3<int>> mStealQueuedPositions;
    std::vector<Vector3<int>> mCoverQueuedPositions;
    std::vector<Vector3<int>> mPlaceQueuedPositions;
    std::vector<Vector3<int>> mCoveredPositions;
    std::vector<Vector3<int>> mCheckStatusQueued;
    std::vector<Vector3<int>> mMiningPosQueued;
    std::vector<Vector3<int>> mUncoverQueuedPositions;
    std::map<std::string, uint64_t> mQueuedTimes;
    std::vector<Vector3<int>> mOreBlackList;
    int mFace = -1;
    float mBreakingProgress = 0.f;
    int mRotationTicks = 5;
    bool mIsRotating = false;
    bool mIsSkippingPackets = false;
    int mPreviousSlot = -1;
    int mToolSlot = -1;
    int mServerSlot = -1;

    bool mShouldResetAnimation = false;

    uint64_t mLastStealStarted = 0;
    uint64_t mEnemyUncoveredTime = 0;
    uint64_t mLastMiningPosReceived = 0;
    uint64_t mStealQueuedTime = 0;

    bool mCurrentUncover = false;

    std::vector<Vector3<int>> mOffsetList = {
        Vector3<int>(0, -1, 0),
        Vector3<int>(0, 1, 0),
        Vector3<int>(0, 0, -1),
        Vector3<int>(0, 0, 1),
        Vector3<int>(-1, 0, 0),
        Vector3<int>(1, 0, 0),
    };
public:
    void onEvent(BaseTickEvent* event) override
    {
        auto player = InstanceManager::getLocalPlayer();
        if (!player) return;
        Vector3<float> playerPos = *player->getPos();
        PlayerInventory* supplies = player->getSupplies();
        mPreviousSlot = supplies->mSelectedSlot;

        if (!mUncoverQueuedPositions.empty())
        {
            for (auto& pos : mUncoverQueuedPositions) {
                if (BlockUtil::isAirBlock(pos)) {
                    if (mOnEnemyUncover) displayMessage("Enemy uncovered ore");
                    mEnemyUncoveredTime = TimeUtil::getCurrentMs();
                    break;
                }
            }
            mUncoverQueuedPositions.clear();
        }

        if (mDynamicUncover && mEnemyUncoveredTime + 2000 > TimeUtil::getCurrentMs()) {
            mCurrentUncover = false;
        }
        else
        {
            mCurrentUncover = mUncover;
        }

        if (mCover)
        {
            if (!mCheckStatusQueued.empty())
            {
                for (auto& status : mCheckStatusQueued)
                {
                    if (BlockUtil::isAirBlock(status))
                    {
                        if (mOnGhostPlace)
                        {
                            displayMessage("Failed to cover ore", true);
                        }
                    }
                }
                mCheckStatusQueued.clear();
            }

            if (!mCoverQueuedPositions.empty())
            {
                bool hasBlock = 0 < ItemUtil::getAllPlaceables(mHotbarOnly);
                for (auto& coverQueued : mCoverQueuedPositions)
                {
                    if (hasBlock) {
                        if (isFilteredBlock(coverQueued, true, true, false, true)) {
                            int exposedFace = BlockUtil::getExposedBlockFace(coverQueued);
                            Vector3<int> placePos = coverQueued.add(mOffsetList[exposedFace]);
                            Vector3<int> belowBlock = placePos.add(Vector3<int>(0, -1, 0));

                            if (!BlockUtil::isAirBlock(placePos) || BlockUtil::isAirBlock(belowBlock)) continue;

                            int blockSlot = ItemUtil::getPlaceableItem(mHotbarOnly, false);
                            if (mUseNonSolid)
                            {
                                int foundSlot = ItemUtil::getNonSolidBlock(mHotbarOnly);
                                if (foundSlot != -1) blockSlot = foundSlot;
                            }
                            if (blockSlot == -1) continue;

                            if (0 < std::count(mCoveredPositions.begin(), mCoveredPositions.end(), placePos)) {
                                auto it = std::find(mCoverQueuedPositions.begin(), mCoverQueuedPositions.end(), coverQueued);
                                mCoverQueuedPositions.erase(it);

                                auto it2 = std::find(mCoveredPositions.begin(), mCoveredPositions.end(), placePos);
                                mCoveredPositions.erase(it2);
                                if (mOnGhostPlace)
                                {
                                    displayMessage("Failed to cover ore", true);
                                }
                                continue;
                            }

                            supplies->mSelectedSlot = blockSlot;
                            PacketUtil::spoofSwitch(blockSlot);

                            BlockUtil::placeBlock(placePos, 1);
                            mCoveredPositions.emplace_back(placePos);

                            supplies->mSelectedSlot = mPreviousSlot;
                            PacketUtil::spoofSwitch(mServerSlot);
                            break;
                        }
                        else {
                            auto it = std::find(mCoverQueuedPositions.begin(), mCoverQueuedPositions.end(), coverQueued);
                            mCoverQueuedPositions.erase(it);
                            if (mOnCover)
                            {
                                displayMessage("Covered ore");
                            }
                        }
                    }
                    else {
                        mCoverQueuedPositions.clear();
                    }
                }
            }
        }

        if (isTerminatable()) {
            resetStates();
            syncSlot();
            return;
        }

        Vector3<int> stealPos = { INT_MAX, INT_MAX, INT_MAX };
        Vector3<int> coveringPos = { INT_MAX, INT_MAX, INT_MAX };
        bool allowedSteal = false;
        if (mSteal) {
            if (!mStealQueuedPositions.empty())
            {
                for (auto& stealQueued : mStealQueuedPositions)
                {
                    for (auto& offset : mOffsetList)
                    {
                        Vector3<int> checkPos = stealQueued.add(offset);
                        if (isFilteredBlock(checkPos, true, false, false) && BlockUtil::getExposedBlockFace(checkPos, mBlockFilter) == -1)
                        {
                            stealPos = checkPos;
                            coveringPos = stealQueued;
                            break;
                        }
                    }
                    if (stealPos.x == INT_MAX) {
                        auto it = std::find(mStealQueuedPositions.begin(), mStealQueuedPositions.end(), stealQueued);
                        mStealQueuedPositions.erase(it);
                        continue;
                    }
                    break;
                }
            }

            if (mDelayed)
            {
                if (stealPos.x != INT_MAX)
                {
                    if (mLastStealerQueuedPos != stealPos)
                    {
                        mLastStealerQueuedPos = stealPos;
                        mStealQueuedTime = TimeUtil::getCurrentMs();
                    }
                    uint64_t timeElapsed = TimeUtil::getCurrentMs() - (mStealQueuedTime - (50 + GlobalVariables::mLastPing));
                    Block* coveringBlock = InstanceManager::getBlockSource()->getBlock(coveringPos);
                    int tool = ItemUtil::getBestBreakingTool(coveringBlock, mHotbarOnly);
                    float destroyRate = ItemUtil::getDestroySpeed(tool, coveringBlock);
                    float breakingProgress = destroyRate * (timeElapsed / 50);
                    if (mDestroySpeed <= breakingProgress)
                    {
                        allowedSteal = true;
                    }
                    else
                    {
                        allowedSteal = false;
                    }
                }
                else
                {
                    mLastStealerQueuedPos = { INT_MAX, INT_MAX, INT_MAX };
                }
            }
            else
            {
                allowedSteal = true;
            }
        }

        if (mSteal && mStealMode == 1 && !mStealQueuedPositions.empty() && mCurrentStatus != Status::Stealing && allowedSteal)
        {
            resetStates();
        }

        if (mCurrentStatus == Status::Stealing) {
            if (std::count(mStealQueuedPositions.begin(), mStealQueuedPositions.end(), mCurrentStealQueued) == 0 && BlockUtil::getExposedBlockFace(mMiningPos, mBlockFilter) == -1)
            {
                resetStates();
                if (mOnCancelSteal) {
                    displayMessage("Steal cancelled", true);
                }
            }
        }

        if (mCurrentStatus == Status::Stealing && mLastStealStarted + mStealerTimeout <= TimeUtil::getCurrentMs())
        {
            mStealQueuedPositions.clear();
            resetStates();
            if (mOnCancelSteal) {
                displayMessage("Steal cancelled (TIMEOUT)", true);
            }
        }

        if (mAntiSteal && mCurrentStatus == Status::Uncovering && std::count(mOreBlackList.begin(), mOreBlackList.end(), mRedstonePos) > 0)
        {
            resetStates();
            if (mOnCancelUncover) {
                displayMessage("Uncover cancelled (ANTI STEAL)", true);
            }
        }

        if (mCurrentStatus == Status::Mining) {
            if (!mPlaceQueuedPositions.empty())
            {
                for (auto& placeQueued : mPlaceQueuedPositions)
                {
                    if (!BlockUtil::isAirBlock(placeQueued) && BlockUtil::getExposedBlockFace(mMiningPos) == -1)
                    {
                        if (mOnCovered) {
                            displayMessage("Ore got covered", true);
                        }
                        break;
                    }
                }
                mPlaceQueuedPositions.clear();
            }
        }

        if (!isFilteredBlock(mMiningPos, mCurrentStatus != Status::Uncovering, !(mCurrentStatus == Status::Stealing || mCurrentStatus == Status::Uncovering))) {
            ChatUtil::displayClientMessage("Finding Ore");
            resetStates();

            if (mSteal && !mStealQueuedPositions.empty() && allowedSteal) {
                if (stealPos.x != INT_MAX)
                {
                    queueBlock(stealPos, stealPos, Status::Stealing);
                    mCurrentStealQueued = coveringPos;
                    mLastStealStarted = TimeUtil::getCurrentMs();
                    return;
                }
            }


            auto blocks = BlockUtil::getBlockList(playerPos.ToInt(), mRange);
            std::vector<BlockInfo> exposedOres;
            std::vector<BlockInfo> unexposedOres;

            for (BlockInfo block : blocks) {
                if (isFilteredBlock(block.mPosition, true, false)) {
                    if (BlockUtil::getExposedBlockFace(block.mPosition, mBlockFilter) != -1) exposedOres.emplace_back(block);
                    else unexposedOres.emplace_back(block);
                }
            }

            Vector3<int> destroyPos = { INT_MAX, INT_MAX, INT_MAX };
            Vector3<int> redstonePos = { INT_MAX, INT_MAX, INT_MAX };
            if (!exposedOres.empty()) {
                float closestDistance = INT_MAX;
                for (int i = 0; i < exposedOres.size(); i++) {
                    Vector3<int> orePos = exposedOres[i].mPosition;
                    float dist = playerPos.distance(orePos.ToFloat());
                    if (dist < closestDistance) {
                        closestDistance = dist;
                        destroyPos = orePos;
                        redstonePos = orePos;
                    }
                }

                if (mConfuse) {
                    Actor* target = nullptr;
                    auto actors = ActorUtils::getActorList(30);

                    std::ranges::sort(actors, [&](Actor* a, Actor* b) -> bool {
                        return a->getPos()->distance(*player->getPos()) < b->getPos()->distance(*player->getPos());
                        });

                    for (auto actor : actors) {
                        if (actor == player) continue;
                        target = actor;
                        break;
                    }

                    if (target) {
                        std::vector<BlockInfo> fakeBlocks = BlockUtil::getBlockList(target->getPos()->ToInt(), mRange);
                        std::vector<Vector3<int>> fakeExposedBlocks;
                        std::vector<Vector3<int>> fakeUnexposedBlocks;

                        for (int i = 0; i < fakeBlocks.size(); i++) {
                            int blockId = fakeBlocks[i].mBlock->getLegacyBlock()->getBlockID();
                            if (blockId == 73 || blockId == 74) {
                                if (BlockUtil::getExposedBlockFace(fakeBlocks[i].mPosition) != -1)
                                    fakeExposedBlocks.push_back(fakeBlocks[i].mPosition);
                                else fakeUnexposedBlocks.push_back(fakeBlocks[i].mPosition);
                            }
                            else continue;
                        }
                        if (mConfuseMode == 0) {
                            for (int i = 0; i < fakeExposedBlocks.size(); i++) {
                                if (fakeExposedBlocks[i] == destroyPos) continue;

                                BlockUtil::startDestroyBlock(fakeExposedBlocks[i], 0);
                            }
                        }
                        else if (mConfuseMode == 1) {
                            for (int i = 0; i < fakeUnexposedBlocks.size(); i++) {
                                Vector3<int> currentBlockPos = fakeUnexposedBlocks[i].add(mOffsetList[1]);
                                if (InstanceManager::getBlockSource()->getBlock(currentBlockPos)->getLegacyBlock()->isSolid()) {
                                    BlockUtil::startDestroyBlock(currentBlockPos, 0);
                                }
                            }
                        }
                    }
                }

                ChatUtil::displayClientMessage("Queued Ore");
                queueBlock(destroyPos, redstonePos, Status::Mining);
                return;
            }
            else if (!unexposedOres.empty() && mCurrentUncover) {
                bool pathFound = false;
                float fastestTime = INT_MAX;
                for (int i = 0; i < unexposedOres.size(); i++) {
                    Vector3<int> orePos = unexposedOres[i].mPosition;
                    PathFindingResult result = getPathToOre(orePos);
                    if (result.blockPos.x == INT_MAX) continue;

                    if (result.breakTime < fastestTime) {
                        fastestTime = result.breakTime;
                        destroyPos = result.blockPos;
                        redstonePos = orePos;
                        pathFound = true;
                    }
                }

                if (pathFound) {
                    queueBlock(destroyPos, redstonePos, Status::Uncovering);
                    return;
                }
            }

            syncSlot();
        }
        else
        {
            Block* miningBlock = InstanceManager::getBlockSource()->getBlock(mMiningPos);
            mFace = BlockUtil::getExposedBlockFace(mMiningPos, mBlockFilter);
            mToolSlot = ItemUtil::getBestBreakingTool(miningBlock, mHotbarOnly);
            float destroySpeed = ItemUtil::getDestroySpeed(mToolSlot, miningBlock);

            if (mToolSlot != mServerSlot) {
                destroySpeed = ItemUtil::getDestroySpeed(mServerSlot, miningBlock);
                PacketUtil::spoofSwitch(mToolSlot);
            }
            swingArm();
            mBreakingProgress += destroySpeed;

            if (mRotationMode == 1) {
                mIsRotating = true;
            }
            else if (mRotationMode == 2) {
                if ((mDestroySpeed * mRotationPercentage) <= mBreakingProgress) {
                    mIsRotating = true;
                }
                if (mDynamicRotation && mDestroySpeed <= mBreakingProgress + (destroySpeed * mRotationTicks)) {
                    mIsRotating = true;
                }
            }

            if (mDestroySpeed <= mBreakingProgress && (mCurrentStatus != Status::Stealing || mFace != -1))
            {
                mIsRotating = true;
                supplies->mSelectedSlot = mToolSlot;
                BlockUtil::destroyBlock(mMiningPos, mFace, mTransaction);
                supplies->mSelectedSlot = mPreviousSlot;
                if (mCurrentStatus == Status::Mining || mCurrentStatus == Status::Stealing)
                {
                    mOreBlackList.clear();
                }
                if (mCurrentStatus == Status::Stealing && mOnSteal) {
                    displayMessage("Stole ore");
                }
            }
        }
    }

    void onEvent(PacketEvent* event) override
    {
        auto player = InstanceManager::getLocalPlayer();
        if (!player) return;

        if (event->mPacket->getId() == PacketID::PlayerAuthInput)
        {
            auto paip = reinterpret_cast<PlayerAuthInputPacket*>(event->mPacket);
            if (mIsRotating)
            {
                const Vector3<float> blockPos = mMiningPos.ToFloat().add(Vector3<float>(0.5f, 0.5f, 0.5f));
                Vector2<float> rotations = MathUtil::getRots(*player->getPos(), blockPos);
                paip->mRotation = rotations;
                paip->mYHeadYaw = rotations.y;
                if (mHeadYaw) paip->mYHeadYaw += 90.0f;
                mIsRotating = false;
            }
        }
        else if (event->mPacket->getId() == PacketID::InventoryTransaction)
        {
            auto it = (InventoryTransactionPacket*)event->mPacket;
            if (it->mTransaction->mType == ComplexInventoryTransaction::Type::ItemUseTransaction)
            {
                ItemUseInventoryTransaction* transac = reinterpret_cast<ItemUseInventoryTransaction*>(it->mTransaction.get());
                if (transac->mActionType == ItemUseInventoryTransaction::ActionType::Place)
                    transac->mClickPos = Vector3<float>(0, 0, 0);
            }
        }
        else if (event->mPacket->getId() == PacketID::MobEquipment)
        {
            auto equipmentPacket = reinterpret_cast<MobEquipmentPacket*>(event->mPacket);
            mServerSlot = equipmentPacket->mSlot;
        }
    }

    void onEvent(LevelEventPacketEvent* event) override
    {
        auto player = InstanceManager::getLocalPlayer();
        if (!player) return;

        auto levelEventPacket = event->mPacket;
        Vector3<int> eventPos = levelEventPacket->mPos.ToInt();
        if (levelEventPacket->mEventId == LevelEvent::StartBlockBreaking)
        {
            uint64_t responseTime = 0;
            Block* eventBlock = InstanceManager::getBlockSource()->getBlock(eventPos);
            if (mPingCompensation && mServerMiningPos == eventPos && mLastMiningPosReceived + 50 < TimeUtil::getCurrentMs())
            {
                return;
            }
            if (std::count(mMiningPosQueued.begin(), mMiningPosQueued.end(), eventPos) > 0) {
                auto it = std::find(mMiningPosQueued.begin(), mMiningPosQueued.end(), eventPos);
                mMiningPosQueued.erase(it);
                mServerMiningPos = eventPos;
                mLastMiningPosReceived = TimeUtil::getCurrentMs();
                if (!mQueuedTimes.empty())
                {
                    responseTime = TimeUtil::getCurrentMs() - mQueuedTimes[convertVectorToString(eventPos)];
                    //ChatUtils::displayClientMessage("ResponseTime:" + std::to_string(responseTime));
                    //ChatUtils::displayClientMessage("CurrentPing:" + std::to_string(mCurrentPing));
                }
                if (!mPingCompensation || GlobalVariables::mLastPing <= responseTime || GlobalVariables::mLastPing - responseTime <= 5)
                {
                    return;
                }
                //ChatUtils::displayClientMessage("Ping Compensation");
            }

            if (15 < player->getPos()->distance(eventPos.ToFloat())) return;

            if (mAntiSteal)
            {
                if (mCurrentStatus == Status::Uncovering && mRedstonePos == eventPos)
                {
                    mOreBlackList.emplace_back(eventPos);
                }
            }

            if (mAntiConfuse && hasExposedCheck(eventPos))
            {
                return;
            }

            if (mSteal)
            {
                int eventBlockId = eventBlock->getLegacyBlock()->getBlockID();
                if (!(eventBlockId == 73 || eventBlockId == 74))
                {
                    for (auto& offset : mOffsetList)
                    {
                        Vector3<int> checkPos = eventPos.add(offset);
                        if (isFilteredBlock(checkPos, true, false, false) && BlockUtil::getExposedBlockFace(checkPos, mBlockFilter) == -1)
                        {
                            mStealQueuedPositions.emplace_back(eventPos);
                            break;
                        }
                    }
                }
            }

            if (mCover)
            {
                if (mBlockStealer || mServerMiningPos != eventPos)
                {
                    if (!mSteal || mCurrentStatus != Status::Stealing || mServerMiningPos != eventPos) {
                        if (isFilteredBlock(eventPos, true, true, false, true)) {
                            if (mAntiSteal && mServerMiningPos == eventPos)
                            {
                                mOreBlackList.emplace_back(eventPos);
                            }
                            mCoverQueuedPositions.emplace_back(eventPos);
                        }
                    }
                }
            }
        }
        else if (levelEventPacket->mEventId == LevelEvent::StopBlockBreaking)
        {
            if (mSteal) {
                if (std::count(mStealQueuedPositions.begin(), mStealQueuedPositions.end(), eventPos) > 0) {
                    auto it = std::find(mStealQueuedPositions.begin(), mStealQueuedPositions.end(), eventPos);
                    mStealQueuedPositions.erase(it);
                }
            }

            if (mAntiSteal && mAntiStealMode == 0)
            {
                if (std::count(mOreBlackList.begin(), mOreBlackList.end(), eventPos) > 0)
                {
                    auto it = std::find(mOreBlackList.begin(), mOreBlackList.end(), eventPos);
                    mOreBlackList.erase(it);
                }
            }

            if (mCover)
            {
                if (std::count(mCoverQueuedPositions.begin(), mCoverQueuedPositions.end(), eventPos)) {
                    auto it = std::find(mCoverQueuedPositions.begin(), mCoverQueuedPositions.end(), eventPos);
                    mCoverQueuedPositions.erase(it);
                }
            }

            if (mServerMiningPos == eventPos) {
                mServerMiningPos = { INT_MAX, INT_MAX, INT_MAX };
            }
        }
    }

    void onEvent(UpdateBlockEvent* event) override
    {
        auto player = InstanceManager::getLocalPlayer();
        if (!player) return;

        auto updateBlockPacket = event->mPacket;
        if (15 < player->getPos()->distance(updateBlockPacket->mPos.ToFloat()) || mMiningPos == updateBlockPacket->mPos) return;
        bool isAir = BlockUtil::isAirBlock(updateBlockPacket->mPos);

        if (!isAir && 0 < std::count(mCoveredPositions.begin(), mCoveredPositions.end(), updateBlockPacket->mPos))
        {
            mCheckStatusQueued.emplace_back(updateBlockPacket->mPos);
            auto it = std::find(mCoveredPositions.begin(), mCoveredPositions.end(), updateBlockPacket->mPos);
            mCoveredPositions.erase(it);
        }

        if (!BlockUtil::isAirBlock(updateBlockPacket->mPos)) {
            for (auto& offset : mOffsetList) {
                Vector3<int> blockPos = updateBlockPacket->mPos.add(offset);
                int blockId = InstanceManager::getBlockSource()->getBlock(blockPos)->getLegacyBlock()->getBlockID();
                if ((blockId == 73 || blockId == 74) && BlockUtil::getExposedBlockFace(blockPos) == -1) {
                    mUncoverQueuedPositions.emplace_back(updateBlockPacket->mPos);
                    break;
                }
            }
        }

        if (mCurrentStatus != Status::Mining) return;
        if (isAir)
        {
            Vector3<int> delta = mMiningPos.submissive(updateBlockPacket->mPos);
            int dist = abs(delta.x) + abs(delta.y) + abs(delta.z);
            if (dist == 1)
            {
                mPlaceQueuedPositions.emplace_back(updateBlockPacket->mPos);
            }
        }
    }

    void onEvent(ImRenderEvent* event) override
    {
        auto player = InstanceManager::getLocalPlayer();
        if (!player) return;

        //renderBlock();
        renderNewProgressBar();
    }

    void resetStates()
    {
        auto player = InstanceManager::getLocalPlayer();
        if (!player) return;

        if (mCurrentStatus != Status::None)
        {
            player->getGameMode()->stopDestroyBlock(mMiningPos);
        }

        mMiningPos = { INT_MAX, INT_MAX, INT_MAX };
        mRedstonePos = { INT_MAX, INT_MAX, INT_MAX };
        mBreakingProgress = 0.f;
        mCurrentStatus = Status::None;
        mToolSlot = -1;
        mCurrentStealQueued = { INT_MAX, INT_MAX, INT_MAX };
    }

    void syncSlot()
    {
        if (mServerSlot != mPreviousSlot)
        {
            PacketUtil::spoofSwitch(mPreviousSlot);
        }
    }

    bool isTerminatable() {
        auto player = InstanceManager::getLocalPlayer();
        if (!player) return true;

        PlayerInventory* supplies = player->getSupplies();
        float absorption = player->getAbsorption();
        int pickaxeSlot = ItemUtil::getItem(ItemType::Pickaxe, mHotbarOnly);
        bool hasPickaxe = pickaxeSlot != -1;

        if (!mAlwaysMine && 10 <= absorption) return true;
        if (!hasPickaxe) return true;
        //if (player->getStatusFlag(ActorFlags::Immobile)) return true;
        //if (player->isDestroying()) return true;
        if (mRotationMode == 2 && !player->isOnGround()) return true;

        return false;
    }

    bool isFilteredBlock(Vector3<int> blockPos, bool redstoneCheck, bool exposedCheck, bool applyBlackList = true, bool useNormalExposedCheck = false)
    {
        auto player = InstanceManager::getLocalPlayer();
        if (!player) return false;

        Block* block = InstanceManager::getBlockSource()->getBlock(blockPos);
        if (blockPos.x == INT_MAX) return false;

        int blockId = block->getLegacyBlock()->getBlockID();
        if (blockId == 0) return false;

        bool isRedstone = blockId == 73 || blockId == 74;
        if (redstoneCheck && !isRedstone) return false;

        AABB blockAABB = AABB(blockPos.ToFloat(), Vector3<float>(1, 1, 1));
        Vector3<float> closestPos = blockAABB.getClosestPoint(*player->getPos());
        if (mRange < closestPos.distance(*player->getPos())) return false;
        bool useBlockFilter = useNormalExposedCheck ? false : mBlockFilter;
        int exposedFace = BlockUtil::getExposedBlockFace(blockPos, useBlockFilter);
        if (exposedCheck && exposedFace == -1) return false;

        if (applyBlackList && mAntiSteal && exposedFace == -1 && std::count(mOreBlackList.begin(), mOreBlackList.end(), blockPos) > 0) return false;

        return true;
    }

    PathFindingResult getPathToOre(Vector3<int> blockPos)
    {
        auto player = InstanceManager::getLocalPlayer();
        if (!player) return { Vector3<int>(INT_MAX, INT_MAX, INT_MAX), 0 };

        BlockSource* source = InstanceManager::getBlockSource();
        if (!source) return { Vector3<int>(INT_MAX, INT_MAX, INT_MAX), 0 };

        float currentBreakingTime = 0;
        float bestBreakingTime = INT_MAX;
        Vector3<int> bestPos = { INT_MAX, INT_MAX, INT_MAX };

        int uncoverRange = 1;

        if (mUncoverMode == 0) {
            bool foundPath = false;
            std::vector<BlockInfo> blockList = BlockUtil::getBlockList(blockPos, uncoverRange);
            for (auto& block : blockList) {
                if (!BlockUtil::isAirBlock(block.mPosition) && BlockUtil::getExposedBlockFace(block.mPosition) != -1) {
                    Vector3<int> delta = block.mPosition.submissive(blockPos);
                    int dist = abs(delta.x) + abs(delta.y) + abs(delta.z);
                    if (uncoverRange < dist) continue;
                    if (dist < bestBreakingTime) {
                        bestBreakingTime = dist;
                        bestPos = block.mPosition;
                        foundPath = true;
                    }
                }
            }
            if (!foundPath) return { Vector3<int>(INT_MAX, INT_MAX, INT_MAX), 0 };
        }
        else if (mUncoverMode == 1) {
            for (int i = 0; i < mOffsetList.size(); i++) {
                currentBreakingTime = 0;
                Vector3<int> pos1 = blockPos.add(mOffsetList[i]);
                Block* currentBlock = source->getBlock(pos1);
                bool isAir = BlockUtil::isAirBlock(pos1);
                float blockBreakingTime1 = 0;
                if (!isAir) blockBreakingTime1 = 1 / ItemUtil::getDestroySpeed(ItemUtil::getBestBreakingTool(currentBlock, mHotbarOnly), currentBlock);
                for (int i2 = 0; i2 < mOffsetList.size(); i2++) {
                    Vector3<int> reversedPos = Vector3<int>(-mOffsetList[i].x, -mOffsetList[i].y, -mOffsetList[i].z);
                    if (reversedPos == mOffsetList[i2]) continue;
                    Vector3<int> pos2 = pos1.add(mOffsetList[i2]);
                    Block* currentBlock2 = source->getBlock(pos2);
                    bool isAir2 = BlockUtil::isAirBlock(pos2);
                    float blockBreakingTime2 = 0;
                    if (!isAir2) blockBreakingTime2 = 1 / ItemUtil::getDestroySpeed(ItemUtil::getBestBreakingTool(currentBlock2, mHotbarOnly), currentBlock2);
                    currentBreakingTime = blockBreakingTime1 + blockBreakingTime2;

                    Vector3<int> breakPos = !isAir2 ? pos2 : pos1;

                    if (currentBreakingTime < bestBreakingTime && isFilteredBlock(breakPos, false, false, false) && !hasExposedCheck(breakPos)) {
                        bestBreakingTime = currentBreakingTime;
                        bestPos = breakPos;
                    }
                }
            }
        }

        return { bestPos, bestBreakingTime };
    }

    bool hasExposedCheck(Vector3<int> blockPos)
    {
        bool hasRedstone = false;
        for (auto offset : mOffsetList)
        {
            Vector3<int> pos = blockPos.add(offset);
            if (BlockUtil::isAirBlock(pos)) return false;
            int blockId = InstanceManager::getBlockSource()->getBlock(pos)->getLegacyBlock()->getBlockID();
            if (blockId == 73 || blockId == 74) hasRedstone = true;
        }

        int originBlockId = InstanceManager::getBlockSource()->getBlock(blockPos)->getLegacyBlock()->getBlockID();
        if (hasRedstone || originBlockId == 73 || originBlockId == 74) {
            return true;
        }
        return false;
    }

    void queueBlock(Vector3<int> blockPos, Vector3<int> redstonePos, Status status)
    {
        auto player = InstanceManager::getLocalPlayer();
        if (!player) return;

        Block* block = InstanceManager::getBlockSource()->getBlock(blockPos);

        mToolSlot = ItemUtil::getBestBreakingTool(block, mHotbarOnly);
        float destroySpeed = ItemUtil::getDestroySpeed(mToolSlot, block);
        if (mDestroySpeed <= destroySpeed) mIsRotating = true;
        PacketUtil::spoofSwitch(mToolSlot);

        if (mRotationMode == 1) {
            mIsRotating = true;
        }
        else if (mRotationMode == 2) {
            if ((mDestroySpeed * mRotationPercentage) <= destroySpeed) {
                mIsRotating = true;
            }

            if (mDynamicRotation)
            {
                Block* redstoneBlock = InstanceManager::getBlockSource()->getBlock(redstonePos);
                int pickaxeSlot = ItemUtil::getBestBreakingTool(redstoneBlock, mHotbarOnly);
                float breakingRate = ItemUtil::getDestroySpeed(pickaxeSlot, redstoneBlock);
                mRotationTicks = (mDestroySpeed - (mDestroySpeed * mRotationPercentage)) / breakingRate;
                if (mDestroySpeed <= mBreakingProgress + (destroySpeed * mRotationTicks)) {
                    mIsRotating = true;
                }
            }
        }

        mFace = BlockUtil::getExposedBlockFace(blockPos, mBlockFilter);
        if (mFace == -1) {
            // this code is required
            mFace = 0;
        }
        mMiningPosQueued.emplace_back(blockPos);
        BlockUtil::startDestroyBlock(blockPos, mFace);
        swingArm();
        mMiningPos = blockPos;
        mRedstonePos = redstonePos;
        mCurrentStatus = status;
        mQueuedTimes[convertVectorToString(blockPos)] = TimeUtil::getCurrentMs();

        mShouldResetAnimation = true;
    }

    void swingArm()
    {
        auto player = InstanceManager::getLocalPlayer();
        if (!player) return;

        if (mSwingMode == 0) {
            player->swing();
        }
        else if (mSwingMode == 1) {
            auto pkt = MinecraftPackets::createPacket(PacketID::Animate);
            AnimatePacket* animPacket = reinterpret_cast<AnimatePacket*>(pkt.get());
            animPacket->mRuntimeId = player->getRuntimeID();
            animPacket->mAction = AnimatePacket::Action::Swing;
            InstanceManager::get<ClientInstance>()->getLoopbackPacketSender()->send(pkt.get());
        }
    }

    void displayMessage(const std::string& message, bool warn = false)
    {
        if (!mDebug) return;

        ChatUtil::displayClientMessage(message);
    }

    std::string convertVectorToString(Vector3<int> position)
    {
        return std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(position.z);
    }

    void renderNewProgressBar()
    {
        auto player = InstanceManager::getLocalPlayer();
        if (!player) return;

        static float lastProgress = 0.f;
        float percentDone = 1.f;

        percentDone = mBreakingProgress;
        percentDone /= mDestroySpeed;
        if (percentDone < lastProgress) lastProgress = percentDone;
        percentDone = MathUtil::lerp(lastProgress, percentDone, ImGui::GetIO().DeltaTime * 10.f); // 30.f - old
        lastProgress = percentDone;
        // clamp percentDone
        percentDone = MathUtil::clamp(percentDone, 0.f, 1.f);

        float delta = ImGui::GetIO().DeltaTime;

        static EasingUtil inEase = EasingUtil();
        if (mShouldResetAnimation)
        {
            inEase.resetPercentage();
            mShouldResetAnimation = false;
        }
        static float anim = 0.f;
        constexpr float easeSpeed = 10.f;
        (mEnabled && mCurrentStatus != Status::None) ? inEase.incrementPercentage(delta * easeSpeed / 10)
            : inEase.decrementPercentage(delta * 2 * easeSpeed / 10);
        float inScale = inEase.easeOutExpo();
        if (inEase.isPercentageMax()) inScale = 0.996;
        inScale = MathUtil::clamp(inScale, 0.0f, 0.996);
        anim = MathUtil::lerp(0, 1, inEase.easeOutExpo());
        anim = MathUtil::lerp(anim, (mEnabled && mCurrentStatus != Status::None) ? 1.f : 0.f, delta * 10.f);


        if (anim < 0.001f) return;

        auto drawList = ImGui::GetBackgroundDrawList();

        ImVec2 displaySize = ImGui::GetIO().DisplaySize;
        ImVec2 pos = ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2.5f);
        pos.y += pos.y / 1.14;
        ImVec2 boxSize = ImVec2(216 * anim, 48 * anim);
        // Center da progress bar
        pos.x -= boxSize.x / 2;
        pos.y -= boxSize.y / 2;

        pos.y += 20; // mOffset

        ImVec2 progressPos = ImVec2(pos.x, pos.y);
        ImVec2 progressSize = ImVec2(boxSize.x * percentDone, boxSize.y);

        int daPerc = percentDone * 100;

        static std::string text = "Mining 0%";

        static ImColor color = ImColor(255, 255, 0, 180);
        ImColor targetColor = ImColor(0, 255, 0, 153);

        bool isMining = mCurrentStatus != Status::None && mBreakingProgress > 0.001f; // i hate you.

        //auto interfaceMod = gFeatureManager->mModuleManager->getModule<Interface>();
        bool isLowercase = false;

        if (mCurrentStatus == Status::Stealing && mEnabled) // Stealing
        {
            targetColor = ImColor(153, 50, 204, 169);
            if (isLowercase) text = "stealing " + std::to_string(daPerc) + "%";
            else text = "Stealing " + std::to_string(daPerc) + "%";
        }
        else if (mCurrentStatus == Status::Uncovering && mEnabled) // Uncovering
        {
            if (isLowercase) text = "uncovering " + std::to_string(daPerc) + "%";
            else text = "Uncovering " + std::to_string(daPerc) + "%";
            targetColor = ImColor(240, 128, 128, 169);
        }
        else if (isMining && 10 <= player->getAbsorption() && mEnabled) // Queueing
        {
            if (isLowercase) text = "queueing " + std::to_string(daPerc) + "%";
            else text = "Queueing " + std::to_string(daPerc) + "%";
            targetColor = ImColor(123, 104, 238, 169);
        }
        else // Mining
        {
            if (isLowercase) text = "mining " + std::to_string(daPerc) + "%";
            else text = "Mining " + std::to_string(daPerc) + "%";
            targetColor = ImColor(255, 215, 33, 169);
        }

        color = targetColor;

        float daPadding = -25.f * anim;

        float max = pos.x + boxSize.x;
        ImVec2 bgMin = ImVec2(pos.x + boxSize.x * percentDone, pos.y);
        ImVec2 bgMax = ImVec2(pos.x + boxSize.x, pos.y + (boxSize.y + daPadding));
        ImVec2 progMax = ImVec2(pos.x + (boxSize.x * percentDone + 6.f), pos.y + (boxSize.y + daPadding));
        progMax.x = std::clamp(progMax.x, pos.x, max);

        float rounding = 99;

        if (percentDone > 0.001f)
        {
            drawList->AddShadowRect(ImVec2(pos.x, pos.y), progMax, color, 50.f, ImVec2(), ImDrawFlags_RoundCornersAll, rounding);
            drawList->PushClipRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + (boxSize.x * percentDone), pos.y + (boxSize.y - 10.f)));
            //drawList->AddRectFilled(ImVec2(pos.x, pos.y), progMax, color, rounding);
            //ImColor leftSide = ImColor(color.Value.x * 0.40f, color.Value.y * 0.40f, color.Value.z * 0.40f, color.Value.w);
            ImColor leftSide = color;
            drawList->AddRectFilledMultiColor(ImVec2(pos.x, pos.y), progMax, color, color, color, color, rounding, ImDrawFlags_RoundCornersAll);
            drawList->PopClipRect();
        }

        if (percentDone > 0.001f) drawList->PushClipRect(bgMin, bgMax);
        drawList->AddRectFilled(ImVec2(pos.x + boxSize.x * percentDone - 6, pos.y), bgMax, ImColor(0.f, 0.f, 0.f, 0.6f), rounding);
        if (percentDone > 0.001f) drawList->PopClipRect();

        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);

        float fontSize = 20.f * anim;
        ImVec2 textSize = ImGui::GetFont()->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, text.c_str());
        // center da text between
        ImVec2 textPos = ImVec2(pos.x + (boxSize.x - textSize.x) / 2, pos.y + 1.6);

        ImRenderUtil::drawText(Vector2<float>(textPos.x, textPos.y), &text, Color(255, 255, 255, 255), fontSize, 255.f, true);
        ImGui::PopFont();
    }

    void renderBlock()
    {
        if (mCurrentStatus == Status::None || !mEnabled) return;

        auto player = InstanceManager::getLocalPlayer();

        static float lastProgress = 0.f;
        float progress = 1.f;

        progress = mBreakingProgress;
        progress /= mDestroySpeed;
        if (progress < lastProgress) lastProgress = progress;
        progress = MathUtil::lerp(lastProgress, progress, ImGui::GetIO().DeltaTime * 30.f);
        lastProgress = progress;

        // clamp the progress to 0-1
        progress = MathUtil::clamp(progress, 0.f, 1.f);

        if (progress < 0.01f) return;

        auto size = Vector3<float>(progress, progress, progress);
        Vector3<float> blockPos = mMiningPos.ToFloat();
        blockPos.x += 0.5f - (progress / 2.f);
        blockPos.y += 0.5f - (progress / 2.f);
        blockPos.z += 0.5f - (progress / 2.f);
        auto blockAABB = AABB(blockPos, size);

        ImColor targetColor = ImColor(255, 255, 0, 255);

        if (mCurrentStatus == Status::Stealing) targetColor = ImColor(255, 0, 0, 255);
        else if (mCurrentStatus == Status::Uncovering) targetColor = ImColor(255, 255, 0, 255);
        else if (mCurrentStatus == Status::Mining && 10 <= player->getAbsorption()) targetColor = ImColor(0, 255, 255, 255);
        else targetColor = ImColor(0, 255, 0, 255);
    };
};