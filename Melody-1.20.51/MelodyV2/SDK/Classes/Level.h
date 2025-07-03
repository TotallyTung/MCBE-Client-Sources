// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "Actor.h"
#include "../../Utils/Math.h"
#include <cstdint>
#include "../../Utils/MemoryUtils.h"
#include "unordered_map"
class Actor;
class SerializedSkin {};
enum class BuildPlatform : int32_t;
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
class mcUUID {
public:
	uint64_t mostSig, leastSig;
};
class PlayerListEntry {
public:
	uint64_t id; // This is the ActorUniqueID
	mcUUID UUID;
	std::string name, XUID, platformOnlineId;
	int buildPlatform;
	char filler[0x164];
	//SerializedSkin skin;
	bool isTeacher, isHost;
};
class Level {
public:
	static std::vector<std::pair<mcUUID, PlayerListEntry>>
		copyMapInAlphabeticalOrder(const std::unordered_map<mcUUID, PlayerListEntry>& sourceMap) {
		std::vector<std::pair<mcUUID, PlayerListEntry>> sortedPairs(sourceMap.begin(), sourceMap.end());

		// Sort the vector based on the 'name' field
		std::sort(sortedPairs.begin(), sortedPairs.end(), [](const auto& a, const auto& b) {
			return a.second.name < b.second.name;
			});

		return sortedPairs;
	}
	std::unordered_map<mcUUID, PlayerListEntry>& getPlayerMap() {
		return hat::member_at<std::unordered_map<mcUUID, PlayerListEntry>>(this, 0x1EA8);
	}
	//CLASS_FIELD(playerListEntries, 0x1EA8, std::unordered_map<mce::UUID, PlayerListEntry>);  // xref: Level::getPlayerList 0x1EA8
	//get_UniqueName 0x40
	//BUILD_ACCESS(this, std::unordered_map<mce::UUID, PlayerListEntry>, playerListEntries, 0x1EA8);
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
