#pragma once

class Scaffold : public Module
{
public:
    Scaffold() :
        Module("Scaffold", "Misc", "Places blocks under your feet.")
    {
        EnumSetting("Rotations", "The rotation mode", { "Normal", "Moonwalk" }, &mRotations);
        EnumSetting("Switch", "How to hold the block", { "Hold", "Spoof", "ClientSpoof" }, &mHoldStyle);
        EnumSetting("TowerMode", "The tower mode", { "Jump", "Velocity", "Timer", "Tozic" }, &mTowerMode);
        EnumSetting("Placement", "The placement mode", { "Normal", "Flareon", "Legit" }, &mPlacement);
        EnumSetting("Counter", "The style of displaying the block counter", { "None", "Aeolus" }, &mCounter);
        BoolSetting("Inventory Slot", "Use the inventory slot", &mInvSlot);
        BoolSetting("Flareon V2", "Places the placement in a speacific style", &mFlareonV2);
        BoolSetting("Aura Bypass", "Stops scaffold rotation and stops scaffold when killaura is hitting a target", &mAuraBypass);
        BoolSetting("Keep Y", "Keeps your y level when placing blocks", &mYLock);
        SliderSetting("PlacesPerTick", "how many blocks it can place in tick", &mPlacesPerTick, 1, 10);
        SliderSetting("Distance", "The place distance (how far to place blocks)", &mRange, 3, 8);
        SliderSetting("Extend", "Distance your position to target block", &mExtend, 0, 10);
    }
private:
    //Settings
    int mRotations = 0;
    int mHoldStyle = 0;
    int mTowerMode = 0;
    int mPlacement = 0;
    int mCounter = 1;
    float mExtend = 0;
    float mRange = 5.f;
    float mPlacesPerTick = 1.f;
    bool mInvSlot = true;
    bool mFlareonV2 = false;
    bool mAuraBypass = true;
    bool mYLock = false;

    //Other
    int mPrevSlot = 0;
    int mBlockSlot = 0;
    int mPlacesPerTickCount = 0;
    float mLockedY = 0;
    bool mIsHoldingSpace = false;
    bool mFoundBlock = false;
    Vector3<float> mBlockPos;
    Vector3<float> mClickBlockPos;
    Vector3<float> mBlockBelow;
public:
    bool canPlace(Vector3<float> pos) 
    {
        return InstanceManager::getBlockSource()->getBlock(pos.floor().ToInt())->getLegacyBlock()->getBlockID() == 0;
    }

    bool findBlock() 
    {
        PlayerInventory* mPlayerInventory = InstanceManager::getLocalPlayer()->getSupplies();
        Inventory* mInventory = mPlayerInventory->getInventory();
        const int mPreviousSlot = mPlayerInventory->mSelectedSlot;
        const int mMaxSlots = mInvSlot ? 36 : 9;

        for (int i = 0; i < mMaxSlots; ++i) 
        {
            ItemStack* stack = mInventory->getItemStack(i);

            if (stack != nullptr && stack->isBlockType() && !stack->getItem()->isBoomBox()) 
            {
                if (mPreviousSlot != i) 
                {
                    mPlayerInventory->mSelectedSlot = i;
                    mBlockSlot = i; 
                }
                return true;
            }
        }

        return false;
    }

    Vector3<float> getBlockBelow(Player* player) 
    {
        Vector3<float> mBlockBelow = player->getAABBShapeComponent()->mPosLower;
        mBlockBelow.y -= 1.f;
        return mBlockBelow;
    }

    void adjustYCoordinate(const Vector3<float>& blockBelowReal) 
    {
        if (floor(blockBelowReal.y) < floor(mLockedY)) 
        {
            mLockedY = blockBelowReal.y + InstanceManager::getLocalPlayer()->getStateVectorComponent()->mVelocity.y;
        }
    }

    Vector3<float> getExtendedPosition(const Vector3<float>& velocity, Vector3<float>& blockBelow, float extendValue) 
    {
        Vector3<float> mExtendedBlock = blockBelow;
        Vector3<float> mNormalizedVelocity = velocity.Normalize();
        mExtendedBlock.x += mNormalizedVelocity.x * extendValue;
        mExtendedBlock.z += mNormalizedVelocity.z * extendValue;
        return mExtendedBlock;
    }

