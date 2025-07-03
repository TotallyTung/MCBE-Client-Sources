#pragma once

class VirtualReality : public Module
{
public:
    VirtualReality(int keybind = Keyboard::NONE, bool enabled = false) :
        Module("VirtualReality", "Render", "Change your view bobbing (Requires ViewBobbing to be enabled.)", keybind, enabled)
    {
        //registerEnumSetting("Mode", "", { "Meme" }, &mode);
        //registerFloatSetting("Test", "", &viewBobAmount, -1, 1);
    }

    float viewBobAmount = 0.3;
    int mode = 0;

    void onEvent(UpdateEvent* event) override
    {
        if (!Game::GetLocalPlayer())
            return;

        switch (mode)
        {
        case 0: // Meme mode
            Game::GetLocalPlayer()->getMovementProxy()->setBob(viewBobAmount);
            break;
        }
    }
};
