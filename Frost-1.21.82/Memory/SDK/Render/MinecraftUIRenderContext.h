#pragma once

class ComponentRenderBatch;

#include "TextMeasureData.h"
#include "CaretMeasureData.h"
#include "UIScene.h"

#include "Context/BaseActorRenderContext.h"

enum ResourceFileSystem : int32_t {
    UserPackage = 0x0000,
    AppPackage = 0x0001,
    Raw = 0x0002,
    RawPersistent = 0x0003,
    SettingsDir = 0x0004,
    ExternalDir = 0x0005,
    ServerPackage = 0x0006,
    DataDir = 0x0007,
    UserDir = 0x0008,
    ScreenshotsDir = 0x0009,
    StoreCache = 0x000a,
    Invalid = 0x000b,
};

class ResourceLocation {
public:
    uint64_t    mFileSystem;        //0x0000
    std::string mFilePath;  //0x0008
    uint64_t    mPathHash{};
    uint64_t    mFullHash{};

    ResourceLocation() = default;

    ResourceLocation(bool external, std::string filePath) {
        this->mFilePath = filePath;
        if (external)
            this->mFileSystem = ResourceFileSystem::Raw;
        else this->mFileSystem = ResourceFileSystem::UserPackage;

        _computeHashes();
    };

    bool operator==(const ResourceLocation& other) const {
        if (this->mFileSystem != other.mFileSystem || this->mPathHash != other.mPathHash) return false;
        return this->mFilePath == other.mFilePath;
    }

    bool operator<(const ResourceLocation& other) const {
        return this->mFilePath < other.mFilePath;
    }
private:
    void _computeHashes() // Should add a Local variable for path hash computation
    {
        const uint64_t FNV_OFFSET_BASIS = 0xCBF29CE484222325u;
        const uint64_t FNV_PRIME = 0x100000001B3u;

        uint64_t _pathHash = FNV_OFFSET_BASIS;
        if (!this->mFilePath.empty()) {
            for (char c : this->mFilePath) {
                _pathHash *= FNV_PRIME;
                _pathHash ^= c;
            }
        }
        else {
            _pathHash = 0;
        }

        uint64_t fileSystemHash = FNV_OFFSET_BASIS ^ static_cast<uint8_t>(this->mFileSystem);
        fileSystemHash *= FNV_PRIME;

        this->mPathHash = _pathHash;
        this->mFullHash = _pathHash ^ fileSystemHash;
    }
};

enum IsMissingTexture : unsigned char {
    Yes = 0x0,
    No  = 0x1,
};

enum TextureLoadState : int {
    UnloadedBit     = 0x0,
    PendingBit      = 0x1,
    LoadedBit       = 0x2,
    PendingMetadata = 0x4,
    LoadedMetadata  = 0x8,
};

struct BedrockTextureData {
public:
    mce::ClientTexture             mClientTexture;         // this+0x0
    mce::TextureDescription        mTextureDescription;    // this+0x10
    IsMissingTexture               mIsMissingTexture;      // this+0x80
    TextureLoadState               mTextureLoadState;      // this+0x81
    cg::TextureSetImageDescription mTextureSetDescription; // this+0x88

    ~BedrockTextureData() {
        mClientTexture.mResourcePointerBlock.reset();
        mTextureSetDescription.mLayerInfo.clear();
    };
};

class BedrockTexture {
public:
    std::shared_ptr<BedrockTextureData> mBedrockTextureData;
};

namespace mce {
    class TexturePtr // TextureData's class
    {
    public:
        std::shared_ptr<BedrockTextureData> mClientTexture;
        std::shared_ptr<ResourceLocation>   mResourceLocation;

        TexturePtr() = default;

        [[nodiscard]] std::string getFilePath() const {
            return mResourceLocation->mFilePath;
        }
    };
}

class ImageData {
public:
    Vector2<float> mPos;     // this+0x0
    Vector2<float> mSize;    // this+0x8
    Vector2<float> mUv;      // this+0x10
    Vector2<float> mUvSize;  // this+0x18
};

