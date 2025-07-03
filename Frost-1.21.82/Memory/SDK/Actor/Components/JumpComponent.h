#pragma once

struct DeserializeDataParams {
    Json::Value& mValue;           // this+0x0
    const SemVersion mPackVersion; //this+0x8
};

struct Description { /* Size=0x8 */
    // Virtuals that I do not want to add
};

struct ComponentDescription : public Description { /* Size=0x8 */
    /* 0x0000: fields for Description */

    // Also virtuals that I do NOT want to add
};

struct JumpControlDescription : public ComponentDescription { /* Size=0x10 */
public:
    float mJumpPower; // this+0x8

    // Also virtuals that I do NOT want to add
};

class JumpControl : public Control {
public:
    // Also virtuals that I do NOT want to add
};

class JumpInfo { // Size: 0x10
public:
    int32_t mAnimDuration;  // this+0x0
    int32_t mJumpDelay;     // this+0x4
    float   mDistanceScale; // this+0x8
    float   mHeight;        // this+0xC
};
static_assert(sizeof(JumpInfo) == 0x10);

class JumpControlComponent {
public:
    bool                                                           mJumping;     // this+0x0
    bool                                                           mSwimming;    // this+0x1
    float                                                          mJumpPower;   // this+0x4
    JumpType                                                       mJumpType;    // this+0x8
    JumpInfo                                                       mJumpInfo[4]; // this+0xC
    std::unique_ptr<JumpControl, std::default_delete<JumpControl>> mJumpControl; // this+0x50
};
static_assert(sizeof(JumpControlComponent) == 0x58);