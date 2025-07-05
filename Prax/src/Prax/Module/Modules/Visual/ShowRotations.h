#pragma once
//
// Created by vastrakai on 9/3/2023.
//


#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include <Prax/Module/Settings/BoolSetting.h>
#include <imgui_internal.h>
#include <Prax/Module/Settings/ModeSetting.h>


class ShowRotations : public ModuleBase<ShowRotations> {
public:
    ModeSetting Mode = ModeSetting("Mode", "The mode to use for regen", {"PlayerAuthInput" , "MovePlayer" }, 0);
    BoolSetting Easing = BoolSetting("Easing", "Eases the rotations", true);
    NumberSetting YawEasingSpeed = NumberSetting("Yaw Easing Speed", "Easing Speed", 0.01f, 20, 2, 0.1);
    NumberSetting HeadYawEasingSpeed = NumberSetting("Head Yaw Easing Speed", "Easing Speed", 0.01f, 20, 2, 0.1);
    NumberSetting PitchEasingSpeed = NumberSetting("Pitch Easing Speed", "Easing Speed", 0.01f, 20, 2, 0.1);
    NumberSetting BodyYawEasingSpeed = NumberSetting("Body Yaw Easing Speed", "Easing Speed", 0.01f, 20, 2, 0.1);
    BoolSetting LockBodyYaw = BoolSetting("Lock Body Yaw", "Locks the body yaw to the head yaw", true);

    ShowRotations() : ModuleBase("ShowRotations", "Shows normally invisible server-sided rotations", "Visual") {
        PacketPriority = Priority::AbsoluteLast;
        AddSetting(&Mode);
        AddSetting(&Easing);
        AddSetting(&YawEasingSpeed);
        AddSetting(&HeadYawEasingSpeed);
        AddSetting(&PitchEasingSpeed);
        AddSetting(&BodyYawEasingSpeed);
        AddSetting(&LockBodyYaw);
    }
    static float Yaw;
    static float HeadYaw;
    static float Pitch;
    static float BodyYaw;
    static float OldYaw;
    static float OldHeadYaw;
    static float OldPitch;
    static float OldBodyYaw;
    static float LerpedYaw;
    static float LerpedHeadYaw;
    static float LerpedPitch;
    static float LerpedBodyYaw;

    void OnEnable() override;
    void OnDisable() override;
    void OnTick() override;
    void OnPacket(LoopbackPacketSender *lbps, Packet *packet, bool *canceled) override;
    void OnRender() override;



};

class BodyYaw
{
public:
    static inline float bodyYaw = 0.f;
    static inline Vector3 posOld = Vector3(0, 0, 0);
    static inline Vector3 pos = Vector3(0, 0, 0);

    static inline void updateRenderAngles(Actor* plr, float headYaw)
    {
        posOld = pos;
        pos = *plr->getPos();

        float diffX = pos.x - posOld.x;
        float diffZ = pos.z - posOld.z;
        float diff = diffX * diffX + diffZ * diffZ;

        float body = bodyYaw;
        if (diff > 0.0025000002F)
        {
            float anglePosDiff = atan2f(diffZ, diffX) * 180.f / 3.14159265358979323846f - 90.f;
            float degrees = abs(wrapAngleTo180_float(headYaw) - anglePosDiff);
            if (95.f < degrees && degrees < 265.f)
            {
                body = anglePosDiff - 180.f;
            }
            else
            {
                body = anglePosDiff;
            }
        }

        turnBody(body, headYaw);
    };

    static inline void turnBody(float bodyRot, float headYaw)
    {
        float amazingDegreeDiff = wrapAngleTo180_float(bodyRot - bodyYaw);
        bodyYaw += amazingDegreeDiff * 0.3f;
        float bodyDiff = wrapAngleTo180_float(headYaw - bodyYaw);
        if (bodyDiff < -75.f)
            bodyDiff = -75.f;

        if (bodyDiff >= 75.f)
            bodyDiff = 75.f;

        bodyYaw = headYaw - bodyDiff;
        if (bodyDiff * bodyDiff > 2500.f)
        {
            bodyYaw += bodyDiff * 0.2f;
        }
    };

    static inline float wrapAngleTo180_float(float value)
    {
        value = fmodf(value, 360.f);

        if (value >= 180.0F)
        {
            value -= 360.0F;
        }

        if (value < -180.0F)
        {
            value += 360.0F;
        }

        return value;
    };
};