#include "Killaura.h"
std::string targetNameka = "None";
float rotatespeed = 0.f;
int multiply = 1;
bool slowswing = false;
Killaura::Killaura() : IModule(0, Category::COMBAT, "Attacks entities around you automatically.") {
	registerIntSetting("HitAttempts", &delay, 20, 1, 100); //this is already hitattempts... ill just add a ka multiplier to act like multiple ka.
	registerIntSetting("Duplicate", &multiply, 1, 1, 5); //repeats entire ka function to deal with protohax nns
	registerFloatSetting("Range", &range, range, 2.f, 20.f);
	registerFloatSetting("rotation", &rotatespeed, rotatespeed, 0.f, 80.f);
	registerBoolSetting("MultiAura", &isMulti, isMulti);
	registerBoolSetting("MobAura", &isMobAura, isMobAura);
	registerBoolSetting("HurtTime", &hurttime, hurttime);
	registerBoolSetting("SlowSwingTime", &slowswing, slowswing);
	registerBoolSetting("Strafe", &rotations, rotations);
	registerBoolSetting("autoweapon", &autoweapon, autoweapon);
}

Killaura::~Killaura() {
}

const char* Killaura::getModuleName() {
	return ("Killaura");
}
const char* Killaura::getModName() {
	return targetNameka.c_str();
}

static std::vector<Entity*> targetList;
void findEntity(Entity* currentEntity, bool isRegularEntity) {
	static auto killauraMod = moduleMgr->getModule<Killaura>();

	if (currentEntity == nullptr)
		return;
	
	if (currentEntity == Game.getLocalPlayer())  // Skip Local player
		return;

	if (!Game.getLocalPlayer()->canAttack(currentEntity, false))
		return;

	if (!Game.getLocalPlayer()->isAlive())
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
		if (currentEntity->getEntityTypeId() == 80)  // Arrows
			return;
		if (currentEntity->getEntityTypeId() == 51) // NPC
			return;
	} else {
		if (!Target::isValidTarget(currentEntity))
			return;
	}

	float dist = (*currentEntity->getPos()).dist(*Game.getLocalPlayer()->getPos());

	if (dist < killauraMod->range) {
		targetList.push_back(currentEntity);
	}
}

void Killaura::findWeapon() {
	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	Inventory* inv = supplies->inventory;
	float damage = 0;
	int slot = supplies->selectedHotbarSlot;
	for (int n = 0; n < 9; n++) {
		ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			float currentDamage = stack->getAttackingDamageWithEnchants();
			if (currentDamage > damage) {
				damage = currentDamage;
				slot = n;
			}
		}
	}
}
int findWeaponswitchhotbar() {
	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	Inventory* inv = supplies->inventory;
	float damage = 0;
	int slot = supplies->selectedHotbarSlot;
	for (int n = 0; n < 9; n++) {
		ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			float currentDamage = stack->getAttackingDamageWithEnchants();
			if (currentDamage > damage) {
				damage = currentDamage;
				slot = n;
			}
		}
	}
	return slot;
}

void Killaura::onTick(GameMode* gm) {
	for (int ie = 0; ie < multiply; ie++) {
		if (moduleMgr->getModule<Desync>()->isEnabled()) {
			auto player = Game.getLocalPlayer();
			if (player->getSelectedItemId() == 259 && (Game.isRightClickDown() || moduleMgr->getModule<PacketGapple>()->isEnabled() && Game.getLocalPlayer()->getHealth() < moduleMgr->getModule<PacketGapple>()->health))
				return;
			if (player->getSelectedItemId() == 300 && (Game.isRightClickDown() || moduleMgr->getModule<PacketGapple>()->isEnabled() && Game.getLocalPlayer()->getHealth() < moduleMgr->getModule<PacketGapple>()->health))
				return;
			if (player->getSelectedItemId() == 426 && (Game.isRightClickDown() || moduleMgr->getModule<PacketGapple>()->isEnabled() && Game.getLocalPlayer()->getHealth() < moduleMgr->getModule<PacketGapple>()->health))
				return;
		};
		targetListEmpty = targetList.empty();
		//Loop through all our players and retrieve their information
		targetList.clear();

		Game.forEachEntity(findEntity);
		if (!targetList.empty()) {
			targetNameka = std::string(targetList[0]->getNameTag()->getText());
			if (autoweapon) Game.getLocalPlayer()->switchHotbar(findWeaponswitchhotbar());

			if (Game.getLocalPlayer()->velocity.squaredxzlen() < 0.01) {
				C_MovePlayerPacket p(Game.getLocalPlayer(), *Game.getLocalPlayer()->getPos());
				Game.getClientInstance()->loopbackPacketSender->sendToServer(&p);  // make sure to update rotation if player is standing still
			}

			// Attack all entitys in targetList
			for (int i = 0; i < delay; i++) {
				if (isMulti) {
					for (auto& i : targetList) {
						if (!(i->damageTime > 0 && hurttime)) {
							Game.getGameMode()->attack(i);
						}
						if (!(i->damageTime > 0 && slowswing)) {
							Game.getLocalPlayer()->swing();
						}
					}
				}
				else {
					if (!(targetList[0]->damageTime > 0 && hurttime)) {
						Game.getGameMode()->attack(targetList[0]);
					}
					if (!(targetList[0]->damageTime > 0 && slowswing)) {
						Game.getLocalPlayer()->swing();
					}
				}
			};
			Vec2 angle = Game.getLocalPlayer()->getPos()->CalcAngle(*targetList[0]->getPos());
			if (moduleMgr->getModule<Silent>()->isEnabled() && autoweapon) {
				Game.getLocalPlayer()->getSupplies()->selectedHotbarSlot = findWeaponswitchhotbar();
			}
		}
		else {
			targetNameka = "None";
		}
	}
}
Vec2 anglevisual;
void Killaura::onPlayerTick(Player* plr) {
	if (!plr) return;
	auto player = Game.getLocalPlayer();
	if (!player) return;
	if (rotations && !targetList.empty()) {
		angle = player->getPos()->CalcAngle(*targetList[0]->getPos());
		anglevisual = player->getPos()->CalcAngle(*targetList[0]->getPos());
		GameSettingsInput* inputcc = Game.getClientInstance()->getGameSettingsInput();
		if (GameData::isKeyDown(*inputcc->leftKey)) {
			angle.y += rotatespeed - 40;
		}
		else if (GameData::isKeyDown(*inputcc->rightKey)) {
			angle.y -= rotatespeed - 40;
		}
		else {
			angle.y -= rotatespeed - 70;
		}
	}
	if (!targetList.empty()) {
		angle.y += rotatespeed / 4;
		player->setRot(angle);
		player->yawUnused1 = anglevisual.y;
		player->pitch = anglevisual.x;
		player->bodyYaw = anglevisual.y;
	}
}
void Killaura::onEnable() {
	if (Game.getLocalPlayer() == nullptr)
		setEnabled(false);
}

void Killaura::onJoiningServer() {
	targetList.clear();
	targetListEmpty = true;
}

void Killaura::onSendPacket(Packet* packet) {
	if (packet->isInstanceOf<C_MovePlayerPacket>() && Game.getLocalPlayer() != nullptr && !rotations) {
		if (!targetList.empty()) {
			auto* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
			Vec2 angle = Game.getLocalPlayer()->getPos()->CalcAngle(*targetList[0]->getPos());
			movePacket->pitch = angle.x;
			movePacket->headYaw = angle.y;
			movePacket->yaw = angle.y;
		}
	}
}