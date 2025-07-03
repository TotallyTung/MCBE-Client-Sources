#pragma once

struct PlayerPositionModeComponent {
public:
    // PlayerPositionModeComponent inner types define
    enum class PositionMode : unsigned char {
        Normal = 0,
        Respawn = 1,
        Teleport = 2,
        OnlyHeadRot = 3,
    };

    PositionMode mPositionMode; // this+0x0
};