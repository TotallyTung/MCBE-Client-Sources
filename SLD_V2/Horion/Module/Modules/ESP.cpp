#include "ESP.h"

#include "../../../Utils/Target.h"

ESP::ESP() : IModule(0x0, Category::VISUAL, "Makes it easier to find entities around you") {
	registerIntSetting("Player R", &this->plrR, this->plrR, 0, 255);
	registerIntSetting("Player G", &this->plrG, this->plrG, 0, 255);
	registerIntSetting("Player B", &this->plrB, this->plrB, 0, 255);
	registerIntSetting("Mob R", &this->mobR, this->mobR, 0, 255);
	registerIntSetting("Mob G", &this->mobG, this->mobG, 0, 255);
	registerIntSetting("Mob B", &this->mobB, this->mobB, 0, 255);
	registerBoolSetting("MobESP", &this->isMobEsp, this->isMobEsp);
	registerBoolSetting("ItemESP", &this->isItemESP, this->isItemESP);
	registerBoolSetting("2d", &this->is2d, this->is2d);
}

ESP::~ESP() {
}

const char* ESP::getModuleName() {
	return ("ESP");
}

static float rcolors[4];

static std::vector<C_Entity*> coloredFolks;
bool coloredFolkFinder(C_Entity* curEnt, bool isRegularEntity) {
	if (curEnt == nullptr) return false;
	if (curEnt == g_Data.getLocalPlayer()) return false;  // Skip Local player
	if (!curEnt->isAlive()) return false;
	if (!g_Data.getLocalPlayer()->isAlive()) return false;
	if (curEnt->getEntityTypeId() == 71) return false;                    // endcrystal
	if (curEnt->getEntityTypeId() == 66) return false;                    // falling block
	if (curEnt->getEntityTypeId() == 64) return false;                    // item
	if (curEnt->getEntityTypeId() == 69) return false;                    // xp orb
	if (curEnt->width <= 0.01f || curEnt->height <= 0.01f) return false;  // Don't hit this pesky antibot on 2b2e.org
	if (!Target::isValidTarget(curEnt)) return false;

	coloredFolks.push_back(curEnt);
	return true;
}

static std::vector<C_Entity*> friendmanESP;
bool friendFinderESP(C_Entity* curEnt, bool isRegularEntity) {
	if (curEnt == nullptr) return false;
	if (curEnt == g_Data.getLocalPlayer()) return false;  // Skip Local player
	if (!curEnt->isAlive()) return false;
	if (!g_Data.getLocalPlayer()->isAlive()) return false;
	if (curEnt->getEntityTypeId() == 71) return false;                    // endcrystal
	if (curEnt->getEntityTypeId() == 66) return false;                    // falling block
	if (curEnt->getEntityTypeId() == 64) return false;                    // item
	if (curEnt->getEntityTypeId() == 69) return false;                    // xp orb
	if (curEnt->width <= 0.01f || curEnt->height <= 0.01f) return false;  // Don't hit this pesky antibot on 2b2e.org
	if (!Target::isValidTarget(curEnt, false)) return false;
	if (FriendList::findPlayer(curEnt->getNameTag()->getText())) {
		friendmanESP.push_back(curEnt);
		return true;
	}
	return false;
}

static std::vector<C_Entity*> mobESP;
bool mobFinderESP(C_Entity* curEnt, bool isRegularEntity){
	if (curEnt == nullptr) return false;
	if (curEnt == g_Data.getLocalPlayer()) return false;  // Skip Local player
	if (!curEnt->isAlive()) return false;
	if (!g_Data.getLocalPlayer()->isAlive()) return false;
	if (curEnt->getEntityTypeId() == 66) return false;                    // falling block
	if (curEnt->getEntityTypeId() == 64) return false;                    // item
	if (curEnt->getEntityTypeId() == 319) return false;                   // player
	if (curEnt->getEntityTypeId() == 69) return false;                    // xp orb
	if (curEnt->width <= 0.01f || curEnt->height <= 0.01f) return false;  // Don't hit this pesky antibot on 2b2e.org

	mobESP.push_back(curEnt);
	return true;
};

