#pragma once

struct ActorLink {
    ActorLinkType mType;               // this+0x0
    int64_t       mA;                  // this+0x8
    int64_t       mB;                  // this+0x10
    bool          mImmediate;          // this+0x18
    bool          mPassengerInitiated; // this+0x19
};