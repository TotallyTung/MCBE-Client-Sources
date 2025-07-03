//
// Created by vastrakai on 7/22/2024.
//

#include "Packet.hpp"

#include <optional>


struct CameraInstruction {
public:
    // CameraInstruction inner types declare
    // clang-format off
    struct FadeInstruction;
    struct SetInstruction;
    // clang-format on

    // CameraInstruction inner types define
    struct FadeInstruction {
    public:
        // FadeInstruction inner types declare
        // clang-format off
        struct ColorOption;
        struct TimeOption;
        // clang-format on

        // FadeInstruction inner types define
        struct ColorOption {
        public:
            float r;
            float g;
            float b;
        };

        struct TimeOption {
        public:
            float mFadeInTime;
            float mWaitTime;
            float mFadeOutTime;
        };

    public:
        std::optional<TimeOption>  mTimeData;
        std::optional<ColorOption> mColor;
    };

    struct SetInstruction {
    public:
        // SetInstruction inner types declare
        // clang-format off
        struct EaseOption;
        struct FacingOption;
        struct PosOption;
        struct RotOption;
        // clang-format on

        // SetInstruction inner types define
        struct EaseOption {
        public:
            unsigned char mType;
            float mTime;
        };

        struct FacingOption {
        public:
            glm::vec3 mFacing;
        };

        struct PosOption {
        public:
            glm::vec3 mPos;
        };

        struct RotOption {
        public:
            glm::vec2 mRot;
        };

    public:
        std::optional<unsigned int> mPreset;
        std::optional<EaseOption>   mEase;
        std::optional<PosOption>    mPos;
        std::optional<RotOption>    mRot;
        std::optional<FacingOption> mFacing;
        std::optional<bool>         mDefaul; // Default?
    };

public:
    std::optional<SetInstruction>  mSetInstruction;
    std::optional<FadeInstruction> mFadeInstruction;
};

class CameraInstructionPacket : public ::Packet {
public:
    static inline PacketID ID = PacketID::CameraInstruction;

    CameraInstruction mData;

    int getPreset() const {
        return mData.mSetInstruction->mPreset.value();
    }
};