#pragma once

struct BlockInfo 
{
    Block* mBlock;
    Vector3<int> mPosition;

    AABB getAABB() 
    {
        return AABB(mPosition.ToFloat(), Vector3<float>(1, 1, 1));
    }

    BlockInfo(Block* block, Vector3<int> position) : mBlock(block), mPosition(position) {}
};

class BlockUtil 
{
public:
    static std::vector<BlockInfo> getBlockList(const Vector3<int>& position, float r)
    {
        std::vector<BlockInfo> blocks = {};

        const auto mBlockSource = InstanceManager::getBlockSource();

        auto newBlocks = std::vector<BlockInfo>();


        const int radius = static_cast<int>(r);
        newBlocks.reserve(radius * radius * radius); // reserve enough space for all blocks

        for (int x = position.x - radius; x <= position.x + radius; x++)
            for (int y = position.y - radius; y <= position.y + radius; y++)
                for (int z = position.z - radius; z <= position.z + radius; z++)
                    if (const auto mBlock = mBlockSource->getBlock({ x, y, z }))
                        newBlocks.push_back({ mBlock, { x, y, z } });


        return newBlocks;
    }

    static int getExposedBlockFace(Vector3<int> blockPos, bool useBlockFilter = false) 
    {
        BlockSource* mSource = InstanceManager::getBlockSource();
        if (!mSource) return false;

        static std::vector<Vector3<int>> checklist = {
            Vector3<int>(0, 1, 0), Vector3<int>(0, -1, 0),
            Vector3<int>(0, 0, 1), Vector3<int>(0, 0, -1),
            Vector3<int>(1, 0, 0), Vector3<int>(-1, 0, 0),
        };
        for (int i = 0; i < checklist.size(); i++) {
            Block* block = mSource->getBlock(blockPos.add(checklist[i]));
            if (block->getLegacyBlock()->getBlockID() == 0) return i;
            if (useBlockFilter && !block->getLegacyBlock()->isSolid()) return i;
        }
        return -1;
    }

    static bool isAirBlock(Vector3<int> blockPos) 
    {
        Block* mBlock = InstanceManager::getBlockSource()->getBlock(blockPos);
        if (!mBlock) return false;

        return mBlock->getLegacyBlock()->getBlockID() == 0;
    }

    static void startDestroyBlock(Vector3<int> pos, int side)
    {
        auto player = InstanceManager::getLocalPlayer();
        Vector3<int> blockPos = pos.ToInt();
        if (side == -1) side = getExposedBlockFace(blockPos);

        bool isDestroyedOut = false;
        player->getGameMode()->startDestroyBlock(blockPos, side, isDestroyedOut);
    }

    static constexpr int AIR_RUNTIME_ID = 3690217760;

    static void destroyBlock(Vector3<int> pos, int side, bool useTransac)
    {
        auto player = InstanceManager::getLocalPlayer();
        Vector3<int> blockPos = pos.ToInt();
        if (side == -1) side = 0;

        if (!useTransac)
        {
            player->getGameMode()->destroyBlock(blockPos, side);
            player->getGameMode()->stopDestroyBlock(blockPos);
            return;
        }

        player->getGameMode()->destroyBlock(blockPos, side);
        player->getGameMode()->stopDestroyBlock(blockPos);
        return;

        /*auto actionPkt = MinecraftPackets::createPacket<PlayerActionPacket>();
        actionPkt->mPos = blockPos;
        actionPkt->mResultPos = blockPos;
        actionPkt->mFace = side;
        actionPkt->mAction = PlayerActionType::StopDestroyBlock;
        actionPkt->mRuntimeId = player->getRuntimeID();
        actionPkt->mtIsFromServerPlayerMovementSystem = false;

        PacketUtils::queueSend(actionPkt, false);
        auto pkt = MinecraftPackets::createPacket<InventoryTransactionPacket>();

        auto cit = std::make_unique<ItemUseInventoryTransaction>();
        cit->mActionType = ItemUseInventoryTransaction::ActionType::Destroy;
        int slot = player->getSupplies()->mSelectedSlot;
        cit->mSlot = slot;
        cit->mItemInHand = NetworkItemStackDescriptor(*player->getSupplies()->getContainer()->getItem(slot));
        cit->mBlockPos = blockPos;
        cit->mFace = side;
        cit->mTargetBlockRuntimeId = 0x92;
        cit->mPlayerPos = *player->getPos();

        cit->mClickPos = glm::vec3(0, 0, 0); // this is correct, i actually checked it this time
        pkt->mTransaction = std::move(cit);
        PacketUtils::queueSend(pkt, false);
        clearBlock(blockPos);*/
    }

    static void placeBlock(Vector3<int> pos, int side)
    {
        auto player = InstanceManager::getLocalPlayer();
        Vector3<int> blockPos = pos.ToInt();
        if (side == -1) side = 0;

        static std::vector<Vector3<int>> offsetlist = {
            Vector3<int>(0, 1, 0), Vector3<int>(0, -1, 0),
            Vector3<int>(0, 0, 1), Vector3<int>(0, 0, -1),
            Vector3<int>(1, 0, 0), Vector3<int>(-1, 0, 0),
        };

        player->getGameMode()->buildBlock(blockPos.add(Vector3<int>(offsetlist[side])), side, true);
    }
};