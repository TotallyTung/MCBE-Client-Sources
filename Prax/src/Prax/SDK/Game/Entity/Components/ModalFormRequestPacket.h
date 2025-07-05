//
// Created by vastrakai on 3/2/2024.
//

#pragma once

#include <Prax/SDK/Game/Network/Network.h>

class ModalFormRequestPacket : public Packet {
public:
    unsigned int mFormId;   // this+0x30
    std::string mFormJSON; // this+0x38
};

enum class ModalFormCancelReason : signed char {
    UserClosed = 0x0,
    UserBusy   = 0x1,
};

class ModalFormResponsePacket : public ::Packet {
public:
    unsigned int mFormId;           // this+0x30
    //Json::Value mJSONResponse; // this+0x38 this is what the game defines, we need to use the Nlohmann json library instead
    std::string mJSONResponse; // this+0x38
    std::optional<ModalFormCancelReason> mFormCancelReason; // this+0x50
};
