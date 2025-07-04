#include "TPAura.h"

TPAura::TPAura() : IModule(0x0, Category::COMBAT, "TP around your enemy. Bypass added by Afar Arkaris") {
	this->registerBoolSetting("MobAura", &this->isMobAura, this->isMobAura);
	this->registerBoolSetting("MultiAura", &this->isMulti, this->isMulti);
	this->registerBoolSetting("Lerp", &this->lerp, this->lerp);
	this->registerBoolSetting("UnlimitedRange", &this->unlim, this->unlim);
	this->registerBoolSetting("Attack", &this->TPAttack, this->TPAttack);
	this->registerFloatSetting("range", &this->range, this->range, 0.5f, 20.f);
	this->registerIntSetting("TP delay", &this->delay, this->delay, 0, 20);
	this->registerIntSetting("Attack delay", &this->delay1, this->delay1, 0, 20);
	//this->registerBoolSetting("AutoWeapon", &this->autoweapon, this->autoweapon);
	//this->registerBoolSetting("Silent Rotations", &this->silent, this->silent);
	this->registerFloatSetting("Radius", &this->radius, this->radius, 0.5f, 2.5f);
	this->registerFloatSetting("Y", &this->y, this->y, -1.5f, 3.0f);
}

TPAura::~TPAura() {
}

const char* TPAura::getModuleName() {
	return ("TPAura");
}

static std::vector<C_Entity*> targetList;

struct CompareTargetEnArray {
	bool operator()(C_Entity* lhs, C_Entity* rhs) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};

void findEntity0(C_Entity* currentEntity, bool isRegularEntity) {
	static auto TPMod = moduleMgr->getModule<TPAura>();

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

	if (TPMod->isMobAura) {
		if (currentEntity->getNameTag()->getTextLength() <= 1 && currentEntity->getEntityTypeId() == 319)
			return;
		if (currentEntity->width <= 0.01f || currentEntity->height <= 0.01f) // Don't hit this pesky antibot on 2b2e.org
			return;
		if (currentEntity->getEntityTypeId() == 64) // item
			return;
	}
	else {
		if (!TargetUtil::isValidTarget(currentEntity))
			return;
	}

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
	if (!TPMod->unlim) {
		if (dist < TPMod->range) {
			targetList.push_back(currentEntity);
			//sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
		}
	}
	else {
		if (dist < TPMod->unlimRange) {
			targetList.push_back(currentEntity);
			//sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
		}
	}
	sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
}

void TPAura::onTick(C_GameMode* gm) {

	auto player = g_Data.getLocalPlayer();
	if (g_Data.getLocalPlayer() == nullptr) return;

	//Loop through all our players and retrieve their information
	targetList.clear();

	g_Data.forEachEntity(findEntity0);

	Odelay++;
	if (!targetList.empty() && Odelay >= delay) {
		vec3_t pos = *targetList[0]->getPos();
		if (turn == 3) {
			pos.y += y;
			pos.x += -radius;
			pos.z += radius;
			turn = 0;
		}
		else if (turn == 2) {
			pos.y += y;
			pos.x += -radius;
			pos.z += -radius;
			turn = 3;
		}
		else if (turn == 1) {
			pos.y += y;
			pos.x += radius;
			pos.z += -radius;
			turn = 2;
		}
		else if (turn == 0) {
			pos.y += y;
			pos.x += radius;
			pos.z += radius;
			turn = 1;
		}
		//if (g_Data.getLocalPlayer()->velocity.squaredxzlen() < 0.01) {
		//C_MovePlayerPacket p(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
		//_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p);
		//}
		C_LocalPlayer* player = g_Data.getLocalPlayer();
		if (!lerp) {
			player->setPos(pos);
		}
		else {
			float dist2 = gm->player->getPos()->dist(pos);
			//player->lerpMotion(pos);
			player->lerpTo(pos, vec2_t(1, 0.1), (int)fmax((int)dist2 * 0.1, 0.1));
			//player->lerpMotion(pos);
		}
		Odelay = 0;
	}

	delay2++;
	g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = false;
	if (!targetList.empty() && delay2 >= delay1 && TPAttack) {

		if (isMulti) {
			for (auto& i : targetList) {
				g_Data.getLocalPlayer()->swing();
				g_Data.getCGameMode()->attack(i);
			}
		}
		else {
			g_Data.getLocalPlayer()->swing();
			g_Data.getCGameMode()->attack(targetList[0]);
		}
		delay2 = 0;
	}
}

void TPAura::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr)
		this->setEnabled(false);
}

