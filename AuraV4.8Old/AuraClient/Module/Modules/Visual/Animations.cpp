#include "Animations.h"

Animations::Animations() : IModule(0x0, Category::VISUAL, "Slows down swing speed") {
    registerFloatSetting("SwingSpeed", &swingspeed, swingspeed, 1.f, 20.f);
}

Animations::~Animations() {
}

const char* Animations::getModuleName() {
    return ("Animations");
}
void Animations::onTick(C_GameMode* gm) {
    SwingSpeedIndex += swingspeed;
}

void Animations::onPlayerTick(C_Player* plr) {
    static unsigned int offset = *reinterpret_cast<int*>(FindSignature("80 BB ? ? ? ? 00 74 1A FF 83") + 2);
    float* stopswing = reinterpret_cast<float*>((uintptr_t)g_Data.getLocalPlayer() + offset);
    *stopswing = 0;
    if (SwingSpeedIndex >= 100) {
        useSwong = false;
        SwingSpeedIndex = 0;
    }
    if (useSwong) {
        std::vector<float> SwingArray;
        for (int i = 0; i < 100; i++)
            SwingArray.push_back(i * 0.01);
        if (SwingSpeedIndex <= 100) {
            float SwingSpeedArray = SwingArray[SwingSpeedIndex];
            float* speedAdr = reinterpret_cast<float*>(reinterpret_cast<__int64>(g_Data.getLocalPlayer()) + 0x79C);
            *speedAdr = SwingSpeedArray;
        }
    }
    else {
        SwingSpeedIndex = 0;
    }
}
void Animations::onSendPacket(C_Packet* packet) {
    if (packet->isInstanceOf<C_AnimatePacket>()) {
        useSwong = true;
    }
}