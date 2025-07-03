#pragma once

class ItemStackBase {
public:
    uintptr_t**                           mVfTable;        // this+0x00
    class Item**                          mItem;           // this+0x08
    void*                                 mCompoundTag;    // this+0x10
    class Block*                          mBlock;          // this+0x18
    short                                 mAuxValue;       // this+0x20
    int8_t                                mCount;          // this+0x22
    bool                                  mValid;          // this+0x23
    std::chrono::steady_clock::time_point mPickupTime;     // this+0x28
    bool                                  mShowPickUp;     // this+0x30
    bool                                  mWasPickedUp;    // this+0x31
    std::vector<class BlockLegacy const*> mCanPlaceOn;     // this+0x38
    uint64_t                              mCanPlaceOnHash; // this+0x50
    std::vector<class BlockLegacy const*> mCanDestroy;     // this+0x58
    uint64_t                              mCanDestroyHash; // this+0x70
    Tick                                  mBlockingTick;   // this+0x78
    std::unique_ptr<class ItemInstance>   mChargedItem;    // this+0x80
    //char         pad_0x0024[0x60]; // this+0x0024

    Item* getItem() {
        return *mItem;
    }
};