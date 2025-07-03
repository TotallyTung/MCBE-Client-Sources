#pragma once

class TextPacket : public Packet
{
public:
    TextPacket() {
        init();
    };

    unsigned __int8 messageType;
    unsigned char gap[7];
    TextHolder sourceName;
    TextHolder message;
    unsigned char field_unknown[24];
    bool translationNeeded = false;
    unsigned char gap2[7];
    TextHolder xboxUserId;
    TextHolder platformChatId;

    void init()
    {
        static void* sig = nullptr;
        if (!sig) sig = findSig("48 8D 05 ? ? ? ? 48 89 44 24 ? 48 8D 4D ? E8 ? ? ? ? 48 8D 4D ? E8 ? ? ? ? 48 8D 4D ? E8 ? ? ? ? 48 8D 4D ? E8 ? ? ? ? 48 8D 4D ? E8 ? ? ? ? 48 8D 4D ? E8 ? ? ? ? 48 8B 4D");
        this->setVTable<TextPacket>(reinterpret_cast<uintptr_t*>(sig));
    }
};