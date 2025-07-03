struct MoveInputComponent {
public:
    // Convert this into manually-padded struct
    CLASS_MEMBER(bool, moveLocked, 0x82);
    CLASS_MEMBER(bool, isSneakDown, 0x20);
    CLASS_MEMBER(bool, isJumping, 0x26);
    CLASS_MEMBER(bool, isJumping2, 0x80);
    CLASS_MEMBER(bool, isSprinting, 0x27);
    CLASS_MEMBER(bool, forward, 0x2A);
    CLASS_MEMBER(bool, backward, 0x2B);
    CLASS_MEMBER(bool, left, 0x2C);
    CLASS_MEMBER(bool, right, 0x2D);
    CLASS_MEMBER(Vec2<float>, moveVector, 0x48);

    // padding to make the struct size 136
    char pad_0x0[0x88];

    void setJumping(bool value) {
        reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + 0x26)[0] = value;
        reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + 0x80)[0] = value;
    }
};
