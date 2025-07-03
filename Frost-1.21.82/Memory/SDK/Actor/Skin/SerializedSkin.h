#pragma once

/*namespace mce
{
    enum class ImageUsage : unsigned char {
        Unknown = 0x0,
        sRGB = 0x1,
        Data = 0x2,
    };

    enum class ImageFormat : int {
        Unknown = 0x0,
        R8Unorm = 0x1,
        RGB8Unorm = 0x2,
        RGBA8Unorm = 0x3,
    };

    class Blob
    {
    public:
        using value_type = unsigned char;
        using size_type = size_t;
        using pointer = value_type*;
        using iterator = value_type*;
        using const_pointer = value_type const*;
        using const_iterator = value_type const*;

        using delete_function = void (*)(pointer);

        struct Deleter {
        public:
            delete_function mFn;

            void operator()(pointer x) const { mFn(x); }
        };

        using pointer_type = std::unique_ptr<value_type[], Deleter>;

        pointer_type mBlob{}; // this+0x0
        size_type    mSize{}; // this+0x10

        [[nodiscard]] _CONSTEXPR23 Blob() = default;

        [[nodiscard]] _CONSTEXPR23 Blob(std::span<unsigned char> s, Deleter deleter = {}) : mSize(s.size()) { // NOLINT
            mBlob = pointer_type(new value_type[mSize], deleter);
            std::copy(s.begin(), s.end(), mBlob.get());
        }

        [[nodiscard]] _CONSTEXPR23 pointer data() const { return mBlob.get(); }

        [[nodiscard]] _CONSTEXPR23 size_type size() const { return mSize; }

        [[nodiscard]] _CONSTEXPR23 std::span<unsigned char> view() const { return { data(), size() }; }

        [[nodiscard]] _CONSTEXPR23 Blob(Blob&&) noexcept = default;

        [[nodiscard]] _CONSTEXPR23 Blob(Blob const& other) : Blob(other.view(), other.mBlob.get_deleter()) {}

        _CONSTEXPR23 Blob& operator=(Blob const& other) {
            if (this != &other) {
                *this = Blob{ other };
            }
            return *this;
        }
    };

    struct Image
    {
    public:
        ImageFormat imageFormat{}; // this+0x0
        unsigned int        mWidth{};      // this+0x4
        unsigned int        mHeight{};     // this+0x8
        unsigned int        mDepth{};      // this+0xC
        ImageUsage  mUsage{};      // this+0x10
        Blob        mImageBytes;   // this+0x18
    };
}*/

class SemVersion {
public:
    struct any_version_constructor;

    // SemVersion inner types define
    enum class MatchType : int {
        Full = 0x0,
        Partial = 0x1,
        None = 0x2,
    };

    enum class ParseOption : int {
        AllowWildcards = 0x0,
        NoWildcards = 0x1,
    };

    unsigned short      mMajor;
    unsigned short      mMinor;
    unsigned short      mPatch;
    std::string         mPreRelease;
    std::string         mBuildMeta;
    std::string         mFullVersionString;
    bool                mValidVersion;
    bool                mAnyVersion;
};

namespace persona {
    enum class AnimatedTextureType : int {
        None = 0x0,
        Face = 0x1,
        Body32x32 = 0x2,
        Body128x128 = 0x3,
    };

    enum class AnimationExpression : int {
        Linear = 0x0,
        Blinking = 0x1,
    };


    enum class PieceType : int {
        Unknown = 0x0,
        Skeleton = 0x1,
        Body = 0x2,
        Skin = 0x3,
        Bottom = 0x4,
        Feet = 0x5,
        Dress = 0x6,
        Top = 0x7,
        High_Pants = 0x8,
        Hands = 0x9,
        Outerwear = 0xA,
        FacialHair = 0xB,
        Mouth = 0xC,
        Eyes = 0xD,
        Hair = 0xE,
        Hood = 0xF,
        Back = 0x10,
        FaceAccessory = 0x11,
        Head = 0x12,
        Legs = 0x13,
        LeftLeg = 0x14,
        RightLeg = 0x15,
        Arms = 0x16,
        LeftArm = 0x17,
        RightArm = 0x18,
        Capes = 0x19,
        ClassicSkin = 0x1A,
        Emote = 0x1B,
        Unsupported = 0x1C,
        Count = 0x1D,
    };


