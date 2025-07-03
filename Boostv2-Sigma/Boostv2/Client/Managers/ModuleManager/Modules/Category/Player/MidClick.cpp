#include "MidClick.h"
#include "../../../../../../SDK/Network/PacketSender.h"
#include "../../../../../Client.h"
bool hasClicked; 
MidClick::MidClick() : Module("MidClick", "Add or remove players from friends list by clicking the middle mouse button.", Category::PLAYER) {}

Actor* getActorFromEntityId32(EntityId entityId) {
	auto player = Game.getLocalPlayer();
	for (auto actor : player->level->getRuntimeActorList()) {
		if (actor->getEntityContext()->mEntity.mRawId == entityId.mRawId) {
			return actor;
		}
	}
	return nullptr;
}

void MidClick::onNormalTick(LocalPlayer* localPlayer) {
	if (GetAsyncKeyState(VK_MBUTTON) & 0x8000) {
		if (!hasClicked) {
			Level* level = localPlayer->level;
			if (level->getHitResult()->type == HitResultType::ENTITY) {
				Actor* hitActor = getActorFromEntityId32(level->getHitResult()->entity.id);
				if (hitActor != nullptr) {
					std::string playerName = *hitActor->getNameTag();
					if (friends.find(playerName) == friends.end()) {
						friends[playerName] = true;
						Client::DisplayClientMessage("%sSuccessfully added %s %sto your friendlist.", MCTF::GREEN, playerName.c_str(), MCTF::GREEN);
					}
					else {
						friends.erase(playerName);
						Client::DisplayClientMessage("%sSuccessfully removed %s %sfrom your friendlist.", MCTF::GREEN, playerName.c_str(), MCTF::GREEN);
					}
				}
			}
			hasClicked = true;
		}
	}
	else {
		hasClicked = false;
	}
}

bool MidClick::isFriend(const std::string& playerName) {
	return friends.find(playerName) != friends.end();
}

std::unordered_map<std::string, bool>& MidClick::getFriends() {
	return friends;
}