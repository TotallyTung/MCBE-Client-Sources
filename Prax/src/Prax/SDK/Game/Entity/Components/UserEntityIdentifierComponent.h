//
// Created by vastrakai on 1/5/2024.
//

#pragma once
#include <Prax/SDK/Game/Entity/Level.h>

enum class SubClientId : unsigned char {
    PrimaryClient    = 0x0,
    Client2          = 0x1,
    Client3          = 0x2,
    Client4          = 0x3,
    ExtraIdSlotStart = 0x64,
    EditorUI         = 0x65,
};

struct RakNetGUID
{
public:
    uint64_t g;
    unsigned short systemIndex;
};

class NetworkIdentifier
{
    enum class Type : int {
        RakNet    = 0x0,
        Address   = 0x1,
        Address6  = 0x2,
        NetherNet = 0x3,
        Generic   = 0x4,
    };

    uint64_t           mNetherNetIdValue; // this+0x0
    RakNetGUID         mGuid;             // this+0x8
    char               mSock[0x80];       // this+0x18
    Type               mType;             // this+0x98
};

struct UserEntityIdentifierComponent {
public:
    class NetworkIdentifier            mNetworkId;   // this+0x0
    SubClientId                        mClientSubId; // this+0xA0
    mceUUID                            mClientUUID;  // this+0xA8
    std::string                        mPlayFabId;   // this+0xB8
    char pad_0xE0[0x8];                             // this+0xE0 (This would normally be std::unique_ptr<class Certificate> mCertificate)
}; // Size: 0xE0

// Size assert
static_assert(sizeof(UserEntityIdentifierComponent) == 0xE0, "UserEntityIdentifierComponent wrong size");