#include "Target.h"

#include <regex>
#include "../Horion/FriendList/FriendsManager.h"

#include "../Horion/Module/ModuleManager.h"

bool Target::isValidTarget(Entity* ent) {

	auto localPlayer = Game.getLocalPlayer();

	if (ent == localPlayer)
		return false;

	static auto antibot = moduleMgr->getModule<AntiBot>();
	static auto hitboxMod = moduleMgr->getModule<Hitbox>();
	static auto noFriends = moduleMgr->getModule<NoFriends>();

	if (!Game.isInGame())
		return false;
	if (!ent->isAlive())
		return false;

	auto entityTypeId = ent->getEntityTypeId();

	if (entityTypeId != 319) return false;

	if (antibot->isEntityIdCheckEnabled() && entityTypeId <= 130 && entityTypeId != 63)
		return false;

	// Temporarily removed from gui, tons of false negatives
	if (antibot->isNameCheckEnabled() && !Target::containsOnlyASCII(ent->getNameTag()->getText()))
		return false;

	if (!noFriends->isEnabled() && FriendsManager::findFriend(ent->getNameTag()->getText()))
		return false;

	if (antibot->isInvisibleCheckEnabled() && ent->isInvisible())
		return false;

	if (antibot->isOtherCheckEnabled() && (ent->isSilent() || ent->isImmobile() || ent->getNameTag()->getTextLength() < 1 || std::string(ent->getNameTag()->getText()).find(std::string("\n")) != std::string::npos))
		return false;

	if (!hitboxMod->isEnabled() && antibot->isHitboxCheckEnabled())
		if ((ent->height < 1.5f || ent->width < 0.49f || ent->height > 2.1f || ent->width > 0.9f))
			return false;

	if (!localPlayer->canAttack(ent, false))
		return false;

	if (antibot->isExtraCheckEnabled() && !ent->canShowNameTag())
		return false;

	return (ent != nullptr);
	return true;
}

bool Target::containsOnlyASCII(const std::string& string) {
	for (auto c : string) {
		int n = static_cast<unsigned char>(c);
		if (n > 127 && n != -89) {
			return false;
		}
	}
	return true;
}
