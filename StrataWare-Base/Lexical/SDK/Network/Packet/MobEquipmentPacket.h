#pragma once


#include <cstdint>
#include "Packet.h"
#include "../../Utils/MemoryUtil.h"
#include "../../World/Item/ItemStack.h"

class MobEquipmentPacket : public Packet
{
public:
    CLASS_MEMBER(__int64, mRuntimeId, 0x30);
    CLASS_MEMBER(int, mSlot, 0x98);
    CLASS_MEMBER(int, mSelectedSlot, 0x9C);
    CLASS_MEMBER(uint8_t, mContainerId, 0xA0);
    CLASS_MEMBER(uint8_t, mSlotByte, 0xA1);
    CLASS_MEMBER(uint8_t, mSelectedSlotByte, 0xA2);
    CLASS_MEMBER(uint8_t, mContainerIdByte, 0xA3);

    //__int64    mRuntimeId;        // this+0x30
    //void*      mItem;             // this+0x38
    //int        mSlot;             // this+0x98
    //int        mSelectedSlot;     // this+0x9C
    //uint8_t    mContainerId;      // this+0xA0
    //uint8_t    mSlotByte;         // this+0xA1
    //uint8_t    mSelectedSlotByte; // this+0xA2
    //uint8_t    mContainerIdByte;  // this+0xA3
}; //Size: 0x0170#pragma once