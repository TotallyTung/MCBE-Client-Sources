#pragma once

struct DepenetrationComponent {
    // DepenetrationComponent inner types define
    enum class Bit {};

    int mCollisionDepth;
    char pad_0x0000[0x34]; //0x0000

    [[nodiscard]] bool isPenetrating() const
    {
        return mCollisionDepth != 10;
    }
};
static_assert(sizeof(DepenetrationComponent) == 0x38, "DepenetrationComponent size is incorrect");