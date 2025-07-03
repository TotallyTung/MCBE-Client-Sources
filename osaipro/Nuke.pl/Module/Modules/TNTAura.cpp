#include "../../../SDK/Tag.h"

int tickIgnite = 0;
float tickBlockPlace = 0.f;
float placedelay_obsidian = 0.3f;
bool dod = true;
#include "TNTAura.h"
bool pm2 = true;
TNTAura::TNTAura() : IModule('f', Category::COMBAT, "Spams TNTs in peoples heads") {
	registerBoolSetting("Priority Mine", &pm2, true);
	ObsidianMode.addEntry("Normal", Normal);
	ObsidianMode.addEntry("AntiThroughWall", AntiThroughWall);
	ObsidianMode.addEntry("OnlyPillar", OnlyPillar);
	ObsidianMode.addEntry("AntiCollapse", AntiCollapse);
	ObsidianMode.addEntry("Only TNT", OnlyGravity);
	registerEnumSetting("Obsidian Mode", &ObsidianMode, Normal);
	registerKeybindSetting("ModeChangeKey", &keys, keys);
	registerFloatSetting("Place Delay", &placedelay_obsidian, placedelay_obsidian, 0.f, 0.1f);
	this->registerIntSetting("Range", &this->range, this->range, 3, 10);
	this->registerIntSetting("Ignite delay", &this->ignitedelay, 0, 0, 3);
	registerBoolSetting("self", &Self, false);
	registerBoolSetting("Disable on Died", &dod, true);
}
TNTAura::~TNTAura() {
}
const char* TNTAura::getModuleName() {
	return ("TNTAura");
}
std::vector<Entity*> targetListTNT;
const char* TNTAura::getModName() {
	std::string Targets = "None";
	if (targetListTNT.empty()) {
		Targets = (" [ None ]");
	}
	else {
		Targets = (" [ " + std::string(targetListTNT[0]->getNameTag()->getText()) + " ]");
	}
	if (ObsidianMode.getSelectedValue() == Normal) {
		return std::string("Normal" + Targets).c_str();
	}
	else if (ObsidianMode.getSelectedValue() == 1) {
		return std::string("AntiThroughWall" + Targets).c_str();
	}
	else if (ObsidianMode.getSelectedValue() == 2) {
		return std::string("OnlyPillar" + Targets).c_str();
	}
	else if (ObsidianMode.getSelectedValue() == 3) {
		return std::string("AntiCollapse" + Targets).c_str();
	}
	else if (ObsidianMode.getSelectedValue() == 4) {
		return std::string("Only TNT" + Targets).c_str();
	};
	return std::string("None" + Targets).c_str();
}
void TNTAura::onKeyUpdate(int keyb, bool isDown) {
	if (!Game.canUseMoveKeys()) return;
	if (!isDown) return;
	if (keyb == this->getKeybind()) {
		this->toggle();
	}
	if (!this->isEnabled()) return;
	if (Game.getLocalPlayer() == nullptr) return;
	if (keyb == keys) {
		if (     ObsidianMode.getSelectedValue() == Normal) {
			ObsidianMode.selected = AntiThroughWall;
			Notifications::addNotifBox("Changed TNTAura Obsidian Mode! {AntiThroughWall}", 5.f);
		}
		else if (ObsidianMode.getSelectedValue() == AntiThroughWall) {
			ObsidianMode.selected = OnlyPillar;
			Notifications::addNotifBox("Changed TNTAura Obsidian Mode! {OnlyPillar}", 5.f);
		}
		else if (ObsidianMode.getSelectedValue() == OnlyPillar) {
			ObsidianMode.selected = AntiCollapse;
			Notifications::addNotifBox("Changed TNTAura Obsidian Mode! {AntiCollapse}", 5.f);
		}
		else if (ObsidianMode.getSelectedValue() == AntiCollapse) {
			ObsidianMode.selected = OnlyGravity;
			Notifications::addNotifBox("Changed TNTAura Obsidian Mode! {OnlyTNT}", 5.f);
		}
		else if (ObsidianMode.getSelectedValue() == OnlyGravity) {
			ObsidianMode.selected = Normal;
			Notifications::addNotifBox("Changed TNTAura Obsidian Mode! {Normal}", 5.f);
		}
	}
};
bool TNTAura::findEntByC(Entity* curEnt, bool isRegularEntity) {
	if (!moduleMgr->getModule<TNTAura>()->Self) {
		if (curEnt == nullptr) return false;
		if (!Game.getLocalPlayer()->canAttack(curEnt, false)) return false;
		if (!Game.getLocalPlayer()->isAlive()) return false;
		if (!curEnt->isAlive()) return false;
		if (curEnt->getEntityTypeId() != 319) return false;
		if (curEnt->getNameTag()->getTextLength() <= 1) return false;
		if (curEnt->isLocalPlayer()) return false;
		std::string invalids[6] = {
		"-", ".", "/",
		"+", ",", "#"
		};
		for (auto invalid : invalids) {
			size_t invalidChar = std::string(curEnt->getNameTag()->getText()).find(invalid);
			if (invalidChar != std::string::npos) return false;
		}
		if (FriendsManager::findFriend(curEnt->getNameTag()->getText())) return false;
		float dist = (*curEnt->getPos()).dist(*Game.getLocalPlayer()->getPos());
		if (dist <= moduleMgr->getModule<TNTAura>()->range) {
			targetListTNT.push_back(curEnt);
			return true;
		}
	}
	else {
		targetListTNT.push_back(Game.getLocalPlayer());
		return true;
	}
	return false;
}

