#include "AutoHoleFill.h"
float entRange = 5.f;
int wait = 5;
bool robot = false;
bool turnOnReady;
bool finW;
int wDel = 0;
int hDel = 0;
int hold = 5;
bool shouldRobot = false;
bool smart = false;
AutoHoleFill::AutoHoleFill() : IModule(0x0, Category::PLAYER, "Automatically fills holes") {
	registerBoolSetting("Obsidian", &obsidian, obsidian, "Do obsidian holes");
	registerBoolSetting("Bedrock", &bedrock, bedrock, "Do bedrock holes");
	registerBoolSetting("Robot", &robot, robot, "Activate only when target leaves a hole");
	registerBoolSetting("Smart", &smart, smart, "Activate when a target is nearby");
	registerIntSetting("Range", &range, range, 0, 10, "Range of the hole");
	registerIntSetting("BPS", &blockPerSec, blockPerSec, 0, 20, "How many blocks should we try to place per second");
	registerIntSetting("Wall Range", &wallRange, wallRange, 0, 10, "Range when in wall");
	registerFloatSetting("Target Range", &entRange, entRange, 0.f, 10.f, 0.01f, "Range of target");
	registerIntSetting("Wait Time", &wait, wait, 0, 20, "How long should target be out of hole");
    swap = SettingEnum(this)
					 .addEntry(EnumEntry("None", 0))
					 .addEntry(EnumEntry("Regular", 1))
					 .addEntry(EnumEntry("NukkitSpoof", 2));
	registerEnumSetting("Swap", &swap, 0, "Switch to obsidian");
}

AutoHoleFill::~AutoHoleFill() {
}

const char* AutoHoleFill::getRawModuleName() {
	return ("AutoHoleFill");
}

