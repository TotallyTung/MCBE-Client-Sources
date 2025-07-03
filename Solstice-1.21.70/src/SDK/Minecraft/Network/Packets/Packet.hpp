#pragma once
//
// Created by vastrakai on 6/28/2024.
//


#include <SDK/Minecraft/Network/PacketID.hpp>
#include <string>
#include <Utils/MemUtils.hpp>
#include <memory>


enum class PacketPriority {
    High,
    Immediate,
    Low,
    Medium,
    Count
};

enum class Reliability {
    Reliable,
    ReliableOrdered,
    Unreliable,
    UnreliableSequenced
};

enum class Compressibility {
    Compressible,
    Incompressible
};

class PacketHandlerDispatcher {
public:
    CLASS_FIELD(void*, vtable, 0x0);

    virtual ~PacketHandlerDispatcher() = 0;
    virtual void handle(void* networkIdentifier, void* netEventCallback, std::shared_ptr<class Packet>) = 0;

    uintptr_t getPacketHandler()
    {
        return MemUtils::GetVTableFunction(this, 1);
    }
};

class Packet {
public:
    PacketPriority mPriority = PacketPriority::Low;
    Reliability mReliability = Reliability::Reliable;
    uint64_t mClientSubId = 0;
    PAD(0x8);
    class PacketHandlerDispatcher* mDispatcher = nullptr;
    Compressibility mCompressibility = Compressibility::Compressible;
    PAD(0x4);

    virtual ~Packet();
    virtual PacketID getId() { return PacketID::Unknown; }
    virtual std::string getName();
    virtual void checkSize();
    virtual void write(class BinaryStream&);
    virtual void read(class BinaryStream&);
    virtual void readExtended(class ReadOnlyBinaryStream&);
    virtual void disallowBatching();


};

static_assert(sizeof(Packet) == 0x30);