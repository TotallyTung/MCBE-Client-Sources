#pragma once

// Should be a public class of EntityContext weakPtr
struct WeakEntityRef {
	char pad_0x0[0x10];  // this+0x0, padding to the `id` field
	EntityId id;         // this+0x10, aligned to 0x10
	char pad_0x14[0x4];  // this+0x14, padding to the next field (if any)
};

class HitResult
{
public:
	Vector3<float>          mStartPos;     // this+0x0
	Vector3<float>          mRayDir;       // this+0xC
	HitResultType           mType;         // this+0x18
	int                     mFacing;       // this+0x1C
	Vector3<int>            mBlockPos;     // this+0x20
	Vector3<float>          mPos;          // this+0x2C
	WeakEntityRef           mEntity;       // this+0x38
	bool                    mIsHitLiquid;  // this+0x50
	FacingID                mLiquidFacing; // this+0x51
	Vector3<int>            mLiquid;       // this+0x54
	Vector3<float>          mLiquidPos;    // this+0x60
	bool                    mIndirectHit;  // this+0x6C

	CLASS_FIELD(Vector3<float>, Origin, 0x0);
	CLASS_FIELD(Vector3<float>, RelativeRayEnd, 0xC);
	CLASS_FIELD(int, BlockFace, 0x1C);
	CLASS_FIELD(Vector3<int>, IBlockPos, 0x20);
	CLASS_FIELD(Vector3<float>, AbsoluteHitPos, 0x2C);
}; 
// Size: 0x0070