void findObsidian() {
	C_PlayerInventoryProxy *supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory *inv = supplies->inventory;
	for (int n = 0; n < 9; n++)
	{
		C_ItemStack *stack = inv->getItemStack(n);
		if (stack->item != nullptr)
		{
			if (stack->getItem()->itemId == 49)
			{
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}

bool tryPlaceHF(vec3_t blkPlacement) {
	blkPlacement = blkPlacement.floor();

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

void AutoHoleFill::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr) return;
	turnOnReady = false;
	finW = false;
	hasStarted = true;
	del0 = 0;

	origSlot = g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot;
	if (swap.getSelectedValue() == 1) {
		findObsidian();
		return;
	}
	
}
bool getSurroundStatusOfPlayer(C_Entity* nigger) {
	vec3_ti loc = nigger->getHumanPos().floor();

	const vec3_ti sides[4] = {
		loc.add(1, 0, 0),
		loc.sub(1, 0, 0),
		loc.sub(0, 0, 1),
		loc.add(0, 0, 1) };

	std::vector<vec3_ti> surrounded;

	for (vec3_ti side : sides) {
		C_Block* block = g_Data.getLocalPlayer()->region->getBlock(side);
		C_BlockLegacy* blockLegacy = (block->blockLegacy);
		if (blockLegacy->blockId == 0) { // check for exposure
			return false;
		}
	}

	return true;
}
static std::vector<C_Entity*> ppl_PP;
bool pplFinder600(C_Entity* curEnt, bool isRegularEntity) {
	if (curEnt == nullptr) return false;
	if (curEnt == g_Data.getLocalPlayer()) return false;  // Skip Local player
	//if (getSurroundStatusOfPlayer(curEnt)) return false;  // make sure that entity is already surrounded in the first place
	if (!curEnt->isAlive()) return false;
	if (!g_Data.getLocalPlayer()->isAlive()) return false;
	if (curEnt->getEntityTypeId() == 71) return false;                    // endcrystal
	if (curEnt->getEntityTypeId() == 66) return false;                    // falling block
	if (curEnt->getEntityTypeId() == 64) return false;                    // item
	if (curEnt->getEntityTypeId() == 69) return false;                    // xp orb
	if (curEnt->width <= 0.01f || curEnt->height <= 0.01f) return false;  // Don't hit this pesky antibot on 2b2e.org
	if (!TargetUtil::isValidTarget(curEnt)) return false;

	float dist = (curEnt->getHumanPos()).dist(g_Data.getLocalPlayer()->getHumanPos());
	if (dist <= entRange) {
		ppl_PP.push_back(curEnt);
		return true;
	}
	return false;
}
void AutoHoleFill::onDisable() {
	if (g_Data.getLocalPlayer() == nullptr) return;
	hasStarted = false;

	if (swap.getSelectedValue() == 1) {
		g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot = origSlot;
		return;
	}
}
void AutoHoleFill::onWorldTick(C_GameMode* gm) {
	ppl_PP.clear();
	//targs.clear();

	if (g_Data.getLocalPlayer() == nullptr) return;

	g_Data.forEachEntity(pplFinder600);


	for (auto& person : ppl_PP) {
		if (!getSurroundStatusOfPlayer(person)) {  // nigger finally came out of the hole
			turnOnReady = true;
			targs.clear();
		}
	}
}
void AutoHoleFill::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr) return;
	auto antiDesync = moduleMgr->getModule<AntiDesync>();
	if (antiDesync->mode.getSelectedValue() == 0 && antiDesync->isEnabled())
	{
		auto player = g_Data.getLocalPlayer();
		if (player->getSelectedItemId() == 259 && g_Data.isRightClickDown())
			return;
		if (player->getSelectedItemId() == 300 && g_Data.isRightClickDown())
			return;
		if (player->getSelectedItemId() == 426 && g_Data.isRightClickDown())
			return;
	}
	attemptPitchUp = false;
	if (turnOnReady && robot) {
		if (wDel < wait && !finW) {
			wDel++;
			return;
		}
		wDel = 0;
		finW = true;

		// enable holefiller
		shouldRobot = true;

		if (hDel < hold) {
			hDel++;
			return;
		}
		hDel = 0;
		finW = false;

		turnOnReady = false;
		if (!turnOnReady)
        return;

		if (disableOnStop)
			return;
	}
	if (!shouldRobot && robot)
		return;
	if (ppl_PP.empty() && smart)
		return;
	if (!(g_Data.getLocalPlayer() == nullptr || !GameData::canUseMoveKeys())) {
		if (hasStarted) {
			hasStarted = false;
			return;
		}
		int maxT = 20 / blockPerSec;

		if (del0 < maxT) {
			del0++;
			return;
		} else
			del0 = 0;

		if (g_Data.isInGame()) {
			vec3_t* pos = g_Data.getLocalPlayer()->getPos();
			for (int x = (int)pos->x - range; x < pos->x + range; x++) {
				for (int z = (int)pos->z - range; z < pos->z + range; z++) {
					for (int y = (int)pos->y - range; y < pos->y + range; y++) {
						vec4_t underline;
						vec3_t blockPos = vec3_t(x, y, z);
						vec3_t air = vec3_t(x, y + 1, z);
						vec3_t air2 = vec3_t(x, y + 2, z);

						vec3_t blockPos2 = vec3_t(x + 1, y, z);
						vec3_t blockPos3 = vec3_t(x, y, z + 1);
						vec3_t blockPos4 = vec3_t(x, y - 1, z);
						vec3_t blockPos5 = vec3_t(x - 1, y, z);
						vec3_t blockPos6 = vec3_t(x, y, z - 1);

						vec3_t blockPos22 = vec3_t(x + 1, y + 1, z);
						vec3_t blockPos32 = vec3_t(x, y + 1, z + 1);
						vec3_t blockPos52 = vec3_t(x - 1, y + 1, z);
						vec3_t blockPos62 = vec3_t(x, y + 1, z - 1);

						C_Block* block = g_Data.getLocalPlayer()->region->getBlock(blockPos);
						C_Block* airpos = g_Data.getLocalPlayer()->region->getBlock(air);
						C_Block* airpos2 = g_Data.getLocalPlayer()->region->getBlock(air2);
						C_Block* block2 = g_Data.getLocalPlayer()->region->getBlock(blockPos2);
						C_Block* block3 = g_Data.getLocalPlayer()->region->getBlock(blockPos3);
						C_Block* block4 = g_Data.getLocalPlayer()->region->getBlock(blockPos4);
						C_Block* block5 = g_Data.getLocalPlayer()->region->getBlock(blockPos5);
						C_Block* block6 = g_Data.getLocalPlayer()->region->getBlock(blockPos6);

						C_Block* block22 = g_Data.getLocalPlayer()->region->getBlock(blockPos22);
						C_Block* block32 = g_Data.getLocalPlayer()->region->getBlock(blockPos32);
						C_Block* block52 = g_Data.getLocalPlayer()->region->getBlock(blockPos52);
						C_Block* block62 = g_Data.getLocalPlayer()->region->getBlock(blockPos62);

						if (block != nullptr && block2 != nullptr && block3 != nullptr && block4 != nullptr) {
                            auto supplies = g_Data.getLocalPlayer()->getSupplies();
				            auto inv = supplies->inventory;
				            slotHF = supplies->selectedHotbarSlot;

							int blockId = block->toLegacy()->blockId;
							int aircheck = airpos->toLegacy()->blockId;
							int aircheck2 = airpos2->toLegacy()->blockId;
							int blockId2 = block2->toLegacy()->blockId;
							int blockId3 = block3->toLegacy()->blockId;
							int blockId4 = block4->toLegacy()->blockId;
							int blockId5 = block5->toLegacy()->blockId;
							int blockId6 = block6->toLegacy()->blockId;

							int blockId22 = block22->toLegacy()->blockId;
							int blockId32 = block32->toLegacy()->blockId;
							int blockId52 = block52->toLegacy()->blockId;
							int blockId62 = block62->toLegacy()->blockId;

							bool obs = false;
                            if (swap.getSelectedValue() == 2)
						        findObsidian();
							vec3_ti plugHole = blockPos;
							if (blockId == 0) obs = true;

							if (doPitchUp)
								attemptPitchUp = true;

							if (bedrock) {
								if ((blockId2 == 7 && blockId3 == 7 && blockId4 == 7 && blockId5 == 7 && blockId6 == 7 && blockId == 0) && (aircheck == 0)  && (countBlksAlongLine(*pos , plugHole.toVector3()).blockCount <= wallRange)) {
									tryPlaceHF(plugHole.toVector3());
								}
							}
							if (obsidian) {
								if ((blockId2 == 49 || blockId2 == 7) && (blockId3 == 49 || blockId3 == 7) && (blockId4 == 49 || blockId4 == 7) && (blockId5 == 49 || blockId5 == 7) && (blockId6 == 49 || blockId6 == 7) && (blockId == 0) && (aircheck == 0) && !((blockId2 == 7 && blockId3 == 7 && blockId4 == 7 && blockId5 == 7 && blockId6 == 7 && blockId == 0) && (aircheck == 0)) && (countBlksAlongLine(*pos, plugHole.toVector3()).blockCount <= wallRange)) {
									tryPlaceHF(plugHole.toVector3());
								}
							}
                            if (swap.getSelectedValue() == 2)
						        supplies->selectedHotbarSlot = slotHF;
						}
					}
				}
			}
		}
	}
}