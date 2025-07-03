#pragma once

class Disabler : public Module {
public:
    Disabler(int keybind = 7, bool enabled = true) :
        Module("Disabler", "Misc", "Disable da anticheat!", keybind, enabled)
    {
        addEnum("Mode", "The mode of which the disabler will work", { "Cancel" }, &mode);
    }

    int mode = 0;
    int setTick = 0;
    int countPacket = 0;
    float fakeTimer = 0.4;
    bool extraSecurity = false;
    bool testing = false;
    bool onlyFly = true;
    bool onlyAuth = false;
    bool debug = false;
    bool showmode = true;
    bool isEnabledFly = false;
    bool ignore = false;

    Vector3<float> savedPos = Vector3<float>(0, 0, 0);
    Vector3<float> oldPos = Vector3<float>(0, 0, 0);

    void onEnabled() override {
        ClientInstance* instance = Address::getClientInstance();
        LoopbackPacketSender* sender = instance->getLoopbackPacketSender();

        if (!instance->getLocalPlayer()) {
            return;
        }

        ignore = false;
    }

    inline int randomInt(int a, int b) {
        int random = ((int)rand()) / (int)RAND_MAX;
        int diff = b - a;
        return a + random * diff;
    }

    void onEvent(ActorBaseTickEvent* event) override {
        ClientInstance* instance = Address::getClientInstance();
        LoopbackPacketSender* sender = instance->getLoopbackPacketSender();

        if (!instance->getLocalPlayer() && !instance->getMinecraftGame()->CanUseKeys) {
            return;
        }

        if (mode == 0) { // Cancel
            {
                std::shared_ptr<Packet> packet = MinecraftPackets::createPacket((int)PacketID::NetworkStackLatency);
                auto* pkt = reinterpret_cast<NetworkStackLatencyPacket*>(packet.get());
                pkt->mCreateTime = randomInt(1, 14);
                if (TimeUtils::hasTimeElapsed("FlareonPacket", 9999999999999999999, true)) {
                    sender->sendToServer(pkt);
                }
            }

        }
    }

    void onEvent(RenderContextEvent* event) override {
        if (!Address::getLocalPlayer()) {
            return;
        }
        // Additional code for rendering, if any
    }

    void addFlagFromName(__int64& inputFlags, InputData flag) {
        inputFlags |= static_cast<__int64>(flag);
    }

    void removeFlagFromName(__int64& inputFlags, InputData flag) {
        inputFlags &= ~static_cast<__int64>(flag);
    }

    std::vector<std::string> getFlagNames(__int64 flags) {
        std::unordered_map<__int64, std::string> flagNames = {
            {1ll << 0, "Ascend"}, {1ll << 1, "Descend"}, {1ll << 2, "NorthJump"},
            {1ll << 3, "JumpDown"}, {1ll << 4, "SprintDown"}, {1ll << 5, "ChangeHeight"},
            {1ll << 6, "Jumping"}, {1ll << 7, "AutoJumpingInWater"}, {1ll << 8, "Sneaking"},
            {1ll << 9, "SneakDown"}, {1ll << 10, "Up"}, {1ll << 11, "Down"},
            {1ll << 12, "Left"}, {1ll << 13, "Right"}, {1ll << 14, "UpLeft"},
            {1ll << 15, "UpRight"}, {1ll << 16, "WantUp"}, {1ll << 17, "WantDown"},
            {1ll << 18, "WantDownSlow"}, {1ll << 19, "WantUpSlow"}, {1ll << 20, "Sprinting"},
            {1ll << 21, "AscendBlock"}, {1ll << 22, "DescendBlock"}, {1ll << 23, "SneakToggleDown"},
            {1ll << 24, "PersistSneak"}, {1ll << 25, "StartSprinting"}, {1ll << 26, "StopSprinting"},
            {1ll << 27, "StartSneaking"}, {1ll << 28, "StopSneaking"}, {1ll << 29, "StartSwimming"},
            {1ll << 30, "StopSwimming"}, {1ll << 31, "StartJumping"}, {1ll << 32, "StartGliding"},
            {1ll << 33, "StopGliding"}, {1ll << 34, "PerformItemInteraction"}, {1ll << 35, "PerformBlockActions"},
            {1ll << 36, "PerformItemStackRequest"}, {1ll << 37, "HandledTeleport"}, {1ll << 38, "Emoting"},
            {1ll << 39, "MissedSwing"}, {1ll << 40, "StartCrawling"}, {1ll << 41, "StopCrawling"},
            {1ll << 42, "StartFlying"}, {1ll << 43, "StopFlying"}, {1ll << 44, "ReceivedServerData"},
            {1ll << 45, "InClientPredictedInVehicle"}, {1ll << 46, "PaddlingLeft"}, {1ll << 47, "PaddlingRight"},
        };

        std::vector<std::string> activeFlags;

        for (int i = 0; i < 64; ++i) {
            __int64 bit = 1ll << i;
            if ((flags & bit) != 0) {
                auto it = flagNames.find(bit);
                if (it != flagNames.end()) {
                    activeFlags.push_back(it->second);
                }
            }
        }

        return activeFlags;
    }

    void onDisabled() override {
    }
};