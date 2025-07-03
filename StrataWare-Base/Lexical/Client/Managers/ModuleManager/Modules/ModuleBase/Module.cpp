#include "Module.h"
#include "../../../../../Libs/json.hpp"

#include "../../../NotificationManager/NotificationManager.h"

using json = nlohmann::json;

Module::Module(std::string moduleName, std::string des, Category c, int k) {
	this->name = moduleName;
	this->description = des;
	this->category = c;
	this->keybind = k;

	registerSetting(new ToggleSetting("Visible", "Visible on arraylist", &visible, true));
	registerSetting(new KeybindSetting("Keybind", "Keybind of module", &keybind, k));
	registerSetting(new EnumSetting("Toggle", "How module should be toggled", { "Press", "Hold" }, &toggleMode, 0));
}

Module::~Module() {
	for (auto& setting : settings) {
		delete setting;
		setting = nullptr;
	}
	settings.clear();
}

std::string Module::getModeText() {
	return "NULL";
}

bool Module::isEnabled() {
	return enabled;
}

bool Module::isVisible() {
	return visible;
}

bool Module::isHoldMode() {
	return toggleMode;
}

int Module::getKeybind() {
	return keybind;
}

void Module::setKeybind(int key) {
	this->keybind = key;
}

bool Module::runOnBackground() {
	return false;
}

void Module::setEnabled(bool enable) {
	if (this->enabled != enable) {
		this->enabled = enable;

		if (enable) {
			this->onEnable();
		}
		else {
			this->onDisable();
		}
	}
}

void Module::toggle() {
	setEnabled(!enabled);
}

void Module::onDisable() {
}

void Module::onEnable() {
}

void Module::onKeyUpdate(int key, bool isDown) {
	static bool inKeyUpdate = false;
	if (inKeyUpdate) return;
	inKeyUpdate = true;

	if (getKeybind() == key) {
		if (isHoldMode()) {
			setEnabled(isDown);
		}
		else {
			if (isDown) {
				toggle();
			}
		}
	}

	inKeyUpdate = false;
}


void Module::onPlayerTick() {
}

void Module::onNormalTick(LocalPlayer* localPlayer) {
}

void Module::onWorldTick(Level* level) {
}

void Module::onUpdateRotation(LocalPlayer* localPlayer) {
}

bool Module::onSendPacket(Packet* packet) {
	return false;
}

bool Module::onSendDelayedPacket(Packet* packet) {
	return false;
}


void Module::onD2DRender() {
}

void Module::onMatrixRender(glm::mat4* matrix) {
}

void Module::onMCRender(MinecraftUIRenderContext* renderCtx) {
}

void Module::onLevelRender() {
}

void Module::onLoadConfig(void* confVoid) {
	json* conf = reinterpret_cast<json*>(confVoid);
	std::string modName = this->getModuleName();

	if (conf->contains(modName)) {
		json obj = conf->at(modName);
		if (obj.is_null())
			return;

		if (obj.contains("enabled")) {
			this->setEnabled(obj.at("enabled").get<bool>());
		}

		for (auto& setting : settings) {
			std::string settingName = setting->name;

			if (obj.contains(settingName)) {
				json confValue = obj.at(settingName);
				if (confValue.is_null())
					continue;

				switch (setting->type) {
				case SettingType::BOOL_S: {
					ToggleSetting* tse = static_cast<ToggleSetting*>(setting);
					(*tse->value) = confValue.get<bool>();
					break;
				}
				case SettingType::KEYBIND_S: {
					KeybindSetting* keybindSetting = static_cast<KeybindSetting*>(setting);
					(*keybindSetting->value) = confValue.get<int>();
					break;
				}
				case SettingType::ENUM_S: {
					EnumSetting* enumSetting = static_cast<EnumSetting*>(setting);
					(*enumSetting->value) = confValue.get<int>();
					break;
				}
				case SettingType::COLOR_S: {
					ColorSetting* colorSetting = static_cast<ColorSetting*>(setting);
					(*colorSetting->colorPtr) = ColorUtil::HexStringToColor(confValue.get<std::string>());
					break;
				}
				case SettingType::SLIDER_S: {
					SliderSettingBase* sliderSettingBase = static_cast<SliderSettingBase*>(setting);
					if (sliderSettingBase->valueType == ValueType::INT_T) {
						NumberSetting<int>* intSlider = static_cast<NumberSetting<int>*>(sliderSettingBase);
						(*intSlider->valuePtr) = confValue.get<int>();
					}
					else if (sliderSettingBase->valueType == ValueType::FLOAT_T) {
						NumberSetting<float>* floatSlider = static_cast<NumberSetting<float>*>(sliderSettingBase);
						(*floatSlider->valuePtr) = confValue.get<float>();
					}
					break;
				}
				}
			}
		}
	}
}

void Module::onSaveConfig(void* confVoid) {
	json* conf = reinterpret_cast<json*>(confVoid);
	std::string modName = this->getModuleName();
	json obj = (*conf)[modName];

	obj["enabled"] = this->isEnabled();

	for (auto& setting : settings) {
		std::string settingName = setting->name;

		switch (setting->type) {
		case SettingType::BOOL_S: {
			ToggleSetting* tse = static_cast<ToggleSetting*>(setting);
			obj[settingName] = (*tse->value);
			break;
		}
		case SettingType::KEYBIND_S: {
			KeybindSetting* keybindSetting = static_cast<KeybindSetting*>(setting);
			obj[settingName] = (*keybindSetting->value);
			break;
		}
		case SettingType::ENUM_S: {
			EnumSetting* enumSetting = static_cast<EnumSetting*>(setting);
			obj[settingName] = (*enumSetting->value);
			break;
		}
		case SettingType::COLOR_S: {
			ColorSetting* colorSetting = static_cast<ColorSetting*>(setting);
			obj[settingName] = ColorUtil::ColorToHexString((*colorSetting->colorPtr));
			break;
		}
		case SettingType::SLIDER_S: {
			SliderSettingBase* sliderSettingBase = static_cast<SliderSettingBase*>(setting);
			if (sliderSettingBase->valueType == ValueType::INT_T) {
				NumberSetting<int>* intSlider = static_cast<NumberSetting<int>*>(sliderSettingBase);
				obj[settingName] = (*intSlider->valuePtr);
			}
			else if (sliderSettingBase->valueType == ValueType::FLOAT_T) {
				NumberSetting<float>* floatSlider = static_cast<NumberSetting<float>*>(sliderSettingBase);
				obj[settingName] = (*floatSlider->valuePtr);
			}
			break;
		}
		}
	}

	(*conf)[modName] = obj;
}