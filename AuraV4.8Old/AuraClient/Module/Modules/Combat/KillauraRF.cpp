#include "KillauraRF.h"

KillauraRF::KillauraRF() : IModule('P', Category::COMBAT, "Credits to: Fox4u") {
	registerEnumSetting("Rotations", &RotMode, 0);
	RotMode = SettingEnum(this)
		.addEntry(EnumEntry("None", 0))
		.addEntry(EnumEntry("Silent", 1))
		.addEntry(EnumEntry("Badman", 2))
		.addEntry(EnumEntry("Horion", 3));
	this->registerBoolSetting("MultiAura", &this->isMulti, this->isMulti);
	this->registerBoolSetting("PacketAttack", &this->isMulti2, this->isMulti2);
	this->registerBoolSetting("MobAura", &this->isMobAura, this->isMobAura);
	this->registerFloatSetting("range", &this->range, this->range, 2.f, 20.f);
	this->registerIntSetting("delay", &this->delay, this->delay, 0, 20);
	this->registerIntSetting("timer", &this->timer, this->timer, 10, 24);
	this->registerIntSetting("Rotoffset", &this->packet, this->packet, -20, 50);
	this->registerIntSetting("RotMultiplier", &this->packet2, this->packet2, 0, 2);
	this->registerBoolSetting("hurttime", &this->hurttime, this->hurttime);
	this->registerBoolSetting("AutoWeapon", &this->autoweapon, this->autoweapon);
	this->registerBoolSetting("Visualise Target", &this->VisTarget, this->VisTarget);
}

KillauraRF::~KillauraRF() {
}

const char* KillauraRF::getModuleName() {
	return ("KillauraRF");
}

static std::vector<C_Entity*> targetList;

void findC_Entity2(C_Entity* currentC_Entity, bool isRegularC_Entity) {
	static auto KillauraRFMod = moduleMgr->getModule<KillauraRF>();

	if (currentC_Entity == nullptr)
		return;

	if (currentC_Entity == g_Data.getLocalPlayer())  // Skip Local player
		return;

	if (!g_Data.getLocalPlayer()->canAttack(currentC_Entity, false))
		return;

	if (!g_Data.getLocalPlayer()->isAlive())
		return;

	if (!currentC_Entity->isAlive())
		return;

	if (KillauraRFMod->isMobAura) {
		if (currentC_Entity->getNameTag()->getTextLength() <= 1 && currentC_Entity->getEntityTypeId() == 63) // this means target every mob a-part from the player!
			return;
		if (currentC_Entity->width <= 0.01f || currentC_Entity->height <= 0.01f)  // Don't hit this pesky antibot on 2b2e.org
			return;
		if (currentC_Entity->getEntityTypeId() == 64)  // item
			return;
	}
	else {
		if (!TargetUtil::isValidTarget(currentC_Entity))
			return;
	}

	float dist = (*currentC_Entity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < KillauraRFMod->range) {
		targetList.push_back(currentC_Entity);
	}
}


void KillauraRF::findWeapon() {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	float damage = 0;
	int slot = supplies->selectedHotbarSlot;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			float currentDamage = stack->getAttackingDamageWithEnchants();
			if (currentDamage > damage) {
				damage = currentDamage;
				slot = n;
			}
		}
	}
	supplies->selectedHotbarSlot = slot;
}

