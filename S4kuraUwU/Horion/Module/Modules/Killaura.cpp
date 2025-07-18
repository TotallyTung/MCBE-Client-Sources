#include "Killaura.h"

Killaura::Killaura() : IModule('P', Category::COMBAT, "Attacks entities around you automatically") {
	registerBoolSetting("MultiAura", &isMulti, isMulti);
	registerBoolSetting("MobAura", &isMobAura, isMobAura);
	//registerBoolSetting("Aurora", &isnotfast, isnotfast);
	registerFloatSetting("range", &range, range, 2.f, 50.f);
	registerIntSetting("delay", &delay, delay, 0, 20);
	registerBoolSetting("hurttime", &hurttime, hurttime);
	registerBoolSetting("AutoWeapon", &autoweapon, autoweapon);
	registerBoolSetting("Rotations", &rotations, rotations);
	registerBoolSetting("Silent Rotations", &silent, silent);
	registerBoolSetting("SwitchToGap", &SwitchToGap, SwitchToGap);
}

Killaura::~Killaura() {
}

const char* Killaura::getModuleName() {
	return ("Killaura");
}

static std::vector<C_Entity*> targetList;
void findEntity(C_Entity* currentEntity, bool isRegularEntity) {
	static auto killauraMod = moduleMgr->getModule<Killaura>();

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

	if (currentEntity->getEntityTypeId() == 66) // falling block
		return;
	  
	if (currentEntity->getEntityTypeId() == 69)  // XP
		return;

	if (killauraMod->isMobAura) {
		if (currentEntity->getNameTag()->getTextLength() <= 1 && currentEntity->getEntityTypeId() == 63)
			return;
		if(currentEntity->width <= 0.01f || currentEntity->height <= 0.01f) // Don't hit this pesky antibot on 2b2e.org
			return;
		if(currentEntity->getEntityTypeId() == 64) // item
			return;
		if (currentEntity->getEntityTypeId() == 0xC00050)  // Arrows - new item id
			return;
		if (currentEntity->getEntityTypeId() == 51) // NPC
			return;
	} else {
		if (!Target::isValidTarget(currentEntity))
			return;
	}

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < killauraMod->range) {
		targetList.push_back(currentEntity);
	}
}

void Killaura::findWeapon() {
	__int64 id = *g_Data.getLocalPlayer()->getUniqueId();
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	float damage = 1;
	int slot = supplies->selectedHotbarSlot;
	for (int n = 1; n < 9; n++) {
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

void Killaura::onTick(C_GameMode* gm) {
	targetListEmpty = targetList.empty();
	// Loop through all our players and retrieve their information
	targetList.clear();

	g_Data.forEachEntity(findEntity);

	Odelay++;
	if (!targetList.empty() && Odelay >= delay) {
		if (autoweapon) findWeapon();

		if (g_Data.getLocalPlayer()->velocity.squaredxzlen() < 0.01) {
			C_MovePlayerPacket p(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p);  // make sure to update rotation if player is standing still
		}

		// Attack all entitys in targetList
		if (isMulti) {
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
					g_Data.getLocalPlayer()->swing();
				}
			}
		} else {
			if (!(targetList[0]->damageTime > 1 && hurttime)) {
				g_Data.getCGameMode()->attack(targetList[0]);
				g_Data.getCGameMode()->attack(targetList[0]);
				g_Data.getCGameMode()->attack(targetList[0]);
				g_Data.getCGameMode()->attack(targetList[0]);
				g_Data.getCGameMode()->attack(targetList[0]);
				g_Data.getCGameMode()->attack(targetList[0]);
				g_Data.getCGameMode()->attack(targetList[0]);
				g_Data.getCGameMode()->attack(targetList[0]);
				g_Data.getCGameMode()->attack(targetList[0]);
				g_Data.getCGameMode()->attack(targetList[0]);
				g_Data.getCGameMode()->attack(targetList[0]);
				g_Data.getLocalPlayer()->swing();
			}
		}
		if (isnotfast) {
			for (auto& i : targetList) {
				if (!(i->damageTime > 1 && hurttime)) {
					g_Data.getLocalPlayer()->swing();
					g_Data.getCGameMode()->attack(i);
				}
			}
		} else {
			if (!(targetList[0]->damageTime > 1 && hurttime)) {
				g_Data.getCGameMode()->attack(targetList[0]);
				g_Data.getCGameMode()->attack(targetList[0]);
				g_Data.getCGameMode()->attack(targetList[0]);
				g_Data.getCGameMode()->attack(targetList[0]);
				g_Data.getCGameMode()->attack(targetList[0]);
				g_Data.getCGameMode()->attack(targetList[0]);
				g_Data.getCGameMode()->attack(targetList[0]);
				g_Data.getCGameMode()->attack(targetList[0]);
				g_Data.getCGameMode()->attack(targetList[0]);
				g_Data.getCGameMode()->attack(targetList[0]);
				g_Data.getCGameMode()->attack(targetList[0]);
				g_Data.getLocalPlayer()->swing();
				
			}
		}
		if (rotations) {
			angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*targetList[0]->getPos());
		}
		Odelay = 0;
	}
}
void Killaura::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr)
		setEnabled(false);
}
void Killaura::onDisable() {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	__int64 id = *g_Data.getLocalPlayer()->getUniqueId();
	if (SwitchToGap) {
		auto inv = g_Data.getLocalPlayer()->getSupplies()->inventory;
		for (int i = 0; i < 9; i++) {
			C_ItemStack* stack = inv->getItemStack(i);
			if (stack->item != nullptr) {
				if (stack->getItem()->itemId == 259) {
					g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot = i;
					return;
				}
			}
		}
	}
}
void Killaura::onSendPacket(C_Packet* packet) {
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

