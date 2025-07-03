#pragma once
#include "Packet.h"
#include <vector>
#include <string>

enum class TextPacketType : uint8_t {
    Raw = 0x0,
    Chat = 0x1,
    Translate = 0x2,
    Popup = 0x3,
    JukeboxPopup = 0x4,
    Tip = 0x5,
    SystemMessage = 0x6,
    Whisper = 0x7,
    Announcement = 0x8,
    TextObjectWhisper = 0x9,
    TextObject = 0xA,
    TextObjectAnnouncement = 0xB,
};

class TextPacket : public Packet {
public:
    TextPacketType           mType;            // this+0x30
    std::string              mAuthor;          // this+0x38
    std::string              mMessage;         // this+0x58
    std::string              mFilteredMessage; // this+0x78
    bool                     mUnknown;         // this+0x98
    std::vector<std::string> mParams;          // this+0xA0
    bool                     mLocalize;        // this+0x88
    std::string              mXuid;            // this+0xC0
    std::string              mPlatformId;      // this+0xE0
};
