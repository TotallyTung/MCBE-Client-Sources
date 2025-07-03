#pragma once

class ModalFormRequestPacket : public Packet {
public:
    unsigned int mFormId;   // this+0x30
    std::string  mFormJSON; // this+0x38
};