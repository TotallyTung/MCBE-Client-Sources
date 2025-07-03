// Created by Tony on 2024-10-10 10:01:54

#include "Module.h"
#include "Client/Notifications.h"

Setting::~Setting() {
	delete minValue;
	delete maxValue;
}

Module::Module(const std::string& mName, const std::string& des, Category c, int k) {
	this->moduleName = obfuscate(mName);
	this->description = des;
	this->category = c;
	this->keybind = k;
	addBoolCheck("Visible", "Show module in Arraylist.", &visible);
	addKeybindSetting("Keybind", "Changes module keybind.", &keybind);
	addEnumSetting("Toggle", "Changes toggle modes.", { "Normal", "Hold" }, &toggleMod);
}

Module::~Module() {
	for (Setting* setting : this->settingList) {
		delete setting;
	}
}

std::string Module::getModuleName() {
	return deobfuscate(this->moduleName);
}

std::string Module::getModName() {
	return this->modName;
}

std::string Module::getDescription() {
	return this->description;
}

bool Module::isEnabled() {
	return this->enabled;
}

void Module::setEnabled(bool enabled) {
	if (this->enabled != enabled) {
		this->enabled = enabled;

		if (this->getModuleName() != "ClickGui") {
			std::string message = std::string(this->getModuleName()) + (enabled ? " has been enabled" : " has been disabled");
			Notifications::addNotifBox(message, 2.f);
		}

		if (enabled) this->onEnable();
		else this->onDisable();
	}
}

void Module::toggle() {
	this->setEnabled(!this->enabled);
}

bool Module::isVisible() {
	return this->visible;
}

void Module::onEnable() {
}

void Module::onDisable() {
}

void Module::onSaveConfig(json* currentConfig) {
    json obj;
    obj["enabled"] = enabled;

    for (Setting* setting : this->settingList) {
        switch (setting->settingType) {
        case (SettingType::KEYBIND):
            obj["Keybind"] = setting->valuePtr->_int;
            break;
        case (SettingType::BOOL):
            obj[setting->settingName] = setting->valuePtr->_bool;
            break;
        case (SettingType::ENUM):
            obj[setting->settingName] = *setting->enumValue;
            break;
        case (SettingType::COLOR): {
            obj[setting->settingName]["RGB"] = ColorUtils::RGBtoHex(*setting->colorPtr);
            obj[setting->settingName]["Alpha"] = setting->colorPtr->a;
            break;
        }
        case (SettingType::SLIDER):
            if (setting->valueType == ValueType::INT_T) {
                obj[setting->settingName] = setting->valuePtr->_int;
            }
            else if (setting->valueType == ValueType::FLOAT_T) {
                obj[setting->settingName] = setting->valuePtr->_float;
            }
            break;
        }
    }
    obj["friend"] = TargetUtils::Friend;
    (*currentConfig)[getModuleName()] = obj;
}

void Module::onLoadConfig(json* config) {
    if (config->contains(getModuleName())) {
        auto obj = config->at(getModuleName());
        if (obj.is_null()) return;

        if (obj.contains("enabled")) {
            enabled = obj["enabled"].get<bool>();
        }
        if (obj.contains("friend")) {
            TargetUtils::Friend = obj["friend"].get<std::vector<std::string>>();
        }


        for (Setting* setting : this->settingList) {
            if (obj.contains(setting->settingName)) {
                switch (setting->settingType) {
                case (SettingType::KEYBIND):
                    setting->valuePtr->_int = obj[setting->settingName].get<int>();
                    break;
                case (SettingType::BOOL):
                    setting->valuePtr->_bool = obj[setting->settingName].get<bool>();
                    break;
                case (SettingType::ENUM):
                    *setting->enumValue = obj[setting->settingName].get<int>();
                    break;
                case (SettingType::COLOR): {
                    auto colorObj = obj[setting->settingName];
                    if (colorObj.contains("RGB")) {
                        *setting->colorPtr = ColorUtils::HexToRGB(colorObj["RGB"].get<std::string>());
                    }
                    if (colorObj.contains("Alpha")) {
                        setting->colorPtr->a = colorObj["Alpha"].get<int>();
                    }
                    break;
                }
                case (SettingType::SLIDER):
                    if (setting->valueType == ValueType::INT_T) {
                        setting->valuePtr->_int = obj[setting->settingName].get<int>();
                    }
                    else if (setting->valueType == ValueType::FLOAT_T) {
                        setting->valuePtr->_float = obj[setting->settingName].get<float>();
                    }
                    break;
                }
            }
        }
    }
}
void Module::onKeyUpdate(int key, bool isDown) {
	if (key == this->keybind) {
		if (this->toggleMod == 0) { // Normal
			if (isDown) this->toggle();
		}
		else { // Hold
			this->setEnabled(isDown);
		}
	}
}

void Module::onRender(MinecraftUIRenderContext* ctx) {
}

void Module::onImGuiRender(ImDrawList* drawlist) {
}

void Module::onNormalTick(Actor* actor) {
}
void Module::onContainerScreenControllerTickEvent(ContainerScreenControllerTickEvent* events) {
}

void Module::onSendPacket(Packet* packet, bool& shouldCancel) {
}

void Module::onLevelRender() {
}
