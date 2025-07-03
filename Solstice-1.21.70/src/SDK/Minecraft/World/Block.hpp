#pragma once
//
// Created by vastrakai on 7/3/2024.
//

#include <SDK/OffsetProvider.hpp>
#include <Utils/MemUtils.hpp>

#include "BlockSource.hpp"

class Block {
public:
    CLASS_FIELD(class BlockLegacy*, mLegacy, OffsetProvider::Block_mLegacy);


    BlockLegacy* toLegacy() {
        return mLegacy;
    }

    uint32_t getRuntimeId() {
        return hat::member_at<uint32_t>(this, OffsetProvider::Block_mRuntimeId);
    }
};

struct ActorBlockSyncMessage {
public:
    // ActorBlockSyncMessage inner types define
    enum class MessageId : int {
        CREATE  = 0x1,
        DESTROY = 0x2,
    };

    uint64_t                         mEntityUniqueID;
    ActorBlockSyncMessage::MessageId mMessage;
};