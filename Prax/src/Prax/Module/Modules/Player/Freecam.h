//
// Created by vastrakai on 10/21/2023.
//


#pragma once

#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/NumberSetting.h>

class Freecam : public ModuleBase<Freecam> {
public:
    BoolSetting RenderLastPos = BoolSetting("Render Last Position", "Renders the last position of the player", true);
    BoolSetting Flight = BoolSetting("Flight", "Allows you to fly", true);
    NumberSetting Speed = NumberSetting("Speed", "How fast you should go", 0, 30, 7.5f, 0.01, &Flight);
    BoolSetting DisableOnLagback = BoolSetting("Disable On Lagback", "Disables freecam when you lagback", true);


    Freecam() : ModuleBase("Freecam", "Lets you move around freely", "Player", 0, false) {
        AddSetting(&RenderLastPos);
        AddSetting(&Flight);
        AddSetting(&Speed);
        AddSetting(&DisableOnLagback);
    }

    /*float realOldPitch = actorRotations->oldPitch;
        float realPitch = actorRotations->pitch;
        float realHeadRot = actorHeadRotations->headRot;
        float realOldHeadRot = actorHeadRotations->oldHeadRot;
        float realBodyYaw = mobBodyRot->yBodyRot;
        float realOldBodyYaw = mobBodyRot->yOldBodyRot;*/
    static AABB LastAABB;
    static Vector3 LastPos;
    static Vector3 LastOldPos;
    static Vector3 LastVel;
    static Vector2 LastYaw;
    static Vector2 LastPitch;
    static Vector2 LastHeadRot;
    static Vector2 LastBodyRot;

    void OnEnable() override;
    void OnDisable() override;
    void OnTick() override;
    void OnPacket(LoopbackPacketSender *lbps, Packet *packet, bool *canceled) override;
    void OnPacketReceive(std::shared_ptr<Packet> packet, bool* cancel) override;
    void OnRender();



};