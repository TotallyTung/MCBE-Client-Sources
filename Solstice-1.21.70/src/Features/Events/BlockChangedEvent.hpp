//
// Created by vastrakai on 7/28/2024.
//

#pragma once

#include <Utils/Structs.hpp>

#include "Event.hpp"


class BlockChangedEvent : public Event
{
public:
    BlockPos mBlockPos;
    Block* mNewBlock;
    Block* mOldBlock;
    int mEventTarget;
    Actor* mActor;

    explicit BlockChangedEvent(BlockPos blockPos, Block* newBlock, Block* oldBlock, int eventTarget, Actor* who) : Event(), mBlockPos(blockPos), mNewBlock(newBlock), mOldBlock(oldBlock), mEventTarget(eventTarget), mActor(who) {}

    [[nodiscard]] const BlockPos& getBlockPos() const {
        return mBlockPos;
    }

    [[nodiscard]] const Block* getNewBlock() const {
        return mNewBlock;
    }

    [[nodiscard]] const Block* getOldBlock() const {
        return mOldBlock;
    }

    [[nodiscard]] int getEventTarget() const {
        return mEventTarget;
    }

    [[nodiscard]] Actor* getActor() const {
        return mActor;
    }
};