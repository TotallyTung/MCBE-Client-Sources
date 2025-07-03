#pragma once

class ChatSuffix : public Module
{
private:
    int fonts = 0;

public:
    ChatSuffix(int keybind = Keyboard::NONE, bool enabled = false) :
        Module("ChatSuffix", "Misc", "Add suffix", keybind, enabled)
    {
        registerEnumSetting("Font", "The type of font", { "Normal" }, &fonts);
    }

    void removeTrailingSpace(std::string& str) {
        size_t endpos = str.find_last_not_of(" \t");
        if (std::string::npos != endpos) {
            str = str.substr(0, endpos + 1);
        }
        else {
            str.clear();
        }
    }

    void onEvent(PacketSendingEvent* event) override
    {
        auto player = Game::GetLocalPlayer();
        if (player == nullptr) return;

        auto packet = event->Packet;

        if (packet->instanceOf<TextPacket>()) {
            TextPacket* textpacket = reinterpret_cast<TextPacket*>(packet);
            std::string Sentence;
            std::string end;
            switch (fonts) {
            case 0: // Normal
                end = " | " + Game::Skiddle::ClientName;
            }

            std::string text = textpacket->message.getText();
            removeTrailingSpace(text);
            Sentence = text + end;
            textpacket->message.resetWithoutDelete();
            textpacket->message = Sentence;
        }
    }
};