#pragma once

class ModalFormResponsePacket : public Packet {
public:
    unsigned int                         mFormId;           // this+0x30
    std::optional<Json::Value>           mJSONResponse;     // this+0x38
    std::optional<ModalFormCancelReason> mFormCancelReason; // this+0x50
};