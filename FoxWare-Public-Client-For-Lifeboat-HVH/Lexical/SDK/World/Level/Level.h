#pragma once
#include "../../../Utils/MemoryUtil.h"
#include "../../Network/PacketSender.h"
#include "../Physical/HitResult.h"
#include <vector>

class Actor;
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
#pragma once

#include <cstdint>
#include <concepts>
#include <libhat.hpp>
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
		return hat::member_at<std::unordered_map<mcUUID, PlayerListEntry>>(this, 0x1BC8);
	}
	std::vector<Actor*> getRuntimeActorList() {
		std::vector<Actor*> listOut;
		MemoryUtil::CallVFunc<278, decltype(&listOut)>(this, &listOut);
		return listOut;
	}


	PacketSender* getPacketSender() {
		return MemoryUtil::CallVFunc<281, PacketSender*>(this);
	}

	HitResult* getHitResult() {
		return MemoryUtil::CallVFunc<288, HitResult*>(this);
	}
};
