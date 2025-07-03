//
// Created by vastrakai on 11/10/2024.
//

#pragma once

#include <bitset>
struct ActorDataFlagComponent {
    std::bitset<0x77> mFlags;
    //uint64_t getFlags(int) const;
    //bool getStatusFlag(::ActorFlags) const;
    //std::optional<std::bitset<119>> setFlags(uint64, int);
    //void setStatusFlag(::ActorFlags, bool);
    uint64_t getFlags(int index) const {
        uint64_t result = 0;
        for (int i = 0; i < 64; ++i) {
            if (mFlags.test(index + i)) {
                result |= (1ULL << i);
            }
        }
        return result;
    }
    bool getStatusFlag(::ActorFlags flag) const {
        return mFlags.test(static_cast<int>(flag));
    }
    std::optional<std::bitset<119>> setFlags(uint64_t newFlags, int index) {
        for (int i = 0; i < 64; ++i) {
            mFlags.set(index + i, newFlags & (1ULL << i));
        }
        return mFlags;
    }
    void setStatusFlag(::ActorFlags flag, bool value) {
        mFlags.set(static_cast<int>(flag), value);
    }
};