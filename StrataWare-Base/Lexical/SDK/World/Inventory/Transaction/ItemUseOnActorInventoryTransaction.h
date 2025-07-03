#pragma once
#include "ComplexInventoryTransaction.h"
#include "../../ActorRuntimeID.h"
#include "../../Item/NetworkItemStackDescriptor.h"

class ItemUseOnActorInventoryTransaction : public ComplexInventoryTransaction {
public:
	enum class ActionType : int32_t {
		Interact = 0,
		Attack = 1,
		ItemInteract = 2,
	};

	ActorRuntimeID actorRuntimeId;
	ActionType actionType;
	int32_t slot;
	NetworkItemStackDescriptor itemInHand;
	Vec3<float> playerPos;
	Vec3<float> clickPos;

	static std::unique_ptr<ItemUseOnActorInventoryTransaction> make_unique() {
	}
};