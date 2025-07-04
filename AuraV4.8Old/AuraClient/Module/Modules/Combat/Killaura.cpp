#include "Killaura.h"
#include "../pch.h"
#include "../../../../SDK/CAttribute.h"
using namespace std;
int keybinds = false;
Killaura::Killaura() : IModule(0, Category::COMBAT, "Advanced Killaura deved by Afar Arkaris, Nakeanl, Zh1n1anQwQ, Man Lay214, xIhyna, Iammtrashhh, DualNebul090112") { //remove // from things.

	registerEnumSetting("Rotations", &rotations, 0);
	rotations.addEntry("None", 0);
	rotations.addEntry("Silent", 1);
	rotations.addEntry("Client", 2);
	rotations.addEntry("Actual", 3);
	registerEnumSetting("MetaRots", &metarots, 0);
	metarots.addEntry("Normal", 0);
	metarots.addEntry("Small", 1);
	metarots.addEntry("Custom", 2);
	registerEnumSetting("Mode", &mode, 0);
	mode.addEntry("Single", 0);
	mode.addEntry("Multi", 1);
	registerEnumSetting("Timer", &timer, 0);
	timer.addEntry("None", 0);
	timer.addEntry("Nukkit", 1);
	timer.addEntry("Vanilla", 2);
	registerIntSetting("Timer TPS", &tps, tps, 20, 50);
	registerIntSetting("APS", &this->APS, this->APS, 1, 50);
	registerBoolSetting("APSBoost", &this->boost, this->boost);
	registerIntSetting("Boost", &this->apsBoost, this->apsBoost, 1, 200);
	registerEnumSetting("AutoWeapon", &AutoWeapon, 0);
	AutoWeapon.addEntry("None", 0);
	AutoWeapon.addEntry("Regular", 1);
	AutoWeapon.addEntry("TrueSilent", 3); // Switches to trident when on water sht up bitch it doesnt idot
	//RotSpeed
	registerIntSetting(" rotationSpeed", &rotationSpeed, rotationSpeed, 10, 180);
	// Packet
	registerBoolSetting("PacketAttack", &pkt, pkt);
	registerIntSetting("Packet", &packetattack, 20, 0, 100);
	registerIntSetting("HitAttempts", &packet, packet, 1, 10);
	registerFloatSetting("Range", &range, range, 3.f, 30.f);

	registerBoolSetting("Criticals", &Critical, Critical);
	registerIntSetting("MinHealth", &stoph, stoph, 0, 36);
	registerBoolSetting("StopOnEat", &StopOnEat, StopOnEat);
	registerBoolSetting("MobAura", &mobs, mobs);
	registerBoolSetting("Visualize", &visualize, visualize);
	registerBoolSetting("Hold", &hold, hold);
	registerBoolSetting("CPSAura", &cps, cps);
	registerBoolSetting("Swing", &swingKA, swingKA);
	registerBoolSetting("HurtTime", &hurttimeka, hurttimeka);
}

void Killaura::onKeyUpdate(int keyb, bool isDown) {
	if (g_Data.getLocalPlayer() == nullptr) return;
	if (!isDown) return;
	if (keyb == this->getKeybind()) this->toggle();
	if (!this->isEnabled()) return;
	if (keyb == keybinds) {
		if (mode.getSelectedValue() == 0) {
			mode.selected = 1;
			clientMessageF("%sChanged Rotations Mode! {Normal}", GREEN);
		}
		else if (mode.getSelectedValue() == 1) {
			mode.selected = 2;
			clientMessageF("%sChanged Rotations Mode! {Small}", GREEN);
		}
	}
}

void Killaura::doCritical() {
	if (Critical) {
		C_MovePlayerPacket p(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
		p.onGround = false;
		p.Position.y = g_Data.getLocalPlayer()->getPos()->y + 1;
		//g_Data.getLoopbackPacketSender()->sendToServer(&p);

		C_MovePlayerPacket p2(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
		p2.onGround = false;
		p2.Position.y = g_Data.getLocalPlayer()->getPos()->y + 0.5f;
		//g_Data.getLoopbackPacketSender()->sendToServer(&p2);
	}
}

decltype(GetTickCount64()) start;
bool Counter(double a1) {
	if ((GetTickCount64() - start) >= a1) {
		start = GetTickCount64();
		return true;
	}
	return false;
}

void Killaura::findWeapon()
{
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	static auto rainMode = moduleMgr->getModule<RainMode>();
	float damage = 0;
	int slot = supplies->selectedHotbarSlot;
	for (int n = 0; n < 9000; n++)
	{
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr)
		{
			float currentDamage = stack->getAttackingDamageWithEnchants();
			if (currentDamage > damage && stack->getItem()->itemId == 316 || currentDamage > damage && stack->getItem()->itemId == 604)
			{
				damage = currentDamage;
				slot = n;

			}
		}
	}
	supplies->selectedHotbarSlot = slot;
}

void Killaura::findWeaponRain()
{
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	static auto rainMode = moduleMgr->getModule<RainMode>();
	float damage = 0;
	int slot = supplies->selectedHotbarSlot;
	for (int n = 0; n < 9000; n++)
	{
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr)
		{
			float currentDamage = stack->getAttackingDamageWithEnchants();
			if (rainMode->isEnabled())
				if (currentDamage > damage && stack->getItem()->itemId == 546)
				{
					damage = currentDamage;
					slot = n;
				}

		}
	}
	supplies->selectedHotbarSlot = slot;
}

