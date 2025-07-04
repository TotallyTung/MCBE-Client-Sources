#include "CombatModuleInfo.h"

CombatInfo::CombatInfo() : IModule(0, Category::VISUAL, "Combat Modules Display") {
	registerFloatSetting("X", &this->CombatInfoX, this->CombatInfoX, 0.f, g_Data.getClientInstance()->getGuiData()->windowSize.x);
	registerFloatSetting("Y", &this->CombatInfoY, this->CombatInfoY, 0.f, g_Data.getClientInstance()->getGuiData()->windowSize.y);
	registerFloatSetting("Scale", &this->scale, this->scale, 0.5f, 1.5f);
}

CombatInfo::~CombatInfo() {
}

const char* CombatInfo::getModuleName() {
	return ("CombatModuleInfo");
}

void CombatInfo::onMove(C_MoveInputHandler* input) {
}

void CombatInfo::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	float yVal = CombatInfoY;
	float xVal = CombatInfoX;
	auto kam = moduleMgr->getModule<Killaura>();
	auto srm = moduleMgr->getModule<TPAura>();
	auto acm = moduleMgr->getModule<AutoCrystal2>();
	auto cbm = moduleMgr->getModule<Surround>();
	auto wtam = moduleMgr->getModule<Flight>();
	auto xtxm = moduleMgr->getModule<Jetpack>();
	auto aufm = moduleMgr->getModule<NoClip>();
	auto alm = moduleMgr->getModule<NoClipGlide>();

	std::string killaura = "KA: ";
	std::string tpaura = "TPA: ";
	std::string Crystalaura = "CA: ";
	std::string Surround = "SRD: ";
	std::string Flight = "Fly: ";
	std::string Jetpack = "Jet: ";
	std::string NoClip = "NC: ";
	std::string NoClipGlide = "NCG: ";
	std::string disable = "OFF";
	std::string enable = "ON";
	float kaw = DrawUtils::getTextWidth(&killaura, scale);
	float srw = DrawUtils::getTextWidth(&tpaura, scale);
	float acw = DrawUtils::getTextWidth(&Crystalaura, scale);
	float cbw = DrawUtils::getTextWidth(&Surround, scale);
	float wtaw = DrawUtils::getTextWidth(&Flight, scale);
	float xtxw = DrawUtils::getTextWidth(&Jetpack, scale);
	float hkw = DrawUtils::getTextWidth(&NoClip, scale);
	float aufw = DrawUtils::getTextWidth(&NoClipGlide, scale);

	auto interfaceColor = ColorUtil::interfaceColor(1);
	DrawUtils::drawText(vec2_t(CombatInfoX, CombatInfoY), &killaura, MC_Color(interfaceColor), scale);
	DrawUtils::drawText(vec2_t(CombatInfoX + kaw, CombatInfoY), kam->isEnabled() ? &enable : &disable, kam->isEnabled() ? MC_Color(0, 255, 0, 0) : MC_Color(255, 0, 0, 0), scale);
	DrawUtils::drawText(vec2_t(CombatInfoX, CombatInfoY + 10.f), &tpaura, MC_Color(interfaceColor), scale);
	DrawUtils::drawText(vec2_t(CombatInfoX + srw, CombatInfoY + 10.f), srm->isEnabled() ? &enable : &disable, srm->isEnabled() ? MC_Color(0, 255, 0, 0) : MC_Color(255, 0, 0, 0), scale);
	DrawUtils::drawText(vec2_t(CombatInfoX, CombatInfoY + 20.f), &Crystalaura, MC_Color(interfaceColor), scale);
	DrawUtils::drawText(vec2_t(CombatInfoX + acw, CombatInfoY + 20.f), acm->isEnabled() ? &enable : &disable, acm->isEnabled() ? MC_Color(0, 255, 0, 0) : MC_Color(255, 0, 0, 0), scale);
	DrawUtils::drawText(vec2_t(CombatInfoX, CombatInfoY + 30.f), &Surround, MC_Color(interfaceColor), scale);
	DrawUtils::drawText(vec2_t(CombatInfoX + cbw, CombatInfoY + 30.f), cbm->isEnabled() ? &enable : &disable, cbm->isEnabled() ? MC_Color(0, 255, 0, 0) : MC_Color(255, 0, 0, 0), scale);

	DrawUtils::drawText(vec2_t(CombatInfoX + 50.f, CombatInfoY), &Flight, MC_Color(interfaceColor), scale);
	DrawUtils::drawText(vec2_t(CombatInfoX + 50.f + wtaw, CombatInfoY), wtam->isEnabled() ? &enable : &disable, wtam->isEnabled() ? MC_Color(0, 255, 0, 0) : MC_Color(255, 0, 0, 0), scale);
	DrawUtils::drawText(vec2_t(CombatInfoX + 50.f, CombatInfoY + 10.f), &Jetpack, MC_Color(interfaceColor), scale);
	DrawUtils::drawText(vec2_t(CombatInfoX + 50.f + xtxw, CombatInfoY + 10.f), xtxm->isEnabled() ? &enable : &disable, xtxm->isEnabled() ? MC_Color(0, 255, 0, 0) : MC_Color(255, 0, 0, 0), scale);


	DrawUtils::drawText(vec2_t(CombatInfoX + 50.f, CombatInfoY + 20.f), &NoClip, MC_Color(interfaceColor), scale);
	DrawUtils::drawText(vec2_t(CombatInfoX + 50.f + aufw, CombatInfoY + 20.f), aufm->isEnabled() ? &enable : &disable, aufm->isEnabled() ? MC_Color(0, 255, 0, 0) : MC_Color(255, 0, 0, 0), scale);
	DrawUtils::drawText(vec2_t(CombatInfoX + 50.f, CombatInfoY + 30.f), &NoClipGlide, MC_Color(interfaceColor), scale);
	DrawUtils::drawText(vec2_t(CombatInfoX + 50.f + aufw, CombatInfoY + 30.f), aufm->isEnabled() ? &enable : &disable, aufm->isEnabled() ? MC_Color(0, 255, 0, 0) : MC_Color(255, 0, 0, 0), scale);


}
void CombatInfo::onDisable() {
	auto player = g_Data.getLocalPlayer();
}



