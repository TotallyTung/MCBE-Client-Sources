#include "CpsAura.h"

CpsAura::CpsAura() : IModule(0, Category::COMBAT, "High Cps by hank") {
	this->registerBoolSetting("MultiAura", &this->isMulti, this->isMulti);
	this->registerFloatSetting("range", &this->range, this->range, 2.f, 20.f);
}

CpsAura::~CpsAura() {
}

const char* CpsAura::getModuleName() {
	return ("CpsAura");
}

static std::vector<C_Entity*> targetListS;

void findEntityS(C_Entity* currentEntityS, bool isRegularEntityS) {
	static auto killauraMod = moduleMgr->getModule<CpsAura>();

	if (currentEntityS == nullptr)
		return;

	if (currentEntityS == g_Data.getLocalPlayer())  // Skip Local player
		return;

	if (!g_Data.getLocalPlayer()->canAttack(currentEntityS, false))
		return;

	if (!g_Data.getLocalPlayer()->isAlive())
		return;

	if (!currentEntityS->isAlive())
		return;

	if (killauraMod->isMobAura) {
		if (currentEntityS->getNameTag()->getTextLength() <= 1 && currentEntityS->getEntityTypeId() == 63)
			return;
		if (currentEntityS->width <= 0.01f || currentEntityS->height <= 0.01f)  // Don't hit this pesky antibot on 2b2e.org
			return;
		if (currentEntityS->getEntityTypeId() == 64)  // item
			return;
		if (currentEntityS->getEntityTypeId() == 69)  // xp
			return;
	} else {
		if (!Target::isValidTarget(currentEntityS))
			return;
	}

	float dist = (*currentEntityS->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < killauraMod->range) {
		targetListS.push_back(currentEntityS);
	}
}

void CpsAura::onTick(C_GameMode* gm) {
	//Loop through all our players and retrieve their information
	targetListS.clear();

	g_Data.forEachEntity(findEntityS);

	Odelay++;
	if (!targetListS.empty() && Odelay >= delay) {
		// Attack all entitys in targetList
		if (isMulti) {
			for (auto& i : targetListS) {
				if (!(i->damageTime > 1 && hurttime)) {
					g_Data.getLocalPlayer()->swing();
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
					g_Data.getCGameMode()->attack(i);
					
				
				}
			}
		} else {
			if (!(targetListS[0]->damageTime > 1 && hurttime)) {
				g_Data.getLocalPlayer()->swing();
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);

			}
		}
	}
}