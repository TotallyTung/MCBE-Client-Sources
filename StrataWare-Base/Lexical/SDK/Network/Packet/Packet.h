#pragma once
#include "../../../Utils/MemoryUtil.h"
#include "../PacketHandlerDispatcherInstance.h"
#include "PacketIDs.h"
#include <string>
#include "../../../Utils/Maths.h"
class Packet {
public:
	CLASS_MEMBER(PacketHandlerDispatcherInstance*, packetHandler, 0x20);
private:
	char pad_0x0[0x30];
public:
	PacketID getId() {
		return MemoryUtil::CallVFunc<1, PacketID>(this);
	}

	std::string getName() {
		return *MemoryUtil::CallVFunc<2, std::string*>(this, std::string());
	}

    virtual size_t getSize() const {
        return sizeof(Packet);
    }

    virtual Packet* clone() const {
        auto size = this->getSize();
        auto* copy = (Packet*)::operator new(size);
        memcpy(copy, this, size);
        return copy;
    }
};
enum class InteractAction
{
    RIGHT_CLICK = 1,
    LEFT_CLICK = 2,
    LEAVE_VEHICLE = 3,
    MOUSEOVER = 4
};

class InteractPacket : public ::Packet
{
public:
    static const PacketID ID = PacketID::Interact;

    // InteractPacket inner types define
    enum class Action : unsigned char {
        Invalid = 0x0,
        StopRiding = 0x3,
        InteractUpdate = 0x4,
        NpcOpen = 0x5,
        OpenInventory = 0x6,
    };

public:
    InteractAction mAction;   // this+0x30
    uint64_t               mTargetId; // this+0x38
    Vec3<float>                mPos;      // this+0x40

    // prevent constructor by default
    InteractPacket& operator=(InteractPacket const&);
    InteractPacket(InteractPacket const&);
    InteractPacket() = default;
};
