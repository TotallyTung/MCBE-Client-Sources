#include "../../../Utils/MemoryUtil.h"
#include "../../Core/WeakPtr.h"
#include "Item.h"
#include <chrono>
#include <vector>
#include <optional>

class ItemStackBase {
public:
    uintptr_t** Vtable;
    WeakPtr<Item> item;
    __int64 mUserData;
    __int64 mBlock;
    short mAuxValue;
    uint8_t mCount;
    bool mValid;
    std::chrono::steady_clock::time_point mPickupTime;
    bool mShowPickUp;
    bool mWasPickedUp;
    std::vector<__int64> mCanPlaceOn;
    uint64_t mCanPlaceOnHash;
    std::vector<__int64> mCanDestroy;
    uint64_t mCanDestroyHash;
    __int64 mBlockingTick;
    __int64 mChargedItem;

    [[nodiscard]] inline bool isValid() const {
        return (item.get() != nullptr);
    }

    bool operator==(const ItemStackBase& other) const {
        return (this->item.get() == other.item.get()) && (this->mCount == other.mCount);
    }

    bool operator!=(const ItemStackBase& other) const {
        return !(*this == other); 
    }

public:
    void setNull(std::optional<std::string> reason) {
        MemoryUtil::CallVFunc<4, void, std::optional<std::string>>(this, reason);
    }

    std::string toString() {
        return *MemoryUtil::CallVFunc<5, std::string*>(this, std::string());
    }

    bool isBlock() {
        using func_t = bool(__thiscall*)(ItemStackBase*);
        static func_t Func = (func_t)Addresses::ItemStackBase_isBlock;
        return Func(this);
    }
};