class NinesliceInfo {
public:
    ImageData              mTopLeft;      // this+0x0
    ImageData              mTopRight;     // this+0x20
    ImageData              mBottomLeft;   // this+0x40
    ImageData              mBottomRight;  // this+0x60
    float                  mWidth;        // this+0x80
    float                  mHeight;       // this+0x84
    std::vector<ImageData> mLeft;
    std::vector<ImageData> mTop;
    std::vector<ImageData> mRight;
    std::vector<ImageData> mBottom;
    std::vector<ImageData> mMiddle;
};

class MinecraftUIRenderContext {
public:
    ClientInstance*  mClientInstance;  // this+0x8
    ScreenContext*   mScreenContext;   // this+0x10
    float            mGlobalAlpha;     // this+0x10
    UIScene*         mUIScene;         // this+0x18
    float            mTextAlpha;       // this+0x20

public:
    std::shared_ptr<mce::TextureGroup>& getTextureGroup() {
        return hat::member_at<std::shared_ptr<mce::TextureGroup>>(this, 0x38);
    }

    std::map<ResourceLocation, BedrockTexture>& getTextures() {
        return getTextureGroup()->getLoadedTextures();
    }

private:
    virtual void Destructor() {};
public:
    virtual float getLineLength(class Font* font, std::string* str, float measureCalculation, bool calculateWidth) {};
private:
    virtual int getTextAlpha() {};
    virtual void setTextAlpha() {};
    virtual void drawDebugText() {};
public:
    virtual void drawText(class Font* font, const float* pos, std::string* str, const float* colour, float, float alinM, const TextMeasureData* textMeasureData, const CaretMeasureData* caretMeasureData) {};
    virtual void flushText(float flushDelta) {};
public:
    virtual void drawImage(BedrockTextureData*, Vector2<float>* ImagePos, Vector2<float>* ImageDimension, Vector2<float> const& uvPos, Vector2<float> const& uvSize) {};
    virtual void drawNineslice(mce::TexturePtr* const& texturePtr, NinesliceInfo NinesliceInfo) {};
    virtual void flushImages(Color& color, float opacity, class HashedString& hashedString) {};
    virtual void beginSharedMeshBatch(uintptr_t ComponentRenderBatch) {};
    virtual void endSharedMeshBatch(float timeSinceLastFlush) {};
public:
    virtual void drawRectangle(Vector4<float> const& rect, Color const& colour, float alpha, int width) {};
    virtual void fillRectangle(Vector4<float> const& rect, Color const& colour, float alpha) {};
public:
    virtual auto increaseStencilRef() -> void {};
    virtual auto decreaseStencilRef() -> void {};
    virtual auto resetStencilRef() -> void {};
    virtual auto fillRectangleStencil(Vector4<float> const& position) -> void {};
    virtual auto enableScissorTest(Vector4<float> const& position) -> void {};
    virtual auto disableScissorTest() -> void {};
    virtual auto setClippingRectangle(Vector4<float> const& position) -> void {};
    virtual auto setFullClippingRectangle() -> void {};
    virtual auto saveCurrentClippingRectangle() -> void {};
    virtual auto restoreSavedClippingRectangle() -> void {};
    virtual auto getFullClippingRectangle() -> int {};
    virtual auto updateCustom(uintptr_t a1) -> void {};
    virtual auto renderCustom(uintptr_t a1, int a2, Vector4<float> const& position) -> void {};
    virtual auto cleanup() -> void {};
    virtual auto removePersistentMeshes() -> void {};
    virtual auto getTexture(class mce::TexturePtr* texture, void* resourceLocation) -> int {};
    virtual auto getZippedTexture(class TexturePtr* Path, class TexturePtr* ResourceLocation, bool a3) -> int {};
    virtual auto unloadTexture(class mce::TexturePtr* ResourceLocation) -> void {};
    virtual auto getUITextureInfo(class mce::TexturePtr* ResourceLocation, bool a2) -> int {};
    virtual auto touchTexture(class mce::TexturePtr* ResourceLocation) -> void {};
    virtual auto getMeasureStrategy(Vector2<float> const&) -> int {};
    virtual auto snapImageSizeToGrid(Vector2<float> const&) -> void {};
    virtual auto snapImagePositionToGrid(Vector2<float> const&) -> void {};
    virtual auto notifyImageEstimate(ULONG) -> void {};
};