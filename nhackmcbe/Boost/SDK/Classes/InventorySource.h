#pragma once
#include <cstdint>

enum class InventorySourceType : int {
    InvalidInventory = -1,
    ContainerInventory = 0,
    GlobalInventory = 1,
    WorldInteraction = 2,
    CreativeInventory = 3,
    NonImplementedFeatureTODO = 99999,
};
enum class ContainerID : uint8_t {
    Invalid = 0xff,
    Inventory = 0,
    First = 1,
    Last = 100,
    Offhand = 119,
    Armor = 120,
    SelectionSlots = 122,
    PlayerUIOnly = 124
};

enum class InventorySourceFlags : uint32_t {
    NoFlag = 0x0,
    WorldInteraction_Random = 0x1,
};

class InventorySource {
public:

    InventorySourceType inventorySourceType; // 0x0
    int containerID; // 0x4
    InventorySourceFlags inventorySourceFlags; // 0x8






	enum class Type {
		Invalid = -1,
		Container,
		Global,
		WorldInteraction,
		Creative,
		UntrackedInteractionUI = 100,
		NotImplemented = 99999
	};

	enum class Flags {
		NoFlag = 0,
		WorldInteraction_Random = 1
	};

	Type type = Type::Invalid;
	ContainerID container = ContainerID::Invalid;
	Flags flags = Flags::NoFlag;


	InventorySource() = default;
	InventorySource(ContainerID id) : container(id) {}
	InventorySource(Type type) : type(type) {}
	InventorySource(Type type, Flags flags) : type(type), flags(flags) {}
	InventorySource(Type type, ContainerID id) : type(type), container(id) {}

	inline bool operator==(InventorySource const& rhs) const {
		return ((this->type == rhs.type) &&
			(this->container == rhs.container) &&
			(this->flags == rhs.flags));
	}
	inline bool operator!=(InventorySource const& rhs) const {
		return !(*this == rhs);
	}
};