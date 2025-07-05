#include "RobotHolefiller.h"
#include "../../../Utils/Target.h"

RobotHolefiller::RobotHolefiller() : IModule(0, Category::COMBAT, "Increase the skillz needed to conduct CPVP by automatically turning on holefill when an enemy leaves a hole") {
	registerFloatSetting("Entity Range", &this->entRange, this->entRange, 0.f, 10.f);
	registerIntSetting("Wait (ticks)", &this->wait, this->wait, 0, 20);
	registerIntSetting("Hold (ticks)", &this->hold, this->hold, 0, 20);
	//registerBoolSetting("Disable After", &this->disableOnStop, this->disableOnStop);

	#ifdef _DEBUG
	registerBoolSetting("testOnSelf(dbg)", &this->testOnSelf, this->testOnSelf);
	#endif  // DEBUG
}

RobotHolefiller::~RobotHolefiller() {
}

const char* RobotHolefiller::getModuleName() {
	return ("RobotHolefiller");
}

void RobotHolefiller::onEnable() {
	turnOnReady = false;
	finW = false;
}

bool getSurroundStatusOfPlayer(C_Entity* nigger) {
	vec3_ti loc = nigger->getHumanPos().floor();
	//int output = false;

	const vec3_ti sides[4] = {
		loc.add(1, 0, 0),
		loc.sub(1, 0, 0),
		loc.sub(0, 0, 1),
		loc.add(0, 0, 1)};

	std::vector<vec3_ti> surrounded;

	for (vec3_ti side : sides) {
		C_Block* block = g_Data.getLocalPlayer()->region->getBlock(side);
		C_BlockLegacy* blockLegacy = (block->blockLegacy);
		if (blockLegacy->blockId == 0) { // check for exposure
			return false;
		}

		//if (blockLegacy->blockId != 7 && blockLegacy->blockId != 49 && blockLegacy->blockId != 0)  // check for non-obby,bedrock surround
			//output = true;
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
	if (!Target::isValidTarget(curEnt)) return false;

	float dist = (curEnt->getHumanPos()).dist(g_Data.getLocalPlayer()->getHumanPos());
	if (dist <= moduleMgr->getModule<RobotHolefiller>()->entRange) {
		ppl_PP.push_back(curEnt);
		return true;
	}
	return false;
}

void RobotHolefiller::onWorldTick(C_GameMode* gm) {
	ppl_PP.clear();
	//targs.clear();

	if (g_Data.getLocalPlayer() == nullptr) return;

	g_Data.forEachEntity(pplFinder600);

	#ifdef _DEBUG
	if (testOnSelf) {
		ppl_PP.push_back(g_Data.getLocalPlayer());
	}
	#endif  // DEBUG


	for (auto& person : ppl_PP) {
		//clientMessageF("person: %s  ~ %i ", person->getNameTag(), getSurroundStatusOfPlayer(person));
		if (!getSurroundStatusOfPlayer(person)) {  // nigger finally came out of the hole
			//clientMessageF("He probably shoudlnt have come out");
			turnOnReady = true;
			targs.clear();
		} 
	}
}

void RobotHolefiller::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr) return;

	if (turnOnReady) {
		// wait delay
		if (wDel < wait && !finW) {
			wDel++;
			return;
		}
		wDel = 0;
		finW = true;

		// enable holefiller
		auto HF = moduleMgr->getModule<HoleFiller>();
		if (!HF->isEnabled())
			HF->setEnabled(true);

		if (hDel < hold) {
			hDel++;
			return;
		}
		hDel = 0;
		finW = false;

		turnOnReady = false;

		if (HF->isEnabled())
			HF->setEnabled(false);

		if (disableOnStop)
			this->setEnabled(false);
	}
}



