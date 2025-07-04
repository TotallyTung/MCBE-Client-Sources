#pragma once
#include "../../Utils/MemoryUtils.h"
#include "NetworkItemStackDescriptor.h"

class ComplexInventoryTransaction {
private:
    uintptr_t** VTable;
public:
    enum class Type : int {
        NormalTransaction = 0x0,
        InventoryMismatch = 0x1,
        ItemUseTransaction = 0x2,
        ItemUseOnEntityTransaction = 0x3,
        ItemReleaseTransaction = 0x4,
    };
    Type type; // 0x8
    __int64 data; // 0xC
};
class ItemUseOnActorInventoryTransaction : public ComplexInventoryTransaction {
public:
	enum class ActionType : int32_t {
		Interact = 0,
		Attack = 1,
		ItemInteract = 2,
	};

	int64_t actorId{};
	ActionType actionType{};
	int32_t slot{};
	NetworkItemStackDescriptor itemInHand{};
	Vec3<float> playerPos{}, clickPos{};

	ItemUseOnActorInventoryTransaction() = default;
	ItemUseOnActorInventoryTransaction(ActionType action, int64_t rid, int32_t slot, ItemStack* item, Vec3<float> pos, Vec3<float> clickPos = { 0.f, 0.f, 0.f }) : ItemUseOnActorInventoryTransaction() {
		this->actorId = rid;
		this->actionType = action;
		this->slot = slot;
		this->itemInHand.fromStack(item);
		this->playerPos = pos;
		this->clickPos = clickPos;
	}
};


//itemuseon entites sigs 48 89 5c 24 ? 57 48 83 ec ? 48 8d 59 ? c7 41 ? ? ? ? ? 48 8d 05 ? ? ? ? 48 89 5c 24 ? 48 89 01 48 8b f9 48 8b cb e8 ? ? ? ? 33 c9 48 8d 05 ? ? ? ? 48 89 4b ? 0f 57 c0 48 89 4b ? 48 89 4b ? 48 8b 5c 24 ? 48 89 07 48 8d 05 ? ? ? ? 48 89 4f ? 89 4f