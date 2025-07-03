#include "CrysKa.h"
std::string targetNameka2 = "None";
int multiply23 = 1;
bool slowswing2 = false;
CrystalKa::CrystalKa() : IModule(0, Category::COMBAT, "Attacks entities around you automatically.") {
	registerIntSetting("HitAttempts", &delay, 5, 1, 5); //this is already hitattempts... ill just add a ka multiplier to act like multiple ka.
	registerIntSetting("Duplicate", &multiply23, 1, 1, 5); //repeats entire ka function to deal with protohax nns
	registerFloatSetting("Range", &range, range, 2.f, 20.f);
	registerBoolSetting("MultiAura", &isMulti, isMulti);
	registerBoolSetting("MobAura", &isMobAura, isMobAura);
	registerBoolSetting("HurtTime", &hurttime, hurttime);
	registerBoolSetting("SlowSwingTime", &slowswing2, slowswing2);
	registerBoolSetting("autoweapon", &autoweapon, autoweapon);
}

CrystalKa::~CrystalKa() {
}

const char* CrystalKa::getModuleName() {
	return ("CrystalKa");
}
const char* CrystalKa::getModName() {
	return ("");
}

static std::vector<Entity*> targetList;
void findEntity2(Entity* currentEntity, bool isRegularEntity) {
	static auto killauraMod = moduleMgr->getModule<CrystalKa>();

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

void CrystalKa::findWeapon() {
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
int findWeaponswitchhotbar2() {
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

void CrystalKa::onTick(GameMode* gm) {
	for (int ie = 0; ie < multiply23; ie++) {
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
		targetList.clear();
		Game.forEachEntity(findEntity2);
		if (!targetList.empty()) {
			targetNameka2 = std::string(targetList[0]->getNameTag()->getText());
			if (autoweapon) Game.getLocalPlayer()->switchHotbar(findWeaponswitchhotbar2());
			for (int i = 0; i < delay; i++) {
				static int tickCount = 0; 
				for (auto& i : targetList) {
					if (tickCount % 5 == 0) { 
						Game.getGameMode()->attack(i);
						if (!(i->damageTime > 0 && slowswing2)) {
							Game.getLocalPlayer()->swing();
						}
					}
				}
				tickCount++; 
			}
		}
	}
};
void CrystalKa::onPlayerTick(Player* plr) {
}
void CrystalKa::onEnable() {
	if (Game.getLocalPlayer() == nullptr)
		setEnabled(false);
}

void CrystalKa::onJoiningServer() {
	targetList.clear();
	targetListEmpty = true;
}

void CrystalKa::onSendPacket(Packet* packet) {
	if (packet->isInstanceOf<C_MovePlayerPacket>() && Game.getLocalPlayer() != nullptr) {
		if (!targetList.empty()) {
			auto* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
			Vec2 angle = Game.getLocalPlayer()->getPos()->CalcAngle(*targetList[0]->getPos());
			movePacket->pitch = angle.x;
			movePacket->headYaw = angle.y;
			movePacket->yaw = angle.y;
		}
	}
}