//
// Created by vastrakai on 5/15/2024.
//

#pragma once

#include <Prax/SDK/Game/Network/Network.h>
#include <magic_enum.hpp>

class SetTitlePacket : public ::Packet {
public:
    static const PacketID ID = PacketID::SetTitle;

    // SetTitlePacket inner types define
    enum class TitleType : int {
        Clear = 0x0,
        Reset = 0x1,
        Title = 0x2,
        Subtitle = 0x3,
        Actionbar = 0x4,
        Times = 0x5,
        TitleTextObject = 0x6,
        SubtitleTextObject = 0x7,
        ActionbarTextObject = 0x8,
    };

public:
    TitleType   mType;                  // this+0x30
    std::string mTitleText;             // this+0x38
    int         mFadeInTime;            // this+0x58
    int         mStayTime;              // this+0x5C
    int         mFadeOutTime;           // this+0x60
    std::string mXuid = "";             // this+0x68
    std::string mPlatformOnlineId = ""; // this+0x88

    static std::shared_ptr<SetTitlePacket> CreatePacket(TitleType type, const std::string& text, int fadeInTime, int stayTime, int fadeOutTime) {
        auto packet = MinecraftPackets::CreatePacket<SetTitlePacket>();
        packet->mType = type;
        packet->mTitleText = text;
        packet->mFadeInTime = fadeInTime;
        packet->mStayTime = stayTime;
        packet->mFadeOutTime = fadeOutTime;
        return packet;
    }

    std::string_view GetTypeString(){
        return magic_enum::enum_name(mType);
    }
};
