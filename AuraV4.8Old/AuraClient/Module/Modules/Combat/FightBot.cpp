#include "FightBot.h"

FightBot::FightBot() : IModule(0x0, Category::COMBAT, "Attacks entities around you automatically") {
	registerIntSetting("TargetRange", &targ, targ, 5, 500);
	registerBoolSetting("MobAura", &isMobAura, isMobAura);
	registerBoolSetting("Silent Rotations", &silent, silent);
}

FightBot::~FightBot() {
}

const char* FightBot::getModuleName() {
	return "FightBot";
}

struct CompareTargetEnArray {
	bool operator()(C_Entity* lhs, C_Entity* rhs) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};

static std::vector<C_Entity*> targetList;
void findEntityFightBot(C_Entity* currentEntity, bool isRegularEntity) {
	std::sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
	static auto FightBotMod = moduleMgr->getModule<FightBot>();

	if (currentEntity == nullptr)
		return;

	if (currentEntity == g_Data.getLocalPlayer())  // Skip Local player
		return;

	if (!g_Data.getLocalPlayer()->canAttack(currentEntity, false))
		return;

	if (!g_Data.getLocalPlayer()->isAlive())
		return;

	if (!currentEntity->isAlive())
		return;

	if (currentEntity->width <= 0.1f || currentEntity->height <= 0.1f)  // Don't hit this pesky antibot on 2b2e.org
		return;

	if (FightBotMod->isMobAura) {
		if (currentEntity->getNameTag()->getTextLength() <= 1 && currentEntity->getEntityTypeId() == 63)
			return;
		if (currentEntity->width <= 0.01f || currentEntity->height <= 0.01f)  // Don't hit this pesky antibot on 2b2e.org
			return;
		if (currentEntity->getEntityTypeId() == 64)  // item
			return;
		if (currentEntity->getEntityTypeId() == 69)  // xp
			return;
	}
	else {
		if (!TargetUtil::isValidTarget(currentEntity))
			return;
	}

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
	if (dist < FightBotMod->targ) {
		targetList.push_back(currentEntity);
	}
}

void FightBot::onTick(C_GameMode* gm) {
	std::sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
	targetListA = targetList.empty();
	if (g_Data.isInGame()) {
		g_Data.forEachEntity(findEntityFightBot);
		if (!targetList.empty()) {
			g_Data.getClientInstance()->getMoveTurnInput()->forward = true;
		}
	}
}

void FightBot::onLevelRender() {
	std::sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
	targetListA = targetList.empty();
	if (g_Data.isInGame()) {
		targetList.clear();
		g_Data.forEachEntity(findEntityFightBot);

		if (!targetList.empty()) {
			if (sexy) {
				joe = g_Data.getLocalPlayer()->getPos()->CalcAngle(*targetList[0]->getPos()).normAngles();
				C_LocalPlayer* player = g_Data.getLocalPlayer();
				vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*targetList[0]->getPos()).normAngles();
				player->bodyYaw = angle.x;
				player->bodyYaw = angle.y;
			}
		}
	}
	if (!g_Data.isInGame())
		setEnabled(false);
}

void FightBot::onEnable() {
	std::sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
	targetList.clear();
	if (g_Data.isInGame()) {
		if (g_Data.getLocalPlayer() == nullptr)
			setEnabled(false);
	}
}
void FightBot::onDisable() {
	targetList.clear();
	g_Data.getClientInstance()->getMoveTurnInput()->forward = false;
	g_Data.getClientInstance()->getMoveTurnInput()->clearMovementState();
}

void FightBot::onSendPacket(C_Packet* packet, bool& cancelsend) {
	if (g_Data.isInGame()) {
		if (!g_Data.isInGame()) {
		}
		targetListA = targetList.empty();
		targetList.clear();
		g_Data.forEachEntity(findEntityFightBot);
		std::sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
		if (packet->isInstanceOf<C_MovePlayerPacket>() && g_Data.getLocalPlayer() != nullptr && silent) {
			if (!targetList.empty()) {
				auto* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
				vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*targetList[0]->getPos());
				movePacket->pitch = angle.x;
				movePacket->headYaw = angle.y;
				movePacket->yaw = angle.y;
			}
		}
	}
}