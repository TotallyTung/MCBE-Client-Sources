#pragma once

namespace mce {
    enum TextureFormat : uint32_t {
        UNKNOWN_TEXTURE_FORMAT = 0,
        R32G32B32A32_FLOAT     = 0x2,
        R16G16B16A16_FLOAT     = 0xA,
        R16G16B16A16_UNORM     = 0xB,
        R32G32_FLOAT           = 0x10,
        R10G10B10A2_UNORM      = 0x18,
        R11G11B10_FLOAT        = 0x1A,
        R8G8B8A8_UNORM         = 0x1C,
        R8G8B8A8_UNORM_SRGB    = 0x1D,
        R16G16_FLOAT           = 0x22,
        R16G16_UNORM           = 0x23,
        R16G16_UINT            = 0x24,
        R16G16_SNORM           = 0x25,
        D32_FLOAT              = 0x28,
        R32_FLOAT              = 0x29,
        R32_UINT               = 0x2A,
        R24G8_TYPELESS         = 0x2C,
        D24_UNORM_S8_UINT      = 0x2D,
        R24_UNORM_X8_TYPELESS  = 0x2E,
        R8G8_UNORM             = 0x31,
        R8G8_SNORM             = 0x33,
        R16_FLOAT              = 0x36,
        D16_UNORM              = 0x37,
        R8_UNORM               = 0x3D,
        R8_UINT                = 0x3E,
        A8_UNORM               = 0x41,
        BC3_UNORM              = 0x4D,
        R5G6B5_UNORM           = 0x55,
        R5G5B5A1_UNORM         = 0x56,
        B8G8R8A8_UNORM         = 0x57,
        B8G8R8A8_UNORM_SRGB    = 0x5B,
        BC7_UNORM              = 0x62,
        R4G4B4A4_UNORM         = 0x73,
        S8_UINT                = 0x74,
        R8G8B8_UNORM           = 0x75,
        COMPRESSED             = 0x76
    };
}

namespace cg {
    struct ImageDescription {
        uint32_t           mWidth;
        uint32_t           mHeight;
        mce::TextureFormat mTextureFormat;
        uint32_t           mArraySize;
    };
}

namespace cg {
    class TextureDescription : public cg::ImageDescription {
    public:
        uint32_t mMipCount;
    };
}

namespace cg {
    enum TextureSetLayerType : int32_t {
        Color      = 0x0,
        ColorUnlit = 0x1,
        MER        = 0x2,
        Metalness  = 0x3,
        Emissive   = 0x4,
        Roughness  = 0x5,
        Normal     = 0x6,
        Heightmap  = 0x7,
    };
}

class ColorChannel {
    float mValue;
};

namespace cg {
    class TextureSetImageDescription {
    public:
        class LayerInfoVar {
            cg::TextureSetLayerType mLayerType;
            std::variant <cg::ImageDescription, ColorChannel, mce::mColor> mData;
        };

        std::vector <cg::TextureSetImageDescription::LayerInfoVar, std::allocator<cg::TextureSetImageDescription::LayerInfoVar>> mLayerInfo;
    };
}

namespace mce {
    enum BindFlagsBit : uint32_t {
        NoBindFlags        = 0x0,
        VertexBufferBit    = 0x1,
        IndexBufferBit     = 0x2,
        ConstantBufferBit  = 0x4,
        ShaderResourceBit  = 0x8,
        StreamOutputBit    = 0x10,
        RenderTargetBit    = 0x20,
        DepthStencilBit    = 0x40,
        UnorderedAccessBit = 0x80,
    };
}

namespace mce {
    class SampleDescription {
        int32_t mCount;
        int32_t mQuality;
    };
}

namespace mce {
    class TextureDescription : public cg::TextureDescription {
    public:
        mce::SampleDescription mSampleDescription;
        mce::mColor mClearColor;
        float mOptimizedClearDepth;
        unsigned char mOptimizedClearStencil;
        mce::BindFlagsBit mBindFlags;
        bool mIsStaging;
    };
}

namespace mce {
    class TextureBase { /* Size=0x78 */
    public:
        mce::TextureDescription textureDescription; // this+0x0
        bool                    created;            // this+0x70
        bool                    mOwnsResource;      // this+0x71
    };
}

namespace mce { 
    class TextureNull : public mce::TextureBase {};
}

namespace mce {
    class ResourceBase {};
}

namespace mce {
    class DragonLifetime {};
}

namespace mce 
{
    class Texture : public mce::TextureNull, protected mce::ResourceBase {
    public:
        std::shared_ptr<mce::DragonLifetime> mDragonTextureLifetime;
    };
}

namespace mce {
    struct ResourceServiceTextureDescription : public mce::TextureDescription { /* Size=0x90 */
        std::basic_string<char, std::char_traits<char>, std::allocator<char> > mDebugName;
    };
}

namespace mce {
    class PerFrameHandleTracker { /* Size=0x4 */
        std::atomic<unsigned short> mCheckCount;
        std::atomic<bool>           mIsValid;
    };
}

class TextureGroupBase {
public:
    std::map<class ResourceLocation, class BedrockTexture>& getLoadedTextures() {
        return hat::member_at<std::map<class ResourceLocation, class BedrockTexture>>(this, 0x18 + 0x178);
    }
};

namespace mce {
    class TextureGroup : public TextureGroupBase {};
}