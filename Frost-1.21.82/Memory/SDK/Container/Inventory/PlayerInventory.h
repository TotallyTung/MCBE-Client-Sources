#pragma once

class PlayerInventory
{
public:
	CLASS_FIELD(int, mSelectedSlot, 0x10);
	CLASS_FIELD(int, mInHandSlot, 0x1C);
public:
	Inventory* getInventory() {
		return hat::member_at<Inventory*>(this, 0xB8); // 1.21.60
	}
};