    Vector3<float> getNextBlock(const Vector3<float>& velocity, const Vector3<float>& blockBelow, float extendValue) 
    {
        Vector3<float> mNextBlock = blockBelow;
        if (abs(velocity.x) > abs(velocity.z)) 
        {
            mNextBlock.x += (velocity.x > 0 ? 1 : (velocity.x < 0 ? -1 : 0)) * extendValue;
        }
        else 
        {
            mNextBlock.z += (velocity.z > 0 ? 1 : (velocity.z < 0 ? -1 : 0)) * extendValue;
        }
        return mNextBlock;
    }

    Vector2<float> getNormAngle(const Vector2<float>& angle) 
    {
        float x = angle.x;
        float y = angle.y;

        while (x > 90.f)
            x -= 180.0f;

        while (x < -90.f)
            x += 180.0f;

        while (y > 180.0f)
            y -= 360.0f;

        while (y < -180.0f)
            y += 360.0f;

        return Vector2<float>(x, y);
    }

    bool buildBlock(Vector3<float> blockBelow) 
    {
        mBlockPos = Vector3<float>(blockBelow.ToInt().ToFloat().add(Vector3<float>(0.5f, 0, 0.5f)));
        //Vector3<float> mVel = InstanceManager::getLocalPlayer()->getStateVectorComponent()->mVelocity;
        //mVel = mVel.Normalize(); 
        blockBelow = blockBelow.floor();

        if (InstanceManager::getBlockSource()->getBlock(Vector3<int>(blockBelow.ToInt()))->getLegacyBlock()->getBlockID() == 0) 
        {
            Vector3<int> mBlock(blockBelow.ToInt());

            static std::vector<Vector3<int>*> mChecklist;
            if (mChecklist.empty()) 
            {
                mChecklist.push_back(new Vector3<int>(0, -1, 0));
                mChecklist.push_back(new Vector3<int>(0, 1, 0));

                mChecklist.push_back(new Vector3<int>(0, 0, -1));
                mChecklist.push_back(new Vector3<int>(0, 0, 1));

                mChecklist.push_back(new Vector3<int>(-1, 0, 0));
                mChecklist.push_back(new Vector3<int>(1, 0, 0));
            }

            bool mFoundCandidate = false;
            int i = 0;
            for (auto mCurrent : mChecklist) 
            {
                Vector3<int> mCalc = mBlock.submissive(*mCurrent);
                if (InstanceManager::getBlockSource()->getBlock(mCalc)->getLegacyBlock()->getBlockID() != 0) {
                    mFoundCandidate = true;
                    mBlock = mCalc;
                    break;
                }
                i++;
            }

            mClickBlockPos = mBlock.ToFloat().add(Vector3<float>(0.5f, 0, 0.5f));
            if (mFoundCandidate) 
            {
                if (1 <= mPlacement) 
                {
                    auto player = InstanceManager::getLocalPlayer();
                    player->getLevel()->getHitResult()->BlockFace = i;
                    player->getLevel()->getHitResult()->IBlockPos = mBlockPos.ToInt();
                    player->getLevel()->getHitResult()->mType = HitResultType::Tile;
                    player->getLevel()->getHitResult()->AbsoluteHitPos = mBlockPos.ToFloat();
                }
                InstanceManager::getLocalPlayer()->getGameMode()->buildBlock(mBlock, i, true);
                return true;
            }
        }
        return false;
    }

