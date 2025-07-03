#pragma once

class InvMove : public Module
{
public:
    InvMove(int keybind = Keys::NONE, bool enabled = false) :
        Module("InvMove", "Player", "Allow you to move while in Inventory's GUI", keybind, enabled)
    {
        
    }

    float speed = 0.31f;
    bool mHasOpenContainer = false;
    float height = 0.42f;

    void onEnabled() {
        auto player = Address::getLocalPlayer();
    }

    void onDisabled() {
        auto player = Address::getLocalPlayer();
    }

    void onEvent(RenderContextEvent* event) override
    {
        if (!Address::getLocalPlayer())
            return;

        Player* player = Address::getLocalPlayer();

        MovementInterpolatorComponent* movement = player->getMovementInterpolator();
        StateVectorComponent* stateVec = player->getStateVector();

        if (!movement || !stateVec)
            return;

        bool w = Global::Keymap[Keys::W];
        bool a = Global::Keymap[Keys::A];
        bool s = Global::Keymap[Keys::S];
        bool d = Global::Keymap[Keys::D];



        bool pressed = !Address::canUseKeys() && (w || s || d || a);
        auto calcYaw = movement->Rotations.y;

        updateYaw(w, a, s, d, calcYaw);

        auto calcYawF = calcYaw * (3.1415927f / 180);
        if (pressed)
        {
            if (player->isOnGround() && Global::Keymap[Keys::SPACEBAR])
                player->jumpFromGround();

            stateVec->Velocity.x = cos(calcYawF) * (speed / 1.5);
            stateVec->Velocity.z = sin(calcYawF) * (speed / 1.5);
        }
    }

    void updateYaw(bool w, bool a, bool s, bool d, float& calcYaw)
    {
        if (w)
        {
            if (!a && !d)
                calcYaw += 90.f;
            else if (a)
                calcYaw += 45.f;
            else if (d)
                calcYaw += 135.f;
        }
        else if (s)
        {
            if (!a && !d)
                calcYaw -= 90.f;
            else if (a)
                calcYaw -= 45.f;
            else if (d)
                calcYaw -= 135.f;
        }
        else if (!w && !s)
        {
            if (!a && d)
                calcYaw += 180.f;
        }
        if (w && s) {
            calcYaw -= 90.f;
        }
    }

    void onEvent(PacketEvent* event) override {
        auto player = Address::getLocalPlayer();
        event->Packet->getId() == PacketID::PlayerAuthInput;

        if (event->Packet->getId() == PacketID::ContainerOpen)
        {
            auto packet = event->Packet->getId() == PacketID::ContainerOpen;
            mHasOpenContainer = true;
        }
        if (event->Packet->getId() == PacketID::ContainerClose)
        {
            mHasOpenContainer = false;
        }

        if (event->Packet->getId() == PacketID::ContainerClose)
        {
            mHasOpenContainer = false;
        }
        else if (event->Packet->getId() == PacketID::ContainerOpen)
        {
            auto packet = event->Packet->getId() == PacketID::ContainerOpen;
            mHasOpenContainer = true;
        }
    }
};