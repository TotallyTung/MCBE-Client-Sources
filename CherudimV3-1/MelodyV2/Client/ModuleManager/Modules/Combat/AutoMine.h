#pragma once
#include "../Module.h"
#include "../../../Client.h"
#include "../Player/PacketMine.h"

#define M_PI 3.14159265358979323846

class AutoMine : public Module {
private:
    Vec2<float> GetRotations(Vec3<float> playerEyePos, Vec3<float> targetPos);
    float lastDestroyRate = 0.f;
    bool countinue = false;
    float Cspeed = 0.1f;
    Vec3<float> possrot;
    float range = 6.f;
    bool silenSwitch = true;
    bool silentBack = true;
    int breakMode = 0;
    int breakKeybind = 0x0;
    Vec3<int> breakPos = Vec3<int>(0, 0, 0);
    uint8_t face = -1;
    std::pair<int, float> getBestPickaxeSlot(Block* block);
    int lastSlot = -1;
    bool shouldSwitchBack = false;

public:
    bool isbeingmine = false;
    AutoMine();
    void Reset();
    virtual void onSendPacket(Packet* packet, bool& shouldCancel);
    virtual void onNormalTick(Actor* actor) override;
    virtual void onRender(MinecraftUIRenderContext* ctx) override;
    virtual void onImGuiRender(ImDrawList* drawlist) override;
};
