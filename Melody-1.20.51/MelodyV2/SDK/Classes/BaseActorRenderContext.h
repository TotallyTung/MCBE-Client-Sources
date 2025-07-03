// Created by Tony on 2024-10-10 10:01:54

#include "ItemStackBase.h"
#include <string>
#include "Material.h"
#include "../../Utils/MemoryUtils.h"
#include "../../Utils/Math.h"
#include <optional>
class BaseActorRenderContext { /* Size=0x298 */
	/* 0x0008 */ float mNumEntitiesRenderedThisFrame;
	/* 0x000c */ float mLastFrameTime;
	/* 0x0010 */ const float mFrameAlpha;
	/* 0x0068 */ std::optional<__int64> mRenderUniqueIdOverride;
	/* 0x0078 */ Vec3<float> mCameraTargetPosition;
	/* 0x0084 */ Vec3<float> mCameraPosition;
	/* 0x0090 */ AABB mWorldClipRegion;
	/* 0x00ac */ bool mIsOnScreen;
	/* 0x00ad */ bool mUpdateBonesAndEffects;
	/* 0x00ae */ bool mUpdateEffects;
	/* 0x00af */ bool mIgnoresLightning;
};