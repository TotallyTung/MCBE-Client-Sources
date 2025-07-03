#pragma once
//
// Created by vastrakai on 8/3/2024.
//

class FastStop : public ModuleBase<FastStop>
{
public:
    FastStop() : ModuleBase<FastStop>("FastStop", "Stops movement instantly on key release", ModuleCategory::Movement, 0, false) {
        mNames = {
            {Lowercase, "faststop"},
            {LowercaseSpaced, "fast stop"},
            {Normal, "FastStop"},
            {NormalSpaced, "Fast Stop"}
        };
    }

    bool mWasUsingMoveKeys = false;
    bool mIsUsingMoveKeys = false;

    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
};