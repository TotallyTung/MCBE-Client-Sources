#pragma once

class Step : public Module
{
public:
    Step(int keybind = Keyboard::NONE, bool enabled = false) :
        Module("Step", "Movement", "Like step but downwards / upwards", keybind, enabled)
    {
        registerBoolSetting("Reverse", "test", &reverse);

        ingameOnly = true; // cant use keybind unless on the toast screen
    }

    bool reverse = false;

    void onEvent(UpdateEvent* event) override
    {
        if (!Game::GetLocalPlayer())
            return;

        Player* player = Game::GetLocalPlayer();

        StateVectorComponent* state = player->try_get<StateVectorComponent>();
        DirectPlayerMovementProxy* proxy = player->getMovementProxy();

        if (!state)
            return;

        if (reverse) {
            if (player->isOnGround() && state->r_Velocity.y < 0 && !(state->r_Velocity.y < -1))
            {
                state->r_Velocity.y = -1;
            }
        }
        else {
            if (proxy->getHorizontalCollision()) {
                if (player->isOnGround()  && !player->isInWater())
                {
                    auto getpos = player->GetPosition();
                    static Vector3<float> savePos = Vector3<float>(getpos.x, getpos.y + 1, getpos.z);
                    player->SetPos(savePos);
                }
            }
        }
    }
};