void Killaura::findWeaponWater()
{
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	float damage = 0;
	int slot = supplies->selectedHotbarSlot;
	for (int n = 0; n < 9000; n++)
	{
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr)
		{
			float currentDamage = stack->getAttackingDamageWithEnchants();
			if (currentDamage > damage && stack->getItem()->itemId == 546)
			{
				damage = currentDamage;
				slot = n;
			}

		}
	}
	supplies->selectedHotbarSlot = slot;
}

void Killaura::findWeaponSmart()
{
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	float damage = 0;
	int slot = supplies->selectedHotbarSlot;
	for (int n = 0; n < 9000; n++)
	{
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr)
		{
			float currentDamage = stack->getAttackingDamageWithEnchants();
			if (currentDamage > damage && stack->getItem()->itemId == 546)
			{
				damage = currentDamage;
				slot = n;
			}
		}
	}
	supplies->selectedHotbarSlot = slot;
}

const char* Killaura::getRawModuleName() {
	return "KillauraNA";
}

const char* Killaura::getModuleName() {
	switch (mode.getSelectedValue()) {
	case 0: name = string("Killaura ") + string(GRAY) + string("[") + string(WHITE) + string("Single") + string(GRAY) + string("]"); break;
	case 1: name = string("Killaura ") + string(GRAY) + string("[") + string(WHITE) + string("Multi") + string(GRAY) + string("]"); break;
	}
	return name.c_str();
}

struct CompareTargetEnArray {
	bool operator()(C_Entity* lhs, C_Entity* rhs) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};

static std::vector <C_Entity*> targetList;

void findEntity(C_Entity* currentEntity, bool isRegularEntity) {
	static auto killaura = moduleMgr->getModule<Killaura>();

	if (currentEntity == nullptr) return;
	if (currentEntity == g_Data.getLocalPlayer()) return;
	if (!g_Data.getLocalPlayer()->canAttack(currentEntity, false)) return;
	if (!g_Data.getLocalPlayer()->isAlive()) return;
	if (!currentEntity->isAlive()) return;
	if (currentEntity->getEntityTypeId() == 66) return; // falling block
	if (currentEntity->getEntityTypeId() == 69) return; // XP
	if (currentEntity->getEntityTypeId() == 64) return; // item
	if (currentEntity->getEntityTypeId() == 80 || currentEntity->getEntityTypeId() == 69) return;

	if (killaura->mobs) {
		if (currentEntity->getNameTag()->getTextLength() <= 1 && currentEntity->getEntityTypeId() == 319) return;
		if (currentEntity->width <= 0.01f || currentEntity->height <= 0.01f) return;
		if (currentEntity->getEntityTypeId() == 0xC00050) return; // Arrows - new item id
		if (currentEntity->getEntityTypeId() == 307) return; // NPC
	}
	else {
		if (!TargetUtil::isValidTarget(currentEntity)) return;
		if (currentEntity->getEntityTypeId() == 51 || currentEntity->getEntityTypeId() == 1677999) return;
	}

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
	if (dist < killaura->range) {
		targetList.push_back(currentEntity);
		sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
	}
}



void Killaura::onEnable() {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
	targetListEmpty = true;
	targetList.clear();
}

void Killaura::onDisable() {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
	targetListEmpty = true;
	targetList.clear();
	g_Data.getClientInstance()->minecraft->setTimerSpeed(20.f);
}

