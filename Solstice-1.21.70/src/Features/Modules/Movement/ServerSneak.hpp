#pragma once
//
// Created by vastrakai on 9/2/2024.
//


class ServerSneak : public ModuleBase<ServerSneak>
{
public:
    BoolSetting mCancelActorData = BoolSetting("Cancel Actor Data", "Cancel ActorData packets (Prevents the server from making you sneak client-sidedly)", true);

    ServerSneak() : ModuleBase("ServerSneak", "Sneak server-sidedly", ModuleCategory::Movement, 0, false)
    {
        addSettings(&mCancelActorData);

        mNames = {
            {Lowercase, "serversneak"},
            {LowercaseSpaced, "server sneak"},
            {Normal, "ServerSneak"},
            {NormalSpaced, "Server Sneak"}
        };
    }

    uint64_t mLastInteract = 0;

    void onEnable() override;
    void onDisable() override;
    void onPacketOutEvent(class PacketOutEvent& event);
    void onPacketInEvent(class PacketInEvent& event);
};