bool placeBloKKK(Vec3 blkPlacement) {
	blkPlacement = blkPlacement.floor();

	Block* block = Game.getLocalPlayer()->region->getBlock(Vec3i(blkPlacement));
	BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->material->isReplaceable) {
		Vec3i blok(blkPlacement);

		// Find neighbour
		static std::vector<Vec3i*> checklist;
		if (checklist.empty()) {
			checklist.push_back(new Vec3i(0, -1, 0));
			checklist.push_back(new Vec3i(0, 1, 0));

			checklist.push_back(new Vec3i(0, 0, -1));
			checklist.push_back(new Vec3i(0, 0, 1));

			checklist.push_back(new Vec3i(-1, 0, 0));
			checklist.push_back(new Vec3i(1, 0, 0));
		}

		bool foundCandidate = false;
		int i = 0;
		for (auto current : checklist) {
			Vec3i calc = blok.sub(*current);
			bool Y = ((Game.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable;
			if (!((Game.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable) {
				// Found a solid block to click
				foundCandidate = true;
				blok = calc;
				break;
			}
			i++;
		}
		if (foundCandidate) {
			Game.getGameMode()->buildBlock(&blok, i, true);

			return true;
		}
	}
	return false;
}
int getBlockID(Vec3 Pos) {
	return Game.getLocalPlayer()->region->getBlock(Vec3i(Pos))->blockLegacy->blockId;
};
bool foundblock = false;
float waitsec2 = 0.f;
void TNTAura::onTick(GameMode* gm) {
	targetListTNT.clear();
	obs.clear();
	if (Game.getLocalPlayer() == nullptr) return;
	if (moduleMgr->AutoDesync()) return;
	Game.forEachEntity(findEntByC);
	auto supplies = Game.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;
	int slotab = supplies->selectedHotbarSlot;
	tickIgnite++;
	for (int n = 0; n < 10; n++) tickBlockPlace += 0.1f;
	if (dod && !Game.getLocalPlayer()->isAlive()) this->setEnabled(false);
	if (!targetListTNT.empty()) {
		for (Entity* tgt : targetListTNT) {
			if (!tgt->isAlive()) return;

			Vec3 Base = tgt->getPos()->floor();
			Vec3 PlayerAbove = Vec3(Base.x, Base.y + 1, Base.z);

			Vec3 PlayerAboveSide1 = Vec3(Base.x + 1, Base.y + 1, Base.z);
			Vec3 PlayerAboveSide2 = Vec3(Base.x - 1, Base.y + 1, Base.z);
			Vec3 PlayerAboveSide3 = Vec3(Base.x, Base.y + 1, Base.z + 1);
			Vec3 PlayerAboveSide4 = Vec3(Base.x, Base.y + 1, Base.z - 1);
			if (pm2) {
				if (moduleMgr->getModule<PacketMine>()->blockPos != Vec3(0, 0, 0)) {
					if (moduleMgr->getModule<PacketMine>()->blockPos == PlayerAbove) {
						if (getBlockID(Vec3(moduleMgr->getModule<PacketMine>()->blockPos.x, moduleMgr->getModule<PacketMine>()->blockPos.y, moduleMgr->getModule<PacketMine>()->blockPos.z)) != (46)) waitsec2 = 0.5f;
					}
				}
				if (waitsec2 >= 0.1f) {
					waitsec2 -= 0.1f;
					return;
				}
			}
			PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
			Inventory* inv = supplies->inventory;
			slotab = supplies->selectedHotbarSlot;
			if (ObsidianMode.getSelectedValue() == Normal) {
				obs.push_back(Vec3(Base.x, Base.y - 2, Base.z - 1));
				obs.push_back(Vec3(Base.x, Base.y - 2, Base.z + 1));
				obs.push_back(Vec3(Base.x - 1, Base.y - 2, Base.z));
				obs.push_back(Vec3(Base.x + 1, Base.y - 2, Base.z));

				obs.push_back(Vec3(Base.x, Base.y - 1, Base.z - 1));
				obs.push_back(Vec3(Base.x, Base.y - 1, Base.z + 1));
				obs.push_back(Vec3(Base.x - 1, Base.y - 1, Base.z));
				obs.push_back(Vec3(Base.x + 1, Base.y - 1, Base.z));

				obs.push_back(Vec3(Base.x, Base.y, Base.z - 1));
				obs.push_back(Vec3(Base.x, Base.y, Base.z + 1));
				obs.push_back(Vec3(Base.x - 1, Base.y, Base.z));
				obs.push_back(Vec3(Base.x + 1, Base.y, Base.z));

				obs.push_back(Vec3(Base.x, Base.y, Base.z - 1));
				obs.push_back(Vec3(Base.x, Base.y, Base.z + 1));
				obs.push_back(Vec3(Base.x - 1, Base.y, Base.z));
				obs.push_back(Vec3(Base.x + 1, Base.y, Base.z));

				obs.push_back(Vec3(Base.x - 1, Base.y + 1, Base.z));
				obs.push_back(Vec3(Base.x + 1, Base.y + 1, Base.z));
				obs.push_back(Vec3(Base.x, Base.y + 1, Base.z + 1));
				obs.push_back(Vec3(Base.x, Base.y + 1, Base.z - 1));

				obs.push_back(Vec3(Base.x, Base.y + 2, Base.z + 1));
				obs.push_back(Vec3(Base.x, Base.y + 2, Base.z));
			}
			if (ObsidianMode.getSelectedValue() == AntiThroughWall) {
				obs.push_back(Vec3(Base.x, Base.y - 2, Base.z - 1));
				obs.push_back(Vec3(Base.x, Base.y - 2, Base.z + 1));
				obs.push_back(Vec3(Base.x - 1, Base.y - 2, Base.z));
				obs.push_back(Vec3(Base.x + 1, Base.y - 2, Base.z));

				obs.push_back(Vec3(Base.x, Base.y - 1, Base.z - 1));
				obs.push_back(Vec3(Base.x, Base.y - 1, Base.z + 1));
				obs.push_back(Vec3(Base.x - 1, Base.y - 1, Base.z));
				obs.push_back(Vec3(Base.x + 1, Base.y - 1, Base.z));

				obs.push_back(Vec3(Base.x, Base.y, Base.z - 1));
				obs.push_back(Vec3(Base.x, Base.y, Base.z + 1));
				obs.push_back(Vec3(Base.x - 1, Base.y, Base.z));
				obs.push_back(Vec3(Base.x + 1, Base.y, Base.z));

				obs.push_back(Vec3(Base.x - 1, Base.y, Base.z - 1));
				obs.push_back(Vec3(Base.x + 1, Base.y, Base.z + 1));
				obs.push_back(Vec3(Base.x - 1, Base.y, Base.z + 1));
				obs.push_back(Vec3(Base.x + 1, Base.y, Base.z - 1));

				obs.push_back(Vec3(Base.x, Base.y, Base.z - 1));
				obs.push_back(Vec3(Base.x, Base.y, Base.z + 1));
				obs.push_back(Vec3(Base.x - 1, Base.y, Base.z));
				obs.push_back(Vec3(Base.x + 1, Base.y, Base.z));

				obs.push_back(Vec3(Base.x - 1, Base.y + 1, Base.z));
				obs.push_back(Vec3(Base.x + 1, Base.y + 1, Base.z));
				obs.push_back(Vec3(Base.x, Base.y + 1, Base.z + 1));
				obs.push_back(Vec3(Base.x, Base.y + 1, Base.z - 1));

				obs.push_back(Vec3(Base.x, Base.y + 1, Base.z - 1));
				obs.push_back(Vec3(Base.x, Base.y + 1, Base.z + 1));
				obs.push_back(Vec3(Base.x, Base.y + 2, Base.z));
			}
			if (ObsidianMode.getSelectedValue() == OnlyPillar) {
				obs.push_back(Vec3(Base.x, Base.y - 2, Base.z + 1));
				obs.push_back(Vec3(Base.x, Base.y - 1, Base.z + 1));
				obs.push_back(Vec3(Base.x, Base.y, Base.z + 1));
				obs.push_back(Vec3(Base.x, Base.y + 1, Base.z + 1));
			}
			if (ObsidianMode.getSelectedValue() == AntiCollapse) {
				obs.push_back(Vec3(Base.x, Base.y - 2, Base.z - 1));
				obs.push_back(Vec3(Base.x, Base.y - 2, Base.z + 1));
				obs.push_back(Vec3(Base.x - 1, Base.y - 2, Base.z));
				obs.push_back(Vec3(Base.x + 1, Base.y - 2, Base.z));

				obs.push_back(Vec3(Base.x, Base.y - 1, Base.z - 1));
				obs.push_back(Vec3(Base.x, Base.y - 1, Base.z + 1));
				obs.push_back(Vec3(Base.x - 1, Base.y - 1, Base.z));
				obs.push_back(Vec3(Base.x + 1, Base.y - 1, Base.z));

				obs.push_back(Vec3(Base.x, Base.y, Base.z - 1));
				obs.push_back(Vec3(Base.x, Base.y, Base.z + 1));
				obs.push_back(Vec3(Base.x - 1, Base.y, Base.z));
				obs.push_back(Vec3(Base.x + 1, Base.y, Base.z));

				obs.push_back(Vec3(Base.x - 1, Base.y, Base.z - 1));
				obs.push_back(Vec3(Base.x + 1, Base.y, Base.z + 1));
				obs.push_back(Vec3(Base.x - 1, Base.y, Base.z + 1));
				obs.push_back(Vec3(Base.x + 1, Base.y, Base.z - 1));

				obs.push_back(Vec3(Base.x, Base.y, Base.z - 1));
				obs.push_back(Vec3(Base.x, Base.y, Base.z + 1));
				obs.push_back(Vec3(Base.x - 1, Base.y, Base.z));
				obs.push_back(Vec3(Base.x + 1, Base.y, Base.z));

				obs.push_back(Vec3(Base.x - 1, Base.y + 1, Base.z));
				obs.push_back(Vec3(Base.x + 1, Base.y + 1, Base.z));
				obs.push_back(Vec3(Base.x, Base.y + 1, Base.z + 1));
				obs.push_back(Vec3(Base.x, Base.y + 1, Base.z - 1));

				obs.push_back(Vec3(Base.x - 1, Base.y + 2, Base.z));
				obs.push_back(Vec3(Base.x + 1, Base.y + 2, Base.z));
				obs.push_back(Vec3(Base.x, Base.y + 2, Base.z + 1));
				obs.push_back(Vec3(Base.x, Base.y + 2, Base.z - 1));

				obs.push_back(Vec3(Base.x - 1, Base.y + 2, Base.z - 1));
				obs.push_back(Vec3(Base.x + 1, Base.y + 2, Base.z + 1));
				obs.push_back(Vec3(Base.x - 1, Base.y + 2, Base.z + 1));
				obs.push_back(Vec3(Base.x + 1, Base.y + 2, Base.z - 1));

				obs.push_back(Vec3(Base.x, Base.y + 3, Base.z - 1));
				obs.push_back(Vec3(Base.x, Base.y + 3, Base.z + 1));
				obs.push_back(Vec3(Base.x + 1, Base.y + 3, Base.z));
				obs.push_back(Vec3(Base.x - 1, Base.y + 3, Base.z));
				obs.push_back(Vec3(Base.x, Base.y + 3, Base.z));
			}
			for (Vec3 PlacePosition : obs) {
				if (tickBlockPlace >= placedelay_obsidian) {
					if (getBlockID(PlacePosition) == 0) {
						for (int n = 0; n < 36; n++) {
							ItemStack* stack = inv->getItemStack(n);
							if (stack->item != nullptr) {
								if (stack->getItem()->itemId == 49) {
									supplies->selectedHotbarSlot = n;
									break;
								}
							}
						}
						placeBloKKK(PlacePosition);
						if (moduleMgr->getModule<Silent>()->isEnabled())
							supplies->selectedHotbarSlot = slotab;
						tickBlockPlace = 0;
					};
				};
			};
			if ((getBlockID(PlayerAboveSide1) != 0) || (getBlockID(PlayerAboveSide2) != 0) || (getBlockID(PlayerAboveSide3) != 0) || (getBlockID(PlayerAboveSide4) != 0))
				foundblock = true;
			if (getBlockID(PlayerAboveSide1) == 0 && getBlockID(PlayerAboveSide2) == 0 && getBlockID(PlayerAboveSide3) == 0 && getBlockID(PlayerAboveSide4) == 0)
				foundblock = false;
			if (foundblock && getBlockID(PlayerAbove) == 0) {
				for (int n = 0; n < 36; n++) {
					ItemStack* stack = inv->getItemStack(n);
					if (stack->item != nullptr) {
						if (stack->getItem()->itemId == 46) {
							supplies->selectedHotbarSlot = n;
							break;
						};
					};
				};
				placeBloKKK(Vec3(PlayerAbove));
				if (moduleMgr->getModule<Silent>()->isEnabled())
					supplies->selectedHotbarSlot = slotab;
			};
			if (getBlockID(PlayerAbove) == 46) {
				for (int n = 0; n < 36; n++) {
					ItemStack* stack = inv->getItemStack(n);
					if (stack->item != nullptr) {
						if (stack->getItem()->itemId == 316) {
							supplies->selectedHotbarSlot = n;
							break;
						};
					};
				};
				if (tickIgnite >= ignitedelay) gm->buildBlock(&Vec3i(PlayerAbove), 0, true);
				if (moduleMgr->getModule<Silent>()->isEnabled())
					supplies->selectedHotbarSlot = slotab;
			};
			tickIgnite = 0;
		};
	};
};

void TNTAura::onJoiningServer() {
	targetListTNT.clear();
	moduleMgr->getModule<TNTAura>()->obs.clear();
}