    bool predictBlock(Vector3<float> blockBelow) {
        //Vector3<float> mVel = InstanceManager::getLocalPlayer()->getStateVectorComponent()->mVelocity;
        //mVel = mVel.Normalize();
        blockBelow = blockBelow.floor();

        static std::vector<Vector3<int>> checkBlocks;
        if (checkBlocks.empty()) 
        { 
            for (int y = -mRange; y < mRange; y++) {
                for (int x = -mRange; x < mRange; x++) {
                    for (int z = -mRange; z < mRange; z++) {
                        checkBlocks.push_back(Vector3<int>(x, y, z));
                    }
                }
            }
            
            std::sort(checkBlocks.begin(), checkBlocks.end(), [](Vector3<int> first, Vector3<int> last) {
                return sqrtf((float)(first.x * first.x) + (float)(first.y * first.y) + (float)(first.z * first.z)) < sqrtf((float)(last.x * last.x) + (float)(last.y * last.y) + (float)(last.z * last.z));
                });
        }

        for (const Vector3<int>& blockOffset : checkBlocks) 
        {
            Vector3<int> currentBlock = Vector3<int>(blockBelow.ToInt()).add(blockOffset);
            mBlockPos = Vector3<float>(currentBlock.ToInt().ToFloat().add(Vector3<float>(0.5f, 0, 0.5f)));

            if (InstanceManager::getBlockSource()->getBlock(Vector3<int>(currentBlock))->getLegacyBlock()->getBlockID() == 0)
            {
                Vector3<int> mBlock(currentBlock);

                static std::vector<Vector3<int>*> mChecklist;
                if (mChecklist.empty()) 
                {
                    mChecklist.push_back(new Vector3<int>(0, -1, 0));
                    mChecklist.push_back(new Vector3<int>(0, 1, 0));

                    mChecklist.push_back(new Vector3<int>(0, 0, -1));
                    mChecklist.push_back(new Vector3<int>(0, 0, 1));

                    mChecklist.push_back(new Vector3<int>(-1, 0, 0));
                    mChecklist.push_back(new Vector3<int>(1, 0, 0));
                }

                bool mFoundCandidate = false;
                int i = 0;
                for (auto mCurrent : mChecklist) 
                {
                    Vector3<int> calc = mBlock.submissive(*mCurrent);
                    
                    if (InstanceManager::getBlockSource()->getBlock(calc)->getLegacyBlock()->getBlockID() != 0) 
                    {
                        // Found a solid block to click
                        mFoundCandidate = true;
                        mBlock = calc;
                        break;
                    }
                    i++;
                }

                mClickBlockPos = mBlock.ToFloat().add(Vector3<float>(0.5f, 0, 0.5f));

                if (mFoundCandidate) 
                {
                    if (1 <= mPlacement) 
                    {
                        auto player = InstanceManager::getLocalPlayer();
                        player->getLevel()->getHitResult()->BlockFace = i;
                        player->getLevel()->getHitResult()->IBlockPos = mBlockPos.ToInt();
                        player->getLevel()->getHitResult()->mType = HitResultType::Tile;
                        player->getLevel()->getHitResult()->AbsoluteHitPos = mBlockPos;
                    }
                    InstanceManager::getLocalPlayer()->getGameMode()->buildBlock(mBlock, i, true);
                    return true;
                }
            }
        }
        return false;
    }

    void onEnabled() override 
    {
        auto mPlayer = InstanceManager::getLocalPlayer();
        if (mPlayer == nullptr) return;

        Vector3<float> mBlockbelow = getBlockBelow(mPlayer);

        mLockedY = mBlockbelow.y;
        mPrevSlot = InstanceManager::getLocalPlayer()->getSupplies()->mSelectedSlot;

        mBlockBelow = mBlockbelow;

        mPlacesPerTickCount = 0;
    }