void KillauraRF::onTick(C_GameMode* gm) {
	g_Data.getClientInstance()->minecraft->setTimerSpeed(timer);
	switch (RotMode.selected) {
	case 0:
		silent = false;
		noSilent = false;
		longstrafe = false;
		break;
	case 1:
		silent = true;
		noSilent = false;
		longstrafe = false;
		break;
	case 2:
		silent = false;
		noSilent = true;
		longstrafe = false;
		break;
	case 3:
		silent = false;
		longstrafe = true;
		noSilent = false;
	}

	//Loop through all our players and retrieve their information
	targetList.clear();

	g_Data.forEachEntity(findC_Entity2);

	//cool 
	if (noSilent && !targetList.empty()) {
		vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*targetList[0]->getPos());
		gm->player->pitch = angle.y - packet2;
		gm->player->yaw = angle.y + packet * packet2;
		gm->player->yawUnused1 = angle.y + packet * packet2;
		gm->player->bodyYaw = angle.y - packet;
	}
	if (longstrafe && !targetList.empty()) {
		vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*targetList[0]->getPos());
		gm->player->bodyYaw = angle.y;
	}

	Odelay++;
	if (!targetList.empty() && Odelay >= delay) {
		if (autoweapon) findWeapon();

		if (g_Data.getLocalPlayer()->velocity.squaredxzlen() < 0.01) {
			C_MovePlayerPacket p(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p);  // make sure to update rotation if player is standing still
		}

		// Attack all C_Entitys in targetList
		if (isMulti) {
			for (auto& i : targetList) {
				if (!(i->damageTime > 1 && hurttime)) {
					g_Data.getLocalPlayer()->swing();
					g_Data.getCGameMode()->attack(i);
					g_Data.getLocalPlayer()->swing();
					g_Data.getCGameMode()->attack(i);
					g_Data.getLocalPlayer()->swing();
					g_Data.getCGameMode()->attack(i);
					g_Data.getLocalPlayer()->swing();
					g_Data.getCGameMode()->attack(i);
					g_Data.getLocalPlayer()->swing();
					g_Data.getCGameMode()->attack(i);
					g_Data.getLocalPlayer()->swing();
					g_Data.getLocalPlayer()->swing();
					g_Data.getCGameMode()->attack(i);
					g_Data.getLocalPlayer()->swing();
					g_Data.getCGameMode()->attack(i);
					g_Data.getLocalPlayer()->swing();
					g_Data.getCGameMode()->attack(i);
					g_Data.getLocalPlayer()->swing();
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					timer5++;
				}
				else {
					timer5 = packet;
				}
			}
		}
		else {
			if (isMulti2) {
				for (auto& i : targetList) {
					if (!(i->damageTime > 1 && hurttime)) {
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						g_Data.getCGameMode()->attack(i);
						timer5++;
					}
					else {
						timer5 = packet;
					}
				}
			}
			else {
				if (!(targetList[0]->damageTime > 1 && hurttime)) {
					g_Data.getLocalPlayer()->swing();
					g_Data.getCGameMode()->attack(targetList[0]);
					g_Data.getLocalPlayer()->swing();
					g_Data.getCGameMode()->attack(targetList[0]);
					g_Data.getLocalPlayer()->swing();
					g_Data.getCGameMode()->attack(targetList[0]);
					g_Data.getLocalPlayer()->swing();
					g_Data.getCGameMode()->attack(targetList[0]);
					g_Data.getLocalPlayer()->swing();
					g_Data.getCGameMode()->attack(targetList[0]);
					g_Data.getLocalPlayer()->swing();
					g_Data.getCGameMode()->attack(targetList[0]);
					g_Data.getLocalPlayer()->swing();
					g_Data.getCGameMode()->attack(targetList[0]);
					g_Data.getLocalPlayer()->swing();
					g_Data.getCGameMode()->attack(targetList[0]);
					g_Data.getLocalPlayer()->swing();
					g_Data.getCGameMode()->attack(targetList[0]);
					g_Data.getLocalPlayer()->swing();
					g_Data.getCGameMode()->attack(targetList[0]);
					g_Data.getLocalPlayer()->swing();
					g_Data.getLocalPlayer()->swing();
					g_Data.getCGameMode()->attack(targetList[0]);
					g_Data.getLocalPlayer()->swing();
					g_Data.getCGameMode()->attack(targetList[0]);
					g_Data.getLocalPlayer()->swing();
					g_Data.getCGameMode()->attack(targetList[0]);
					g_Data.getLocalPlayer()->swing();
					g_Data.getCGameMode()->attack(targetList[0]);
					g_Data.getLocalPlayer()->swing();
					g_Data.getLocalPlayer()->swing();
					g_Data.getCGameMode()->attack(targetList[0]);
					g_Data.getLocalPlayer()->swing();
					g_Data.getCGameMode()->attack(targetList[0]);
					g_Data.getCGameMode()->attack(targetList[0]);
					g_Data.getCGameMode()->attack(targetList[0]);
					timer5++;
				}
				else {
					timer5 = packet;
				}
			}
			Odelay = packet;
		}
	}
}

void KillauraRF::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr)
		this->setEnabled(false);
	timer5 = packet;
}
void KillauraRF::onDisable() {
	g_Data.getClientInstance()->minecraft->setTimerSpeed(20.f);
}

void KillauraRF::onSendPacket(C_Packet* packet) {
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

float t1 = 0;
void KillauraRF::onLevelRender() {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	targetList.clear();
}