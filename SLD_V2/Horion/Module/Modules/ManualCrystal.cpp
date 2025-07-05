#include "ManualCrystal.h"
#include "../../../Utils/Utils.h"
#include "../../../Utils/Logger.h"
#include "../../../Utils/Target.h"
#include "../ModuleManager.h"

//  Yay my first module that is not a complete edit of other modules!

ManualCrystal::ManualCrystal() : IModule(0, Category::COMBAT, "Hold right click to place and destroy crystals!") {
	registerBoolSetting("Switch to crystals", &this->switchToCrystal, this->switchToCrystal);
	registerBoolSetting("testspoof", &this->spoof, this->spoof);
	registerIntSetting("Delay (ticks)", &this->delay, this->delay, 0, 25);
	origSlot;
}

ManualCrystal::~ManualCrystal(){};

const char* ManualCrystal::getModuleName() {
	return ("ManualCrystal");
}

void ManualCrystal::onEnable() {

	if (g_Data.getLocalPlayer() == nullptr) return;  // maybe putting this on onEnable() may help too?

	if (switchToCrystal) {
		auto supplies = g_Data.getLocalPlayer()->getSupplies();
		origSlot = supplies->selectedHotbarSlot;
		auto FinishSelect = true;
		auto inv = supplies->inventory; // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
		for (int n = 0; n < 9; n++) {
			C_ItemStack* stack = inv->getItemStack(n);
			if (stack->item != nullptr) {
				if (stack->getItem()->itemId == 637) {  // select end crystals
					if (origSlot != n)
						supplies->selectedHotbarSlot = n;
					return;
				}
			}
		}
	}
}

void ManualCrystal::onDisable() {
	if (switchToCrystal) {
		auto supplies = g_Data.getLocalPlayer()->getSupplies();
		supplies->selectedHotbarSlot = origSlot;
	}
}

void noCrystalInteract() {
	Level* pstruct = g_Data.getLocalPlayer()->getlevel();            // jpix should really be doing this w/ me if he wants to learn how to make a CA...
	g_Data.getCGameMode()->buildBlock(new vec3_ti(pstruct->block), pstruct->blockSide, true);  // add crystal

	g_Data.forEachEntity([](C_Entity* ent, bool b) {  // this code really looks weird
		int id = ent->getEntityTypeId();
		if (id == 71) {  // kill crystal
			g_Data.getCGameMode()->attack(ent);
		}
	});
}

bool getCrystalSpoofMC() {
	__int64 id = *g_Data.getLocalPlayer()->getUniqueId();
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if ((*stack->item)->itemId == 637) {
				C_MobEquipmentPacket a(id, *stack, n, n);
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
				return true;
			}
		}
	}
	C_MobEquipmentPacket a(id, *g_Data.getLocalPlayer()->getSelectedItem(), supplies->selectedHotbarSlot, supplies->selectedHotbarSlot);
	g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
	return false;
}

void stopSpoofingNigger() {
	__int64 id = *g_Data.getLocalPlayer()->getUniqueId();
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	C_MobEquipmentPacket a(id, *g_Data.getLocalPlayer()->getSelectedItem(), supplies->selectedHotbarSlot, supplies->selectedHotbarSlot);
	g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
	return;
}

void ManualCrystal::onTick(C_GameMode* gm) {
	ctr++;

	if (g_Data.getLocalPlayer() == nullptr) return;  // ALWAYS HAVE THIS IN (on onTick() funcs), OR ELSE MC WILL CRASH IF NOT IN-GAME

	if (ctr >= delay) {
		if ((not spoof && (g_Data.getLocalPlayer()->getSelectedItemId() == 637)) && g_Data.isRightClickDown()) {
			auto ptr = g_Data.getLocalPlayer()->getlevel();

			if (ptr != nullptr && ptr->rayHitType == 0) {

				if (g_Data.getLocalPlayer()->region->getBlock(ptr->block)->toLegacy()->blockId == 49 
				|| g_Data.getLocalPlayer()->region->getBlock(ptr->block)->toLegacy()->blockId == 7) {
					noCrystalInteract();  // efficient coding...so much if statements :(
				}
			}
			if (ptr->hasEntity()) {
				if (ptr->getEntity()->getEntityTypeId() == 71) {
					g_Data.forEachEntity([](C_Entity* ent, bool b) {
						int id = ent->getEntityTypeId();
						if (id == 71) {  // kill crystal
							g_Data.getCGameMode()->attack(ent);
							//g_Data.getLocalPlayer()->swingArm();
						}
					});
				
				}
			}
		} else if (spoof) {
			auto ptr = g_Data.getLocalPlayer()->getlevel();

			if (ptr != nullptr && ptr->rayHitType == 0) {
				if (g_Data.getLocalPlayer()->region->getBlock(ptr->block)->toLegacy()->blockId == 49 || g_Data.getLocalPlayer()->region->getBlock(ptr->block)->toLegacy()->blockId == 7) {
					getCrystalSpoofMC();
					noCrystalInteract();  // efficient coding...so much if statements :(
					stopSpoofingNigger();
				}
			}
			if (ptr->hasEntity()) {
				if (ptr->getEntity()->getEntityTypeId() == 71) {
					g_Data.forEachEntity([](C_Entity* ent, bool b) {
						int id = ent->getEntityTypeId();
						if (id == 71) {  // kill crystal
							g_Data.getCGameMode()->attack(ent);
							//g_Data.getLocalPlayer()->swingArm();
						}
					});
				}
			}
		}
		ctr = 0;
	}
}

void ManualCrystal::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (g_Data.getLocalPlayer() == nullptr) return;
	if (g_Data.getLocalPlayer()->getSelectedItemId() != 637) return;

	auto ptr = g_Data.getLocalPlayer()->getlevel();
	if (ptr != nullptr) {
		if (ptr->getEntity() == nullptr && ptr->rayHitType == 0) {
			if (g_Data.getLocalPlayer()->region->getBlock(ptr->block)->toLegacy()->blockId == 49 ||
				g_Data.getLocalPlayer()->region->getBlock(ptr->block)->toLegacy()->blockId == 7) {
				DrawUtils::setColor(.02f, 1.f, .75f, 1.f);
				DrawUtils::drawBox(ptr->block.toVec3t().add(0.f, 1.5f, 0.f),
								   ptr->block.add(1).toVec3t().add(0.f, 1.5f, 0.f), .3f);
			}
		}
		if (ptr->getEntity() != nullptr && ptr->hasEntity()) {
			if (ptr->getEntity()->getEntityTypeId() == 71) {
				DrawUtils::setColor(.02f, 1.f, .75f, 1.f);
				DrawUtils::drawEntityBox(ptr->getEntity(), .3f);
			}
		}
	}
}