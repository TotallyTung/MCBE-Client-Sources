#pragma once

struct HistoricalFrameTimes {
    uint64_t mFrameIndex; // this+0x0000
    std::array<float, 30> mHistoricalMinimumFrameTimes; // this+0x0008
    std::array<float, 30> mHistoricalMaximumFrameTimes; // this+0x0080
    std::array<float, 30> mHistoricalAverageFrameTimes; // this+0x00F8
    std::array<float, 30> mLastFrameTime;               // this+0x0170
};
//Size: 0x01E8
static_assert(sizeof(HistoricalFrameTimes) == 0x1E8);

class SortedMeshDrawList {};

class BaseActorRenderContext {
public:
    float                                              mNumEntitiesRenderedThisFrame;     // this+0x0008
    float                                              mLastFrameTime;                    // this+0x000C
    float                                              mFrameAlpha;                       // this+0x0010
    SortedMeshDrawList                                 mSortedMeshDrawList;               // this+0x0014
    class ClientInstance*                              mClientInstance;                   // this+0x0018
    class MinecraftGame*                               mMinecraftGame;                    // this+0x0020
    class ScreenContext*                               mScreenContext;                    // this+0x0028
    class BlockActorRenderDispatcher*                  mBlockEntityRenderDispatcher;      // this+0x0030
    class std::shared_ptr<class ActorRenderDispatcher> mEntityRenderDispatcher;           // this+0x0038
    class ActorBlockRenderer*                          mEntityBlockRenderer;              // this+0x0048
    class ItemInHandRenderer*                          mItemInHandRenderer;               // this+0x0050
    class ItemRenderer*                                mItemRenderer;                     // this+0x0058
    class ParticleSystemEngine*                        mParticleSystemEngine;             // this+0x0060
    class std::optional<int64_t>                       mRenderUniqueIdOverride;           // this+0x0068
    glm::vec3                                          mCameraTargetPosition;             // this+0x0078
    glm::vec3                                          mCameraPosition;                   // this+0x0084
    class AABB                                         mWorldClipRegion;                  // this+0x0090
    float                                              mNumEntitiesRenderedThisFrame2;    // this+0x00A8
    bool                                               mIsOnScreen;                       // this+0x00AC
    bool                                               mUpdateBonesAndEffects;            // this+0x00AD
    bool                                               mUpdateEffects;                    // this+0x00AE
    bool                                               mIgnoresLightning;                 // this+0x00AF
    class HistoricalFrameTimes                         mHistoricalFrameTimes;             // this+0x00B0

    virtual ~BaseActorRenderContext();
};
//Size: 0x0298
static_assert(sizeof(BaseActorRenderContext) == 0x298);