static std::vector<C_Entity*> itemESP;
bool itemFinderESP(C_Entity* curEnt, bool isRegularEntity) {
	if (curEnt == nullptr) return false;
	if (curEnt == g_Data.getLocalPlayer()) return false;  // Skip Local player
	if (!curEnt->isAlive()) return false;
	if (!g_Data.getLocalPlayer()->isAlive()) return false;
	if (curEnt->width <= 0.01f || curEnt->height <= 0.01f) return false;  // Don't hit this pesky antibot on 2b2e.org
	if (curEnt->getEntityTypeId() == 64) {                                // item
		itemESP.push_back(curEnt);
		return true;
	} 
	return false;
};

//void ESP::onPlayerTick(C_Player* player) {
	/*
	if (g_Data.getLocalPlayer() == nullptr || !GameData::canUseMoveKeys())
		return;
	
	coloredFolks.clear();
	friendmanESP.clear();
	mobESP.clear();
	itemESP.clear();

	g_Data.forEachEntity(coloredFolkFinder);
	g_Data.forEachEntity(friendFinderESP);
	if (isMobEsp)
		g_Data.forEachEntity(mobFinderESP);
	if (isItemESP)
		g_Data.forEachEntity(itemFinderESP);*/


void ESP::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();

	if (localPlayer != nullptr && GameData::canUseMoveKeys()) {
		coloredFolks.clear();
		friendmanESP.clear();
		mobESP.clear();
		itemESP.clear();

		g_Data.forEachEntity(coloredFolkFinder);
		g_Data.forEachEntity(friendFinderESP);
		if (isMobEsp)
			g_Data.forEachEntity(mobFinderESP);
		if (isItemESP)
			g_Data.forEachEntity(itemFinderESP);

		for (C_Entity* ent : coloredFolks) {
			DrawUtils::setColor(plrR, plrG, plrB, 1.f);
			if (is2d) {
				DrawUtils::draw2D(ent, (float)fmax(0.4f, 1 / (float)fmax(1, localPlayer->getHumanPos().dist(ent->getHumanPos()) * 3.f)));
			} else {
				DrawUtils::drawEntityBox(ent, (float)fmax(0.2f, 1 / (float)fmax(1, localPlayer->getHumanPos().dist(ent->getHumanPos()))));
			}
		}

		for (C_Entity* ent : friendmanESP) {
			DrawUtils::setColor(0, 255, 0, 1.f);
			if (is2d) {
				DrawUtils::draw2D(ent, (float)fmax(0.4f, 1 / (float)fmax(1, localPlayer->getHumanPos().dist(ent->getHumanPos()) * 3.f)));
			} else {
				DrawUtils::drawEntityBox(ent, (float)fmax(0.2f, 1 / (float)fmax(1, localPlayer->getHumanPos().dist(ent->getHumanPos()))));
			}
		}

		for (C_Entity* ent : mobESP) {
			DrawUtils::setColor(mobR, mobG, mobB, 1.f);
			if (is2d) {
				DrawUtils::draw2D(ent, (float)fmax(0.4f, 1 / (float)fmax(1, localPlayer->getHumanPos().dist(ent->getHumanPos()) * 3.f)));
			} else {
				DrawUtils::drawEntityBox(ent, (float)fmax(0.2f, 1 / (float)fmax(1, localPlayer->getHumanPos().dist(ent->getHumanPos()))));
			}
		}

		for (C_Entity* ent : itemESP) {
			DrawUtils::setColor(0, 0, 255, 1.f);
			DrawUtils::draw2D(ent, (float)fmax(0.4f, 1 / (float)fmax(1, localPlayer->getHumanPos().dist(ent->getHumanPos()) * 3.f)));
		}
	}
}
