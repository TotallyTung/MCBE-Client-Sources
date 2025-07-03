//
// Created by vastrakai on 6/30/2024.
//

#pragma once




struct MoveInputComponent {
public:
    CLASS_FIELD(bool, mIsMoveLocked, 0x8A);
    CLASS_FIELD(bool, mIsSneakDown, 0x28);
    CLASS_FIELD(bool, mIsJumping, 0x2F);
    CLASS_FIELD(bool, mIsJumping2, 0x80);
    CLASS_FIELD(bool, mIsSprinting, 0x30);
    CLASS_FIELD(bool, mForward, 0xD);
    CLASS_FIELD(bool, mBackward, 0xE);
    CLASS_FIELD(bool, mLeft, 0xF);
    CLASS_FIELD(bool, mRight, 0x10);
    CLASS_FIELD(glm::vec2, mMoveVector, 0x48);

    // padding to make the struct size 136
    PAD(0x88);

    void setJumping(bool value) {
        reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + 0x26)[0] = value;
        reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + 0x80)[0] = value;
    }

    void reset(bool lockMove = false, bool resetMove = true) {
        mIsMoveLocked = lockMove;
        mIsSneakDown = false;
        mIsJumping = false;
        mIsJumping2 = false;
        mIsSprinting = false;
        if (resetMove)
        {
            mForward = false;
            mBackward = false;
            mLeft = false;
            mRight = false;
        }
        mMoveVector = glm::vec2(0, 0);
    }
};

struct RawMoveInputComponent {
public:
    CLASS_FIELD(bool, mIsMoveLocked, 0x82);
    CLASS_FIELD(bool, mIsSneakDown, 0x20);
    CLASS_FIELD(bool, mIsJumping, 0x26);
    CLASS_FIELD(bool, mIsJumping2, 0x80);
    CLASS_FIELD(bool, mIsSprinting, 0x27);
    CLASS_FIELD(bool, mForward, 0x2C);
    CLASS_FIELD(bool, mBackward, 0x2D);
    CLASS_FIELD(bool, mLeft, 0x2E);
    CLASS_FIELD(bool, mRight, 0x2F);
    CLASS_FIELD(glm::vec2, mMoveVector, 0x48);

    // padding to make the struct size 136
    char pad_0x0[0x88];

    void setJumping(bool value) {
        reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + 0x26)[0] = value;
        reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + 0x80)[0] = value;
    }
};

static_assert(sizeof(MoveInputComponent) == 136, "MoveInputComponent size is not 136 bytes!");
static_assert(sizeof(RawMoveInputComponent) == 136, "RawMoveInputComponent size is not 136 bytes!");