#include "Criticals.h"

Criticals::Criticals() : IModule(0, Category::COMBAT, "Each hit is a critical hit") {
	registerBoolSetting("Hurttime", &hurttime, hurttime);
}

Criticals::~Criticals() {
}

const char* Criticals::getModuleName() {
	return ("Criticals");
}
static std::vector<C_Entity*> targetListA;
void findEntity_Criticals(C_Entity* currentEntity, bool isRegularEntity) {
	static auto criticals = moduleMgr->getModule<Criticals>();

	if (currentEntity == nullptr)
		return;

	if (currentEntity == g_Data.getLocalPlayer())
		return;

	if (!g_Data.getLocalPlayer()->canAttack(currentEntity, false))
		return;

	if (!g_Data.getLocalPlayer()->isAlive())
		return;

	if (!currentEntity->isAlive())
		return;

	if (currentEntity->getEntityTypeId() == 69)  // XP
		return;

	if (currentEntity->getEntityTypeId() == 80)  // Arrows
		return;

	if (currentEntity->getEntityTypeId() == 1677999)  // Villager
		return;

	if (currentEntity->getEntityTypeId() == 51)  // NPC
		return;

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < 8)
		targetListA.push_back(currentEntity);
}
void Criticals::onSendPacket(C_Packet* packet) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
	targetListA.clear();

	g_Data.forEachEntity(findEntity_Criticals);
	if (packet->isInstanceOf<C_MovePlayerPacket>() && g_Data.getLocalPlayer() != nullptr) {
		C_MovePlayerPacket* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
		if (g_Data.canUseMoveKeys() && !targetListA.empty()) {
			if (hurttime && targetListA[0]->damageTime < 1)
				return;
			for (auto& i : targetListA) {
				movePacket->onGround = false;
			}
		}
	}
}