void Killaura::onTick(C_GameMode* gm) {

	auto* localPlayer = g_Data.getLocalPlayer();


	auto player = g_Data.getLocalPlayer();
	g_Data.getClientInstance()->minecraft->setTimerSpeed(Timer);
	int dbrr = g_Data.getLocalPlayer()->getSelectedItemId();
	if (player == nullptr) return;


	PointingStruct* pointing = g_Data.getLocalPlayer()->pointingStruct;
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	targetListEmpty = targetList.empty();

	targetList.clear();

	g_Data.forEachEntity(findEntity);
	Odelay++;
	slotKA = supplies->selectedHotbarSlot;

	if (localPlayer->getHealth() <= stoph) {
		targetList.clear();
		g_Data.getClientInstance()->minecraft->setTimerSpeed(20.f);
		return;
	}

	if (localPlayer->getC_ItemUseDuration() > 0) {
		targetList.clear();
		g_Data.getClientInstance()->minecraft->setTimerSpeed(20.f);
		return;
	}

	if (timer.getSelectedValue() == 2)
	{
		g_Data.getClientInstance()->minecraft->setTimerSpeed(tps);
	}
	if (timer.getSelectedValue() == 1)
	{
		g_Data.getClientInstance()->minecraft->setTimerSpeed(26.f);
	}


	if (hold && !g_Data.isLeftClickDown())
		return;

	if (!targetList.empty() && Odelay) {
		if (mode.getSelectedValue() == 0) {
			if (packet = 1) {
				for (auto& i : targetList) {
					if (!(i->damageTime > 1 && hurttimeka)) {
						gm->attack(targetList[0]);
						if (swingKA) {
							player->swing();
						}
					}
				}
			}

			if (packet = 2) {
				for (auto& i : targetList) {
					if (!(i->damageTime > 1 && hurttimeka)) {
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						if (swingKA) {
							player->swing();
						}
					}
				}
			}
			if (packet = 3) {
				for (auto& i : targetList) {
					if (!(i->damageTime > 1 && hurttimeka)) {
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						if (swingKA) {
							player->swing();
						}
					}
				}
			}
			if (packet = 4) {
				for (auto& i : targetList) {
					if (!(i->damageTime > 1 && hurttimeka)) {
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						if (swingKA) {
							player->swing();
						}
					}
				}
			}
			if (packet = 5) {
				for (auto& i : targetList) {
					if (!(i->damageTime > 1 && hurttimeka)) {
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						if (swingKA) {
							player->swing();
						}
					}
				}
			}
			if (packet = 6) {
				for (auto& i : targetList) {
					if (!(i->damageTime > 1 && hurttimeka)) {
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						if (swingKA) {
							player->swing();
						}
					}
				}
			}
			if (packet = 7) {
				for (auto& i : targetList) {
					if (!(i->damageTime > 1 && hurttimeka)) {
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						if (swingKA) {
							player->swing();
						}
					}
				}
			}
			if (packet = 8) {
				for (auto& i : targetList) {
					if (!(i->damageTime > 1 && hurttimeka)) {
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						if (swingKA) {
							player->swing();
						}
					}
				}
			}
			if (packet = 9) {
				for (auto& i : targetList) {
					if (!(i->damageTime > 1 && hurttimeka)) {
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						if (swingKA) {
							player->swing();
						}
					}
				}
			}
			if (packet = 10) {
				for (auto& i : targetList) {
					if (!(i->damageTime > 1 && hurttimeka)) {
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						if (swingKA) {
							player->swing();
						}
					}
				}
			}
			if (cps) {
				for (auto& i : targetList) {
					if (!(i->damageTime > 1 && hurttimeka)) {
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						if (swingKA) {
							player->swing();
						}
					}
				}
			}
		}

		if (mode.getSelectedValue() == 1) {
			for (auto& i2 : targetList) {
				if (!(i2->damageTime > 1 && hurttimeka)) {
					gm->attack(i2);
					gm->attack(i2);
					gm->attack(i2);
					gm->attack(i2);
					gm->attack(i2);
					if (swingKA) {
						player->swing();

					}
				}
			}
		}
		if (AutoWeapon.getSelectedValue() == 3)
			supplies->selectedHotbarSlot = slotKA;
		if (rotations.getSelectedValue() == 3) {
			tro = g_Data.getLocalPlayer()->getPos()->CalcAngle(*targetList[0]->getPos());
			Odelay = 0;

			if (pkt) {
				for (int i = 0; i < packetattack; i++) {
					if (mode.getSelectedValue() == 1) {
						for (auto& i : targetList) {
							if (!(i->damageTime > 1 && hurttime)) {
								g_Data.getLocalPlayer()->swing();
								g_Data.getCGameMode()->attack(i);
							}
						}
					}


					else {
						if (!(targetList[0]->damageTime > 1 && hurttime)) {
							g_Data.getLocalPlayer()->swing();
							g_Data.getCGameMode()->attack(targetList[0]);
						}
						break;
					}
				}
			}
		}
	}
}
void Killaura::onPlayerTick(C_Player* plr) {

	auto* localPlayer = g_Data.getLocalPlayer();
	if (plr == nullptr) return;

	if (!targetList.empty()) {
		if (hold && !g_Data.isLeftClickDown())
			return;

		for (auto& i : targetList) {
			vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*targetList[0]->getPos());
			vec2_t pos = g_Data.getLocalPlayer()->getPos()->CalcAngle(*i->getPos());

			if (rotations.getSelectedValue() == 2) {
				plr->yawUnused1 = angle.y;
				plr->pitch = angle.x;
				plr->bodyYaw = angle.y;
			}

			// Normal
			if (rotations.getSelectedValue() == 3) {
				if (metarots.getSelectedValue() == 0) {
					plr->setRot(tro);
					plr->yawUnused1 = angle.y+ rotationSpeed;
					plr->pitch = angle.x= rotationSpeed;
					plr->bodyYaw = angle.y= rotationSpeed;
					//plr->bodyYaw = angle.y;
				}
			}
			//Small
			if (rotations.getSelectedValue() == 3) {
				if (metarots.getSelectedValue() == 1) {
					plr->setRot(angle);
					plr->yawUnused1 = angle.y;
					plr->pitch = angle.x;
					plr->bodyYaw = angle.y;
				}
			}
		}
	}
}

