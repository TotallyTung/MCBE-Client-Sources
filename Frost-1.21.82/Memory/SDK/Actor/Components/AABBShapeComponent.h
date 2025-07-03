#pragma once

struct AABBShapeComponent {
	Vector3<float> mPosLower = { 0, 0, 0 }; // this+0x0
	Vector3<float> mPosUpper = { 0, 0, 0 }; // this+0xC
	Vector2<float> mHitbox   = { 0, 0 };    // this+0x18 

	AABB getAABB() const {
		return AABB(mPosLower, mPosUpper, true);
	}
};
// Size: 0x20