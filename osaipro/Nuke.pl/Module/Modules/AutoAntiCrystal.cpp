#include "../../../Utils/Logger.h"
#include "../../../Utils/Target.h"
#include "../ModuleManager.h"
#include "AutoAntiCrystal.h"

AutoAntiCrystal::AutoAntiCrystal() : IModule(0x0, Category::PLAYER, "Enable AntiCrystal automatically.") {
	registerFloatSetting("Range", &this->range, this->range, 1.f, 15.f);
	registerIntSetting("CrystalCount", &this->counter, this->counter, 1, 6);
}

AutoAntiCrystal::~AutoAntiCrystal() {
}


const char* AutoAntiCrystal::getModuleName() {
	return ("AutoReduces");
}



static std::vector<Entity*> targetListA;
void findEntityA(Entity* currentEntity, bool isRegularEntity) {
	static auto killauraMod = moduleMgr->getModule<AutoAntiCrystal>();

	if (currentEntity->getEntityTypeId() != 71)
		return;

	float dist = (*currentEntity->getPos()).dist(*Game.getLocalPlayer()->getPos());

	if (dist < killauraMod->range) {
		targetListA.push_back(currentEntity);
	}
}
void AutoAntiCrystal::onTick(GameMode* gm) {
	targetListA.clear();
	Game.forEachEntity(findEntityA);
	if (targetListA.size() >= counter) moduleMgr->getModule<AntiCrystal>()->setEnabled(true);
}

void AutoAntiCrystal::onJoiningServer() {
	targetListA.clear();
}