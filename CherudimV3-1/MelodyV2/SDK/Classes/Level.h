#pragma once
#include "../../Utils/Math.h"
#include "../../Utils/MemoryUtils.h"

class Actor;
enum class HitResultType : int32_t
{
	BLOCK,
	ENTITY,
	ENTITY_OUT_OF_RANGE,
	AIR
};
class WeakEntityRef
{
	
};
class SRaycastResult
{
public:
	Vec3<float> liquidHitLocation; //0x0000
	Vec3<float> raycastDirection; //0x0000
	HitResultType type; //0x0018
	int32_t selectedFace; //0x001C
	Vec3<int> blockPos;
	Vec3<float> endPos; //0x002C
	WeakEntityRef entity; //0x0038
	bool isLiquid; //0x004C
	char pad_004D[3]; //0x004D
	int32_t liquidFace; //0x0050
	Vec3<int> liquidBlockPos; //0x0054
	Vec3<float> liquidPos; //0x0060
	bool indirectHit; //0x006C
	char pad_006D[3]; //0x006D
}; //Size: 0x0070
class Level {
public:
	BUILD_ACCESS(this, SRaycastResult*, mHitResult, 0xA48);
	Vec3<float> getPointingBlockPos() {
		Vec3<float> pos;
		pos.x = *reinterpret_cast<float*>((uintptr_t)(this) + 0xAE4);
		pos.y = *reinterpret_cast<float*>((uintptr_t)(this) + 0xAE8);
		pos.z = *reinterpret_cast<float*>((uintptr_t)(this) + 0xAEC);
		return pos;
	}

	std::vector<Actor*> getRuntimeActorList() {
		std::vector<Actor*> listOut;
		MemoryUtils::CallVFunc<294, decltype(&listOut)>(this, &listOut);
		return listOut;
	}
};