    class ArmSize
    {
    public:
        // ArmSize inner types define
        enum class Type : int64_t {
            Slim = 0,
            Wide = 1,
            Count = 2,
            Unknown = 3,
        };
    };
};

class AnimatedImageData
{
public:
    persona::AnimatedTextureType mType;
    persona::AnimationExpression mAnimationExpression;
    mce::Image                   mImage;
    float                        mFrames;
};

class TintMapColor
{
public:
    std::array<Color, 4> colors;
};

class SerializedPersonaPieceHandle
{
public:
    std::string        mPieceId;
    persona::PieceType mPieceType;
    int                mPackId;
    bool               mIsDefaultPiece;
    std::string        mProductId;
};

class SerializedSkin {
public:
    CLASS_FIELD(int32_t, skinWidth, 0xA4);
    CLASS_FIELD(int32_t, skinHeight, 0xA8);
    CLASS_FIELD(const uint8_t*, skinData, 0xC0); // skinImage + Blob = 0xB8 + 0x8 pointer = 0xC0
public:
    enum class TrustedSkinFlag : signed char {
        Unset = 0x0,
        False = 0x1,
        True = 0x2,
    };

    std::string                                          mId;                             // this+0x0
    std::string                                          mPlayFabId;                      // this+0x20
    std::string                                          mFullId;                         // this+0x40
    std::string                                          mResourcePatch;                  // this+0x60
    std::string                                          mDefaultGeometryName;            // this+0x80
    mce::Image                                           mSkinImage;                      // this+0xA0
    mce::Image                                           mCapeImage;                      // this+0xC0
    std::vector<AnimatedImageData>                       mSkinAnimatedImages;             // this+0xE0
    Json::Value                                          mGeometryData;                   // this+0x100
    SemVersion                                           mGeometryDataEngineVersion;      // this+0x120
    Json::Value                                          mGeometryDataMutable;            // this+0x128
    std::string                                          mAnimationData;                  // this+0x148
    std::string                                          mCapeId;                         // this+0x168
    std::vector<SerializedPersonaPieceHandle>            mPersonaPieces;                  // this+0x188
    persona::ArmSize::Type                               mArmSizeType;                    // this+0x1A8
    std::unordered_map<persona::PieceType, TintMapColor> mPieceTintColors;                // this+0x1B0
    Color                                                mSkinColor;                      // this+0x1D0
    TrustedSkinFlag                                      mIsTrustedSkin;                  // this+0x1D8
    bool                                                 mIsPremium;                      // this+0x1DC
    bool                                                 mIsPersona;                      // this+0x1DD
    bool                                                 mIsPersonaCapeOnClassicSkin;     // this+0x1DE
    bool                                                 mIsPrimaryUser;                  // this+0x1DF
    bool                                                 mOverridesPlayerAppearance;      // this+0x1E0
public:
    int32_t getSkinHeight() {
        return mSkinImage.mHeight;
    }
    
    int32_t getSkinWidth() {
        return mSkinImage.mWidth;
    }

    const uint8_t* getSkinData() {
        return skinData;
        //return *mSkinImage.mImageBytes.mData.get();
    }
/*public:
private:
	char padding_0[0xa4];
public:
	int width; //0xa4
public:
	int height; //0xa8
private:
	char padding_1[0xc];
public:
	unsigned char* image; //0xb8*/
	/*uint8_t pad[0x268]{};
public:
	CLASS_FIELD(std::string, name, 0x0);
	CLASS_FIELD(std::string, geometryType, 0x80);
	CLASS_FIELD(int32_t, skinWidth, 0xA4);
	CLASS_FIELD(int32_t, skinHeight, 0xA8);
	CLASS_FIELD(const uint8_t*, skinData, 0xB8);
	CLASS_FIELD(int32_t, capeWidth, 0xCC);
	CLASS_FIELD(int32_t, capeHeight, 0xD0);
	CLASS_FIELD(const uint8_t*, capeData, 0xE0);
	CLASS_FIELD(MinecraftJson::Value, skinGeometry, 0x108);
	CLASS_FIELD(TrustedSkinFlag, trustedSkinFlag, 0x260);
	CLASS_FIELD(bool,isPremiumSkin, 0x261);
	CLASS_FIELD(bool,isPersonaSkin, 0x262);*/
};