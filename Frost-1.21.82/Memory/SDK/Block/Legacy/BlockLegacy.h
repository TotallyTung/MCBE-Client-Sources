#pragma once

// BLOCK ID: 44 0F ? ? ? ? ? ? B8 ? ? ? ? 48 8B ? 48 8B

struct Brightness {};

class BlockLegacy
{
public:
    struct NameInfo {
        HashedString mRawName;           // this+0x0
        std::string  mNamespaceName;      // this+0x30
        HashedString mFullName;          // this+0x50
        HashedString mPreFlatteningName; // this+0x80
    };
public:
	CLASS_FIELD(uintptr_t**, Vtable, 0x0);

    std::string           mDescriptionId;                              // this + 0x0
    BlockComponentStorage mComponents;                                 // this + 0x20
    BlockLegacy::NameInfo mNameInfo;                                   // this + 0x88
    BlockProperty*        mProperties;                                 // this + 0x138
    bool                  mFancy;                                      // this + 0x140
    BlockRenderLayer      mRenderLayer;                                // this + 0x141
    bool                  mRenderLayerCanRenderAsOpaque;               // this + 0x142
    BlockActorType        mBlockEntityType;                            // this + 0x144
    bool                  mAnimatedTexture;                            // this + 0x148
    float                 mThickness;                                  // this + 0x14C
    bool                  mCanSlide;                                   // this + 0x150
    bool                  mCanReactToNeighborsDuringInstatick;         // this + 0x151
    bool                  mIsInteraction;                              // this + 0x152
    float                 mGravity;                                    // this + 0x154
    Material/* const&*/   mMaterial;                                   // this + 0x158
    bool                  mFalling;                                    // this + 0x160
    float                 mParticleQuantityScalar;                     // this + 0x164
    CreativeItemCategory  mCreativeCategory;                           // this + 0x168
    std::string           mCreativeGroup;                              // this + 0x170
    bool                  mIsHiddenInCommands;                         // this + 0x190
    bool                  mAllowsRunes;                                // this + 0x191
    bool                  mCanBeBrokenFromFalling;                     // this + 0x192
    bool                  mCanBeOriginalSurface;                       // this + 0x193
    bool                  mSolid;                                      // this + 0x194
    bool                  mPushesOutItems;                             // this + 0x195
    bool                  mIgnoreBlockForInsideCubeRenderer;           // this + 0x196
    bool                  mIsTrapdoor;                                 // this + 0x197
    bool                  mIsDoor;                                     // this + 0x198
    bool                  mIsOpaqueFullBlock;                          // this + 0x199
    float                 mTranslucency;                               // this + 0x19C
    bool                  mShouldRandomTick;                           // this + 0x1A0
    bool                  mShouldRandomTickExtraLayer;                 // this + 0x1A1
    bool                  mIsMobPiece;                                 // this + 0x1A2
    bool                  mCanBeExtraBlock;                            // this + 0x1A3
    bool                  mCanPropagateBrightness;                     // this + 0x1A4
    Brightness            mLightBlock;                                 // this + 0x1A5
    Brightness            mLightEmission;                              // this + 0x1A6
    FlameOdds             mFlameOdds;                                  // this + 0x1A8
    BurnOdds              mBurnOdds;                                   // this + 0x1AC
    LavaFlammable         mLavaFlammable;                              // this + 0x1B0
    mce::Color*           mMapColor;                                   // this + 0x1B4
    float                 mFriction;                                   // this + 0x1C4
    NoteBlockInstrument   mNoteBlockInstrument;                        // this + 0x1C8
    TintMethod            mTintMethod;                                 // this + 0x1CC
    bool                  mReturnDefaultBlockOnUnidentifiedBlockState; // this + 0x1CD
    int                   mID;                                         // this + 0x1D0

	int getBlockID() {
        return hat::member_at<int>(this, OffsetManager::BlockLegacy_mBlockId);
	}

	bool isSolid() {
		return mSolid;
	}

	Material* getMaterial() {
		return &mMaterial;
	}
};