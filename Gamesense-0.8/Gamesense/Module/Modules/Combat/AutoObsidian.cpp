#include "AutoObsidian.h"
bool self = false;
SettingEnum mode2;
AutoObsidian::AutoObsidian() : IModule(0x0, Category::COMBAT, "Automatically places obsidian if target is near to flatten arena") {
	registerFloatSetting("Range", &range, range, 3, 10);
	registerBoolSetting("Silent", &silentab, silentab);
	registerBoolSetting("Self", &self, self);
	registerEnumSetting("Mode", &mode2, 0);
	mode2.addEntry("Normal", 0);
	mode2.addEntry("BedPvP", 1);
}

const char* AutoObsidian::getRawModuleName() {
	return ("Flatten");
}
const char* AutoObsidian::getModName()
{
	if (silentab)
		name = " Silent";
	else
		name = " Normal";
	return name;
}
void AutoObsidian::findObsi() {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 49) {
				supplies->selectedHotbarSlot = n;
				break;
			}
		}
	}
	//supplies->selectedHotbarSlot = slot;
}


struct CompareTargetEnArray {
	bool operator()(C_Entity* lhs, C_Entity* rhs) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};


static std::vector<C_Entity*> targetListcc2;
void findTargetgg2(C_Entity* currentEntity, bool isRegularEntity) {

	if (!TargetUtil::isValidTarget(currentEntity)) return;

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
	if (dist < moduleMgr->getModule<AutoObsidian>()->range) {
		targetListcc2.push_back(currentEntity);
		sort(targetListcc2.begin(), targetListcc2.end(), CompareTargetEnArray());
	}
}

bool AutoObsidian::canpla(vec3_ti blockPos) {
	if (g_Data.getLocalPlayer()->getPos()->dist(blockPos.toVector3()) >= 7) {
		return false;
	}
	else return true;
}

bool AutoObsidian::tryAutoTrap2(vec3_ti blkPlacement) {
	//blkPlacement = blkPlacement.floor();

	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blkPlacement));
	C_BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->material->isReplaceable) {
		vec3_ti blok(blkPlacement);
		int i = 0;

		static std::vector<vec3_ti*> checklist;
		bool foundCandidate = false;
		if (checklist.empty()) {
			checklist.push_back(new vec3_ti(0, -1, 0));
			checklist.push_back(new vec3_ti(0, 1, 0));

			checklist.push_back(new vec3_ti(0, 0, -1));
			checklist.push_back(new vec3_ti(0, 0, 1));

			checklist.push_back(new vec3_ti(-1, 0, 0));
			checklist.push_back(new vec3_ti(1, 0, 0));
		}

		for (auto current : checklist) {
			vec3_ti calc = blok.sub(*current);
			bool Y = ((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable;
			if (!((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable) {
				// Found a solid block to click
				foundCandidate = true;
				blok = calc;
				break;
			}
			i++;
		}
		if (foundCandidate) {
			g_Data.getCGameMode()->buildBlock(&blok, i);
			return true;
		}
	}
	return false;
}
void AutoObsidian::onTick(C_GameMode* gm) {

	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	aovcl = false;
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	targetListcc2.clear();

	g_Data.forEachEntity(findTargetgg2);
	if (silentab) {
		aovcl = true;
		//slotab = supplies->selectedHotbarSlot;
		//findObsi();
	}
	if (g_Data.getLocalPlayer()->getSelectedItemId() == 49 || aovcl) {
		if (!self) {
			if (!targetListcc2.empty()) {
				for (auto& a : targetListcc2) {
					/*
					blockpos2 = targetListcc[0]->getPos()->floor();
					//blockpos2.x = (int)blockpos2.x - 1;
					blockpos2.y = (int)blockpos2.y - 1.6f;
					//blockpos2.z = (int)blockpos2.z - 1;
					*/
					vec3_ti targetPosi = targetListcc2[0]->getPos()->floor();
					for (int x = targetPosi.x - 2; x <= targetPosi.x + 2; x++) {
						for (int z = targetPosi.z - 2; z <= targetPosi.z + 2; z++) {
							if (silentab) {
								slotab = supplies->selectedHotbarSlot;
								findObsi();
							}
							//if (canpla(vec3_ti(x, (int)targetPosi.y - 1.6f, z))) {
							//	gm->buildBlock(&vec3_ti(x, (int)targetPosi.y - 1.6f, z), 0);
							//}
							if (mode2.getSelectedValue() == 0) {
								if (canpla(vec3_ti(x, (int)targetPosi.y - 1.6f, z))) {
									tryAutoTrap2(vec3_ti(x, (int)targetPosi.y - 1.6f, z));
								}
							}
							if (mode2.getSelectedValue() == 1) {
								if (canpla(vec3_ti(x, (int)targetPosi.y - 0.5f, z))) {
									tryAutoTrap2(vec3_ti(x, (int)targetPosi.y - 0.5, z));
								}
							}
							if (silentab) {
								supplies->selectedHotbarSlot = slotab;
							}
						}

					}
				}
			}
		}
		else if (self) {
			for (auto& a : targetListcc2) {
				/*
				blockpos2 = targetListcc[0]->getPos()->floor();
				//blockpos2.x = (int)blockpos2.x - 1;
				blockpos2.y = (int)blockpos2.y - 1.6f;
				//blockpos2.z = (int)blockpos2.z - 1;
				*/
				vec3_ti targetPosi = g_Data.getLocalPlayer()->getPos()->floor();
				for (int x = targetPosi.x - 2; x <= targetPosi.x + 2; x++) {
					for (int z = targetPosi.z - 2; z <= targetPosi.z + 2; z++) {
						if (silentab) {
							slotab = supplies->selectedHotbarSlot;
							findObsi();
						}
						if (mode2.getSelectedValue() == 0) {
							if (canpla(vec3_ti(x, (int)targetPosi.y - 1.6f, z))) {
								tryAutoTrap2(vec3_ti(x, (int)targetPosi.y - 1.6f, z));
							}
						}
						if (mode2.getSelectedValue() == 1) {
							if (canpla(vec3_ti(x, (int)targetPosi.y - 0.5f, z))) {
								tryAutoTrap2(vec3_ti(x, (int)targetPosi.y - 0.5, z));
							}
						}
						if (silentab) {
							supplies->selectedHotbarSlot = slotab;
						}
					}
				}
			}
		}
	}

}

void AutoObsidian::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
	/*
	if (GameData::canUseMoveKeys()) {
		if (!moduleMgr->getModule<ClickGUIMod>()->isEnabled()) {
			DrawUtils::setColor(1.f, 0.f, 0.f, 1.f);
			DrawUtils::drawBox(vec3_t(blockpos2.toVector3().floor()), vec3_t(blockpos2.toFloatVector().floor().add(1.f)), 0.5f, false);
		}
	}
	*/
}