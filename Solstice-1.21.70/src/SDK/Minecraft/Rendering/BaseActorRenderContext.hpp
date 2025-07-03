#pragma once
//
// Created by jcazm on 7/27/2024.
//

struct HistoricalFrameTimes
{
    uint64_t mFrameIndex; //0x0000
    std::array<float, 30> mHistoricalMinimumFrameTimes; //0x0008
    std::array<float, 30> mHistoricalMaximumFrameTimes; //0x0080
    std::array<float, 30> mHistoricalAverageFrameTimes; //0x00F8
    std::array<float, 30> mLastFrameTime; //0x0170
};
static_assert(sizeof(HistoricalFrameTimes) == 0x1E8);

class BaseActorRenderContext
{
public:
    float mNumEntitiesRenderedThisFrame;                                  //0x0008
    float mLastFrameTime;                                                 //0x000C
    float mFrameAlpha;                                                    //0x0010
    char pad_0014[4];                                                     //0x0014
    class ClientInstance *mClientInstance;                                //0x0018
    class MinecraftGame *mMinecraftGame;                                  //0x0020
    class ScreenContext *mScreenContext;                                  //0x0028
    class BlockActorRenderDispatcher *mBlockEntityRenderDispatcher;       //0x0030
    class std::shared_ptr<class ActorRenderDispatcher> mEntityRenderDispatcher; //0x0038
    class ActorBlockRenderer *mEntityBlockRenderer;                       //0x0048
    class ItemInHandRenderer *mItemInHandRenderer;                        //0x0050
    class ItemRenderer *mItemRenderer;                                    //0x0058
    class ParticleSystemEngine *mParticleSystemEngine;                    //0x0060
    class std::optional<int64_t> mRenderUniqueIdOverride;                 //0x0068
    glm::vec3 mCameraTargetPosition;                                        //0x0078
    glm::vec3 mCameraPosition;                                              //0x0084
    class AABB mWorldClipRegion;                                          //0x0090
    float mNumEntitiesRenderedThisFrame2;                                 //0x00A8
    bool mIsOnScreen;                                                     //0x00AC
    bool mUpdateBonesAndEffects;                                          //0x00AD
    bool mUpdateEffects;                                                  //0x00AE
    bool mIgnoresLightning;                                               //0x00AF
    class HistoricalFrameTimes mHistoricalFrameTimes;                     //0x00B0

    virtual ~BaseActorRenderContext();
}; //Size: 0x0298
static_assert(sizeof(BaseActorRenderContext) == 0x298);
