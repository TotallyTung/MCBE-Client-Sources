#include "CustomFont.h"
#include "../../../Client.h"
#include "../../../ConfigManager/ConfigManager.h"
bool saveconfig; 
ClientManager::ClientManager() : Module("ClientManager", "Changes your current Client font.", Category::CLIENT) {
	enumsetting("Switch", "NULL", { "Arial", "Minecraft", "DMSans", "RaleWay"}, &fontType);
	boolsetting("Shadow", "NULL", &fontShadow);
	boolsetting("Italic", "NULL", &fontItalic);
	boolsetting("Save Config", "NULL", &saveconfig);
	slider<float>("Size", "NULL", ValueType::FLOAT_T, &fontsize1, 10.f, 60.f);
	enumsetting("Color Mode", "Change colors mode", { "Single", "Rainbow", "Wave" }, &modeValue);
	slider<float>("Seconds", "Changes rainbow speed.", ValueType::FLOAT_T, &seconds, 1.f, 10.f);
	slider<float>("Saturation", "Changes rainbow saturation.", ValueType::FLOAT_T, &saturation, 0.f, 1.f);
	slider<float>("Brightness", "Changes rainbow brightness.", ValueType::FLOAT_T, &brightness, 0.f, 1.f);
	slider<int>("Seperation", "Changes color seperation.", ValueType::INT_T, &seperation, 0, 250);
	colorsetting("Color 1", "NULL", &mainColor);
	colorsetting("Color 2", "NULL", &secondaryColor);
}

bool ClientManager::isEnabled() {
	return true;
}

void ClientManager::setEnabled(bool enabled) {
	// Do nothing :D
}

bool ClientManager::isVisible() {
	return false;
}

void ClientManager::onNormalTick(Actor* actor) {
	if (saveconfig) {
		saveconfig = false;
		configMgr = new ConfigManager();
		static Notifications* NotificationsMod = (Notifications*)client->moduleMgr->getModule("Notifications");
		configMgr->saveConfig();
		NotificationsMod->addNotifBox("Saved.", 5.0f);
	}
}