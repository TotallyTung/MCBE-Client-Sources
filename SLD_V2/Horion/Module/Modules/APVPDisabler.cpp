#include "APVPDisabler.h"

APVPDisabler::APVPDisabler() : IModule(0, Category::WORLD, "stops villafag654 servers from doing that weird anti-place shit; rmb to turn off rotates")  {
	// registerBoolSetting("Packlet Mode", &this->packetLook, this->packetLook); - doesnt work right
	registerIntSetting("Angle", &this->angle, this->angle, -360, 360);
	registerBoolSetting("On Mod w/ build", &this->onlyOnBuildMods, this->onlyOnBuildMods);
	registerBoolSetting("Pause On XP", &this->pauseOnXP, this->pauseOnXP);
	registerBoolSetting("Pause On Bow", &this->pauseOnBow, this->pauseOnBow);
	registerBoolSetting("Pause On Mine", &this->pauseOnMine, this->pauseOnMine);
}

APVPDisabler::~APVPDisabler() {
}

const char* APVPDisabler::getModuleName() {
	return "APVPDisabler";
}

void APVPDisabler::onPlayerTick(C_Player* player) {
	if (g_Data.getLocalPlayer() == nullptr)
		return;

	if (!packetLook) {
		if (onlyOnBuildMods) {
			// modules with build capabilities

			// crystalauras
			bool CPlace = moduleMgr->getModule<CrystalPlace>()->isEnabled();
			bool CABPG = moduleMgr->getModule<CrystalAura>()->isEnabled();
			bool CAOW = moduleMgr->getModule<CrystalAura>()->isEnabled();
			bool CAWTA = moduleMgr->getModule<CrystalAura>()->isEnabled();

			// holefiller
			bool HF = moduleMgr->getModule<HoleFiller>()->isEnabled();

			// surround
			bool jtwdSurr = moduleMgr->getModule<Surround>()->isEnabled();
			bool renSurr = moduleMgr->getModule<RenSurround>()->isEnabled();
			bool burr = moduleMgr->getModule<Burrow>()->isEnabled();

			// scaffold
			bool scaf = moduleMgr->getModule<Scaffold>()->isEnabled();
			bool tower = moduleMgr->getModule<Tower>()->isEnabled();

			// specials
			bool AnvA = moduleMgr->getModule<AnvilAura>()->isEnabled();
			bool AncA = moduleMgr->getModule<AnchorAura>()->isEnabled();
			bool trap = moduleMgr->getModule<AutoTrap>()->isEnabled();

			if (not(CPlace || CABPG || CAOW || CAWTA || HF || jtwdSurr || renSurr || burr || scaf || tower || AnvA || AncA || trap))
				return;  // none of the above modules were enabled
		}

		if (GameData::isRightClickDown()) {
			if (pauseOnXP) {
				if ((g_Data.getLocalPlayer()->getSelectedItemId() == 508 || g_Data.getLocalPlayer()->getSelectedItemId() == 374))
					return;
			}

			if (pauseOnBow) {
				if (g_Data.getLocalPlayer()->getSelectedItemId() == 374 == 300)
					return;
			}

			if (GameData::isLeftClickDown() && pauseOnMine)
				return;

		}

	}
	// here's the actual main part; its short af
	player->pitch = angle;
}