void Killaura::onSendPacket(C_Packet* packet) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	if (!targetList.empty() && packet->isInstanceOf<C_MovePlayerPacket>() && (rotations.getSelectedValue() == 1 || rotations.getSelectedValue() == 2)) {
		vec2_t anglefd = g_Data.getLocalPlayer()->getPos()->CalcAngle(targetList[0]->eyePos0);
		auto* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);

		movePacket->pitch = anglefd.x;
		movePacket->headYaw = anglefd.y;
		movePacket->yaw = anglefd.y;

		vec2_t appl = angle.sub(localAngle).normAngles();
		if (abs(appl.y) > rotationSpeed) {
			if (appl.y > 0.f) {
				localAngle.y += rotationSpeed;
				angle.y = localAngle.y;
			}
			else if (appl.y < 0.f) {
				localAngle.y -= rotationSpeed;
				angle.y = localAngle.y;
			}
			return;
		}
	}
}
/*
if (packet->isInstanceOf<C_TextPacket>()) {
	auto* textPacket = reinterpret_cast<C_TextPacket*>(packet);
	textPacket->message = textPacket->message.getText() + std::string("| Test");
}
*/



void Killaura::onGetPickRange(C_GameMode* gm) {

;
static auto rainMode = moduleMgr->getModule<RainMode>();

	// APS
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();

	// Normal
	if (AutoWeapon.getSelectedValue() == 1 && !targetList.empty() && AutoWeapon.getSelectedValue() != 0)
		findWeapon();
	// If in Rain
	if (AutoWeapon.getSelectedValue() == 1 && !targetList.empty() && AutoWeapon.getSelectedValue() != 0)
		findWeaponRain();
	// If in water
	if (AutoWeapon.getSelectedValue() > 0 && !targetList.empty() && gm->player->isInWater() && AutoWeapon.getSelectedValue() != 0)
		findWeaponSmart();
	// If in air
	if (AutoWeapon.getSelectedValue() == 3 && !targetList.empty() && !gm->player->isInWater() && AutoWeapon.getSelectedValue() != 0)
		findWeapon();

	if (rainMode->isEnabled())
		if (AutoWeapon.getSelectedValue() == 3 && !targetList.empty() && AutoWeapon.getSelectedValue() != 0)
			findWeaponRain();


	if (Counter(1000.f / (float)APS)) {
		switch (mode.selected) {
			// Single
		case 0:
			if (boost) {
				for (int i = 0; i < apsBoost; i++) {
					for (auto& i : targetList) {
						if (!(i->damageTime > 1 && hurttime)) {
							g_Data.getCGameMode()->attack(targetList[0]);
							if (!moduleMgr->getModule<NoSwing>()->isEnabled() && canswing && swing)
								localPlayer->swing();
						}
					}
				}
			}

			else {
				for (auto& i : targetList) {
					if (!(i->damageTime > 1 && hurttime)) {
						g_Data.getCGameMode()->attack(targetList[0]);
						if (!moduleMgr->getModule<NoSwing>()->isEnabled() && canswing && swing)
							localPlayer->swing();
					}
				}
			}
			break;

			// Multi
		case 1:
			if (boost) {
				for (int i = 0; i < apsBoost; i++) {
					for (auto& i : targetList) {
						if (!(i->damageTime > 1 && hurttime)) {
							g_Data.getCGameMode()->attack(i);
							if (!moduleMgr->getModule<NoSwing>()->isEnabled() && canswing && swing)
								localPlayer->swing();
						}
					}
				}
			}
			else {
				for (auto& i : targetList) {
					if (!(i->damageTime > 1 && hurttime)) {
						g_Data.getCGameMode()->attack(i);
						if (!moduleMgr->getModule<NoSwing>()->isEnabled() && canswing && swing)
							localPlayer->swing();
					}
				}
			}
			break;
		}
	}
}