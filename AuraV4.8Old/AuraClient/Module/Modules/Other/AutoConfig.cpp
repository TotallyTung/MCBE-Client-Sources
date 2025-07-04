#include "AutoConfig.h"
#include "../../ModuleManager.h"

AutoConfig::AutoConfig() : IModule(0, Category::OTHER, "Save Config Click") {
	registerBoolSetting("AutoSave", &AutoSave, false);
}

AutoConfig::~AutoConfig() {
}

const char* AutoConfig::getModuleName() {
	return ("ConfigSave");
}

void AutoConfig::onTick(C_GameMode* gm) {
	configMgr->saveConfig();
	SettingMgr->saveSettings();
	clientMessageF("[%sAuraV4%s] %sSuccessfully saved config %s%s%s!", GREEN, WHITE, GREEN, GRAY, configMgr->currentConfig.c_str(), GREEN);
	clientMessageF("[%sAuraV4%s] %sSuccessfully saved settings %s%s%s!", GREEN, WHITE, GREEN, GRAY, SettingMgr->currentSettings.c_str(), GREEN);
	static auto AutoConfigMod = moduleMgr->getModule<AutoConfig>();
	AutoConfigMod->setEnabled(false);
}
