#pragma once

class NetworkIdentifier {
public:
    // NetworkIdentifier inner types define
    enum class Type : int {
        RakNet    = 0x0,
        Address   = 0x1,
        Address6  = 0x2,
        NetherNet = 0x3,
        Generic   = 0x4,
    };

    uint64_t           mNetherNetIdValue; // this+0x0
    RakNet::RakNetGUID mGuid;             // this+0x8
    char               mSock[0x80];       // this+0x18
    Type               mType;             // this+0x98
};