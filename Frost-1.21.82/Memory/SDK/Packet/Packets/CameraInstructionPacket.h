#pragma region

struct CameraInstruction {
public:
    struct FadeInstruction;
    struct SetInstruction;

    struct FadeInstruction {
    public:
        struct ColorOption;
        struct TimeOption;

        struct ColorOption {
        public:
            float   r; // this+0x0
            float   g; // this+0x4
            float   b; // this+0x8
        };

        struct TimeOption {
        public:
            float   mFadeInTime;  // this+0x0
            float   mWaitTime;    // this+0x4
            float   mFadeOutTime; // this+0x8
        };

    public:
        std::optional<TimeOption>    mTimeData; // this+0x0
        std::optional<ColorOption>   mColor;    // this+0x18 (std::optional uses 24 bytes internally on most implementations)
    };

    struct SetInstruction {
    public:
        struct EaseOption;
        struct FacingOption;
        struct PosOption;
        struct RotOption;

        struct EaseOption {
        public:
            unsigned char   mType;  // this+0x0
            float           mTime;  // this+0x4
        };

        struct FacingOption {
        public:
            Vector3<float> mFacing; // this+0x0
        };

        struct PosOption {
        public:
            Vector3<float> mPos; // this+0x0
        };

        struct RotOption {
        public:
            Vector2<float> mRot; // this+0x0
        };

    public:
        std::optional<unsigned int> mPreset; // this+0x0
        std::optional<EaseOption>   mEase;   // this+0x18
        std::optional<PosOption>    mPos;    // this+0x30
        std::optional<RotOption>    mRot;    // this+0x48
        std::optional<FacingOption> mFacing; // this+0x60
        std::optional<bool>         mDefault; // this+0x78 (Default?)
    };

public:
    std::optional<SetInstruction>  mSetInstruction;  // this+0x0
    std::optional<FadeInstruction> mFadeInstruction; // this+0x18
};

class CameraInstructionPacket : public ::Packet {
public:
    CameraInstruction mData; // this+0x30

    int getPreset() const {
        return mData.mSetInstruction->mPreset.value();
    }
};
