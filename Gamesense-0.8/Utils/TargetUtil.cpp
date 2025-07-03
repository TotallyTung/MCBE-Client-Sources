#include "TargetUtil.h"
#include <regex>

C_LocalPlayer** localPlayer;

void TargetUtil::init(C_LocalPlayer** cl) {
	localPlayer = cl;
}

bool TargetUtil::isValidTarget(C_Entity* ent) {

	if (ent == NULL) return false;
	if (ent == g_Data.getLocalPlayer()) return false;
	if (!ent->isAlive()) return false;
	if (!(*localPlayer)->canAttack(ent, false)) return false;
	if (ent->getEntityTypeId() != 319) return false;
	if (ent->width <= 0.01f || ent->height <= 0.01f) return false;
	if (FriendList::findPlayer(ent->getNameTag()->getText())) return false;

	return true;
}

bool TargetUtil::containsOnlyASCII(const std::string& string) {
	for (auto c : string) {
		int n = static_cast<unsigned char>(c);
		if (n > 127 && n != -89) {
			return false;
		}
	}
	return true;
}