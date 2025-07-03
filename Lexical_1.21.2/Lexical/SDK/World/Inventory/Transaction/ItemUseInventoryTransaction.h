#pragma once
#include "ComplexInventoryTransaction.h"
#include "../../Item/NetworkItemStackDescriptor.h"

class ItemUseInventoryTransaction : public ComplexInventoryTransaction {
public:
	enum class ActionType : int32_t {
		Place = 0,
		Use = 1,
		Destroy = 2,
	};

	ActionType actionType;
	BlockPos blockPos;
	uint32_t targetBlockRuntimeId;
	uint8_t face;
	int64_t slot;
	NetworkItemStackDescriptor itemInHand;
	Vec3<float> playerPos;
	Vec3<float> clickPos;

	static std::unique_ptr<ItemUseInventoryTransaction> make_unique() {
	}
};