    void onEvent(BaseTickEvent* event) override 
    {
        auto player = InstanceManager::getLocalPlayer();
        if (player == nullptr) return;

        if (getModuleByType<Regen>()->isEnabled() && GlobalVariables::mIsMiningRedstone) 
        {
            return;
        }

        mPlacesPerTickCount = 0;

        mFoundBlock = findBlock();
        if (!mFoundBlock) return;

        float speed = InstanceManager::getLocalPlayer()->getStateVectorComponent()->mVelocity.magnitudexz();
        Vector3<float> velocity = InstanceManager::getLocalPlayer()->getStateVectorComponent()->mVelocity.Normalize();
        Vector3<float> blockBelow = getBlockBelow(player);
        mBlockBelow = blockBelow;
        Vector3<float> yLockedBlockBelow = Vector3<float>(blockBelow.x, mLockedY, blockBelow.z);
        if (mYLock) blockBelow = yLockedBlockBelow;

        if (mYLock) {
            if (player->isOnGround()) 
            {
                mLockedY = blockBelow.y;
            }

            adjustYCoordinate(getBlockBelow(player));
            blockBelow = yLockedBlockBelow;
        }
        else mLockedY = blockBelow.y;
        for (int i = 0; i < mPlacesPerTick; i++) 
        {
            if (canPlace(blockBelow)) {
                if (predictBlock(blockBelow)) 
                {

                }
            }

            if (!mIsHoldingSpace) {
                if (mPlacement == 1) {
                    for (int i = 1; i < mExtend; i++) {
                        Vector3<float> nextBlock = getNextBlock(velocity, blockBelow, i);
                        if (canPlace(nextBlock) && predictBlock(nextBlock)) {
                            break;
                        }
                    }
                }
                else {
                    for (int i = 1; i < mExtend; i++) {
                        Vector3<float> nextBlock = getExtendedPosition(velocity, blockBelow, i);
                        if (canPlace(nextBlock) && predictBlock(nextBlock)) {
                            break;
                        }
                    }
                }
            }
        }

        if (GlobalVariables::mKey[VK_SPACE]) 
        {
            mIsHoldingSpace = true;
        }
        else 
        {
            mIsHoldingSpace = false;
        }

        if (mTowerMode == 1) 
        {
            float speedOfTower = 0.7;
            bool space = GlobalVariables::mKey[VK_SPACE];
            static bool oldSpace = false;
            if (space) 
            {
                player->getStateVectorComponent()->mVelocity.y = speedOfTower;
            }
            else if (oldSpace) 
            {
                player->getStateVectorComponent()->mVelocity.y = -5.f;
            }

            oldSpace = space;
        }

        if (mHoldStyle >= 1) 
        {
            if (mHoldStyle == 2) 
            {
                PacketUtil::spoofSwitch(mBlockSlot);
            }

            player->getSupplies()->mSelectedSlot = mPrevSlot;
        }
    } 

    void onEvent(PacketEvent* event) 
    {
        auto player = InstanceManager::getLocalPlayer();

        if (!player || !player->getStateVectorComponent()) 
        { 
            return; 
        }

        if (getModuleByType<Regen>()->isEnabled() && GlobalVariables::mIsMiningRedstone) 
        {
            return;
        }

        float speed = player->getStateVectorComponent()->mVelocity.magnitudexz();
        Vector3<float> blockBelow = getBlockBelow(player);

        Vector2<float> mAngleNormal = player->getPosition().CalcAngle(player->getPosition(), mBlockPos.ToFloat());
        Vector2<float> mAngleMoonwalk = player->getPosition().CalcAngle(player->getPosition(), mBlockBelow.ToFloat());

        if (event->mPacket->getId() == PacketID::InventoryTransaction && mFlareonV2) 
        {
            auto it = (InventoryTransactionPacket*)event->mPacket;
            if (it->mTransaction->mType == ComplexInventoryTransaction::Type::ItemUseTransaction)
            {
                ItemUseInventoryTransaction* transac = reinterpret_cast<ItemUseInventoryTransaction*>(it->mTransaction.get());
                if (transac->mActionType == ItemUseInventoryTransaction::ActionType::Place)
                    transac->mClickPos = Vector3<float>(0, 0, 0);
            }
        }

        PlayerAuthInputPacket* mPacket = nullptr;
        MovePlayerPacket* mMovePacket = nullptr;

        if (event->mPacket->getId() == PacketID::PlayerAuthInput) 
        {
            mPacket = (PlayerAuthInputPacket*)event->mPacket;
        }

        if (event->mPacket->getId() == PacketID::MovePlayer) 
        {
            mMovePacket = (MovePlayerPacket*)event->mPacket;
        }

        switch (mRotations)
        {
        case 0: // Normal
            PacketUtil::setPlayerAuthInputRotations(mAngleNormal, mPacket);
            PacketUtil::setMovePlayerRotations(mAngleNormal, mMovePacket);
            break;
        case 1: // Moonwalk
            mAngleMoonwalk = getNormAngle(mAngleMoonwalk);

            PacketUtil::setPlayerAuthInputRotations(mAngleMoonwalk, mPacket);
            PacketUtil::setMovePlayerRotations(mAngleMoonwalk, mMovePacket);
            break;
        }
    }

    void onDisabled() override 
    {
        if (!InstanceManager::get<ClientInstance>() || !InstanceManager::getLocalPlayer())
            return;

        InstanceManager::getLocalPlayer()->getSupplies()->mSelectedSlot = mPrevSlot;
    }
};