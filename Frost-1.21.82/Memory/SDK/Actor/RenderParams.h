#pragma once

// I got those variables from client 

class ParticleSystem {
public:
	class ParticleEmitter;
};

class IBlockWorldGenAPI {
public:
	IBlockWorldGenAPI& operator=(IBlockWorldGenAPI const&);
	IBlockWorldGenAPI(IBlockWorldGenAPI const&);
	IBlockWorldGenAPI();

public:
	virtual ~IBlockWorldGenAPI() = default;
};

class Netease {
public:
	class BlockCache;
};

struct MolangScriptArg {
	char filler[0x58];
};

struct RenderParams {
	class BaseActorRenderContext*          mBaseActorRenderContext; // this+0x0
	class MolangVariableMap*               mVariables;              // this+0x8
	class AnimationComponent*              mAnimationComponent;     // this+0x10
	class AnimationComponent*              mRootAnimationComponent; // this+0x18
	std::shared_ptr<class DataDrivenModel> mModel;                  // this+0x20
	char                                   mSourceContext[0x8];     // this+0x30
	Actor*                                 mActor;                  // this+0x38
	Actor*                                 mOther;                  // this+0x40
	Actor*                                 mPlayer;                 // this+0x48
	Actor*                                 mTarget;                 // this+0x50
	Actor*                                 mParent;                 // this+0x58
	Actor*                                 mBaby;                   // this+0x60
	Actor*                                 mDamager;                // this+0x68
	Level*                                 mLevel;                  // this+0x70
	ItemStackBase*                         mItemStackBasePtr;       // this+0x78
	Block*                                 mBlock;                  // this+0x80
	Vector3<int>                           mBlockPos;               // this+0x88
	BlockSource*                           mBlockSource;            // this+0x98
	ActorRenderData*                       mActorRenderData;        // this+0xA0
	ParticleSystem::ParticleEmitter*       mParticleEmitter;        // this+0xA8
	IBlockWorldGenAPI*                     mBlockPlacementTarget;   // this+0xB0
	int                                    mVertexCount;            // this+0xB8
	uint64_t                               mSubRenderLayerIndex;    // this+0xBC
	std::function<float __cdecl(void)>     mRandomFunction;         // this+0xC0
	float                                  mCameraDistance;         // this+0x100
	float                                  mParams[8];              // this+0x104
	MolangScriptArg                        mThisValue;              // this+0x128
	MolangScriptArg                        mScratchValue;           // this+0x180
	char                                   mFlags[0x4];             // this+0x1D8
	Vector3<float>                         mClickPos;               // this+0x1DC
	Netease::BlockCache*                   mBlockCache;             // this+0x1E8
	char                                   mModParams;              // this+0x1F0
};