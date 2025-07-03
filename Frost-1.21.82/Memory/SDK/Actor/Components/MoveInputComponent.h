#pragma once

struct MoveInputComponent {
public:
    // Previous
    CLASS_FIELD(bool, mSneakingPrev, 0x0);
    CLASS_FIELD(bool, mJumpingPrev, 0x06);
    CLASS_FIELD(bool, mSprintingPrev, 0x07);
    CLASS_FIELD(bool, mForwardPrev, 0x0A);
    CLASS_FIELD(bool, mBackwardPrev, 0x0B);
    CLASS_FIELD(bool, mLeftPrev, 0x0C);
    CLASS_FIELD(bool, mRightPrev, 0x0D);

    // Current
    CLASS_FIELD(bool, mSneaking, 0x28);
    CLASS_FIELD(bool, mJumping, 0x2F);
    CLASS_FIELD(bool, mSprinting, 0x30);
    CLASS_FIELD(bool, mForward, 0xD);
    CLASS_FIELD(bool, mBackward, 0xE);
    CLASS_FIELD(bool, mLeft, 0xF);
    CLASS_FIELD(bool, mRight, 0x10);

    // sizeMovement/forwardMovement
    CLASS_FIELD(Vector2<float>, mMoveVector, 0x48); 

    // idk
    CLASS_FIELD(bool, mIsMoveLocked, 0x8A);
    CLASS_FIELD(bool, mPreJumpDownState, 0x80);

    // padding to make the struct size 136
    char pad_0x0[0x88];

public:
    void setJumping(bool value) {
        mJumping = value;
        mPreJumpDownState = value;
    }

    bool isPressed() const {
        return mForward || mBackward || mLeft || mRight;
    };
    
    bool isStrafing() const {
        return mBackward || mLeft || mRight;
    };
};
static_assert(sizeof(MoveInputComponent) == 136, "MoveInputComponent size is not 136 bytes!");