#include "CombatInfo.h"
using json = nlohmann::json;
CombatInfo::CombatInfo() : IModule(0x0, Category::VISUAL, "Combat Info") {
	registerFloatSetting("Pos-X", &posx, 2.5f, 0.f, Game.getClientInstance()->getGuiData()->windowSizeReal.x);
	registerFloatSetting("Pos-Y", &posy, 100.f, 0.f, Game.getClientInstance()->getGuiData()->windowSizeReal.y);
	registerBoolSetting("Show in ClickGui", &showinclickgui,true);
	registerFloatSetting("Scale", &scale, 1.f, 0.f, 2.f);
	registerBoolSetting("Show Keybind", &showkb__, true);
}
CombatInfo::~CombatInfo() {
};
const char* CombatInfo::getModuleName() {
	return "CombatInfo";
}
void CombatInfo::onImGuiRender(ImDrawList* d) {
	if (Game.getLocalPlayer() == nullptr) return;
	if (!GameData::canUseMoveKeys() && (!moduleMgr->getModule<ClickGuiMod>()->isEnabled() || !showinclickgui) || (moduleMgr->getModule<ClickGuiMod>()->isEnabled() && !showinclickgui))
		return;
	if (ModName_NameAlias == std::vector<std::pair<std::string,std::string>> {})
		return;
	float currentX = 0.f;
	float currentY = posy - 10;
	for (std::pair<std::string, std::string> na : ModName_NameAlias) {
		ImGuiUtils::drawText(Vec2(posx + 2.5f, currentY), na.second.c_str(), UIColor(255,255,255,255), scale, false);
		if (moduleMgr->getModuleByName(na.first).value()->getModuleName() != "PointBreak") {
			ImGuiUtils::drawText(Vec2(posx + 2.5f + ImGuiUtils::getTextWidth(na.second.c_str(), scale), currentY), std::string(" [ ").c_str(), UIColor(165, 165, 165, 255), scale, false);
			ImGuiUtils::drawText(Vec2(posx + 2.5f + ImGuiUtils::getTextWidth(std::string(na.second + " [ " + std::string(moduleMgr->getModuleByName(na.first).value()->isEnabled() ? "ON" : "OFF")).c_str(), scale), currentY), std::string(" ]").c_str(), UIColor(165, 165, 165, 255), scale, false);
			ImGuiUtils::drawText(Vec2(posx + 2.5f + ImGuiUtils::getTextWidth(std::string(na.second + " [ ").c_str(), scale), currentY), std::string(moduleMgr->getModuleByName(na.first).value()->isEnabled() ? "ON" : "OFF").c_str(), moduleMgr->getModuleByName(na.first).value()->isEnabled() ? UIColor(0, 255, 0, 255) : UIColor(225, 0, 0, 255), scale, false);
			if (std::string(Utils::getKeybindName(moduleMgr->getModuleByName(na.first).value()->getKeybind())) != "Unknown" && showkb__)
				ImGuiUtils::drawText(Vec2(posx + ImGuiUtils::getTextWidth(std::string(na.second + " [ " + std::string(moduleMgr->getModuleByName(na.first).value()->isEnabled() ? "ON" : "OFF") + " ]").c_str(), scale), currentY), std::string(std::string(" [ ") + std::string(Utils::getKeybindName(moduleMgr->getModuleByName(na.first).value()->getKeybind())) + std::string(" ]")).c_str(), UIColor(165, 165, 165, 255), scale, false);
		}
		else {
			static PointBreak* PointBreakMod = moduleMgr->getModule<PointBreak>();
			ImGuiUtils::drawText(Vec2(posx + 2.5f + ImGuiUtils::getTextWidth(na.second.c_str(), scale), currentY), std::string(" [ ").c_str(), UIColor(165, 165, 165, 255), scale, false);
			ImGuiUtils::drawText(Vec2(posx + 2.5f + ImGuiUtils::getTextWidth(std::string(na.second + " [ " + std::string((PointBreakMod->mode.getSelectedValue() != 0 && PointBreakMod->breaking) ? "BREAKING" : "NOT BREAKING")).c_str(), scale), currentY), std::string(" ]").c_str(), UIColor(165, 165, 165, 255), scale, false);
			ImGuiUtils::drawText(Vec2(posx + 2.5f + ImGuiUtils::getTextWidth(std::string(na.second + " [ ").c_str(), scale), currentY), std::string((PointBreakMod->mode.getSelectedValue() != 0 && PointBreakMod->breaking) ? "BREAKING" : "NOT BREAKING").c_str(), (PointBreakMod->mode.getSelectedValue() != 0 && PointBreakMod->breaking) ? UIColor(0, 255, 0, 255) : UIColor(225, 0, 0, 255), scale, false);
			if (std::string(Utils::getKeybindName(PointBreakMod->keybinds)) != "Unknown" && showkb__)
				ImGuiUtils::drawText(Vec2(posx + ImGuiUtils::getTextWidth(std::string(na.second + " [ " + std::string((PointBreakMod->mode.getSelectedValue() != 0 && PointBreakMod->breaking) ? "BREAKING" : "NOT BREAKING") + " ]").c_str(), scale), currentY), std::string(std::string(" [ ") + std::string(Utils::getKeybindName(PointBreakMod->keybinds)) + std::string(" ]")).c_str(), UIColor(165, 165, 165, 255), scale, false);
		};
		currentY = currentY - 15 * scale;
		if (ImGuiUtils::getTextWidth(na.second.c_str(), scale) > currentX)
			currentX = ImGuiUtils::getTextWidth(na.second.c_str(), scale);
	}
}
void CombatInfo::onSaveConfig(void* conf) {
	IModule::onSaveConfig(conf);
	json* confS = reinterpret_cast<json*>(conf);
	// Save to json
	if (confS->contains("CombatInfoSetting"))
		confS->erase("CombatInfoSetting");

	json obj = {};

	for (const auto& wind : ModName_NameAlias) {
		obj[wind.first] = wind.second;
	}

	confS->emplace("CombatInfoSetting", obj);
}
void CombatInfo::onLoadConfig(void* conf) {
	IModule::onLoadConfig(conf);
	ModName_NameAlias.clear();
	json* confS = reinterpret_cast<json*>(conf);
	if (confS->contains("CombatInfoSetting")) {
		auto obj = confS->at("CombatInfoSetting");
		if (obj.is_null())
			return;
		try {
			for (auto it = obj.begin(); it != obj.end(); ++it) {
				ModName_NameAlias.push_back(std::make_pair(it.key(), it.value()));
			}
		}
		catch (std::exception e) {
			logF("Config Load Error (CombatInfoSertting): %s", e.what());
		}
	};
}