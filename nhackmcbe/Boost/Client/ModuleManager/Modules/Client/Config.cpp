#include "Config.h"
#include "../../../Client.h"

Config121::Config121() : Module("ConfigSave", "toggle to save the config", Category::CLIENT) {
	addClickMeSetting("Save config", "save the current config");
}

Config121::~Config121() {
}
void Config121::onEnable() {
	static Notifications* NotificationsMod = (Notifications*)client->moduleMgr->getModule("Notifications");
	ConfigManager* configMgr = nullptr;
	configMgr = new ConfigManager();
	configMgr->saveConfig();
	NotificationsMod->addNotifBox("Saved config", 5.0f);
	this->setEnabled(false);
}