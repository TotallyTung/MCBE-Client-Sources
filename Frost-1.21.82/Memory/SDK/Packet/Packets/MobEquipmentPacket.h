#pragma once

class MobEquipmentPacket : public Packet
{
public:
	__int64                      mRuntimeId;        // this+0x30
	NetworkItemStackDescriptor   mItem;             // this+0x38
	//char                         pad_0040[0x60];    // this+0x40 (padding to reach offset 0x98)
	int                          mSlot;             // this+0x98
	int                          mSelectedSlot;     // this+0x9C
	uint8_t                      mContainerId;      // this+0xA0
	uint8_t                      mSlotByte;         // this+0xA1
	uint8_t                      mSelectedSlotByte; // this+0xA2
	uint8_t                      mContainerIdByte;  // this+0xA3
}; 
// Size: 0x0170