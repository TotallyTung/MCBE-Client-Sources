#pragma once
#include "../ContainerID.h"

enum class InventorySourceType : int32_t {
    InvalidInventory = -1,
    ContainerInventory = 0,
    GlobalInventory = 1,
    WorldInteraction = 2,
    CreativeInventory = 3,
    NonImplementedFeatureTODO = 99999,
};

enum class InventorySourceFlags : uint32_t {
    NoFlag = 0x0,
    WorldInteraction_Random = 0x1,
};

class InventorySource {
public:
    InventorySourceType type = InventorySourceType::InvalidInventory;
    ContainerID containerId = ContainerID::None;
    InventorySourceFlags flags = InventorySourceFlags::NoFlag;

    inline bool operator==(InventorySource const& rhs) const {
        return ((this->type == rhs.type) &&
            (this->containerId == rhs.containerId) &&
            (this->flags == rhs.flags));
    }

    inline bool operator!=(InventorySource const& rhs) const {
        return !(*this == rhs);
    }
};

namespace std {
    template <>
    struct hash<InventorySource> {
        size_t operator()(InventorySource const& key) const {
            return static_cast<size_t>(key.containerId)
                ^ (static_cast<size_t>(static_cast<unsigned int>(key.type)) << 16);
        }
    };
}