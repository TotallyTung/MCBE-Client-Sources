//
// Created by vastrakai on 5/10/2024.
//

#pragma once

#include <Prax/SDK/Game/Network/Network.h>

enum ScorePacketType : uint8_t {
    Change = 0,
    Remove = 0x1,
};


struct PlayerScoreboardId {
public:
    uint64_t mActorUniqueId; // this+0x0
};





struct ScoreboardId {
public:
    int64_t             mRawId;       // this+0x0
    class IdentityDefinition* mIdentityDef; // this+0x8
};

class IdentityDefinition {
public:
    // IdentityDefinition inner types define
    enum class Type : unsigned char {
        Invalid    = 0x0,
        Player     = 0x1,
        Entity     = 0x2,
        FakePlayer = 0x3,
    };
    ScoreboardId       mScoreboardId;       // this+0x0
    bool               mIsHiddenFakePlayer; // this+0x10
    PlayerScoreboardId mPlayerId;           // this+0x18
    int64_t            mEntityId;           // this+0x20
    std::string        mPlayerName;         // this+0x28
    Type               mIdentityType;       // this+0x48
};

struct ScorePacketInfo {
public:
    ScoreboardId             mScoreboardId;   // this+0x0
    std::string              mObjectiveName;  // this+0x10
    int                      mScoreValue;     // this+0x30
    IdentityDefinition::Type mIdentityType;   // this+0x34
    PlayerScoreboardId       mPlayerId;       // this+0x38
    int64_t                  mEntityId;       // this+0x40
    std::string              mFakePlayerName; // this+0x48

    // NOLINTEND
};



class SetScorePacket : public ::Packet {
public:
    static const PacketID ID = PacketID::SetScore;

    ScorePacketType              mType;      // this+0x30
    std::vector<ScorePacketInfo> mScoreInfo; // this+0x38
};