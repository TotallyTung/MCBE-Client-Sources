#pragma once

#include <string>
#include "../SDK/Classes/MinecraftUIRenderContext.h"
#include <functional>
#include "../../../Thrift/Client/SDK/Minecraft.h"

class Category;

struct ModuleEvent {
	bool isEnabled, isTicking;
};

enum class SettingType {
	BOOL,
	INT,
	FLOAT,
};

struct SettingValue {
	union {
		float _float;
		int _int;
		bool _bool;
	};
};

class Setting {
public:
	std::string name;

	SettingType settingType;
	SettingValue settingValue;

	SettingValue* valuePtr = nullptr;
	SettingValue* minValue = nullptr;
	SettingValue* maxValue = nullptr;

	~Setting();
};

class Module {
public:
	std::string name;
	std::string description;
	Category* category;

	std::vector<Setting*> settingList;
public:
	Module(Category*, std::string, std::string);
public:
	bool isEnabled = false, wasEnabled = false;
	bool isExtending = false;
	uint64_t key = NULL;
	
public:
	bool debugState = false;
protected:
	void Float(const std::string& name, float min, float max, float* valuePtr) {
		Setting* setting = new Setting();
		setting->name = name;
		setting->settingType = SettingType::FLOAT;
		setting->valuePtr = (SettingValue*)(valuePtr);
		setting->minValue = (SettingValue*)(new float(min));
		setting->maxValue = (SettingValue*)(new float(max));
		this->settingList.push_back(setting);
	}

	void Int(const std::string& name, int min, int max, int* valuePtr) {
		Setting* setting = new Setting();
		setting->name = name;
		setting->settingType = SettingType::INT;
		setting->valuePtr = (SettingValue*)(valuePtr);
		setting->minValue = (SettingValue*)(new int(min));
		setting->maxValue = (SettingValue*)(new int(max));
		this->settingList.push_back(setting);
	}
	void Bool(const std::string& name, bool* boolPtr) {
		Setting* setting = new Setting();
		setting->name = name;
		setting->settingType = SettingType::BOOL;
		setting->valuePtr = (SettingValue*)(boolPtr);
		this->settingList.push_back(setting);
	}
public:
	auto baseTick(void) -> void;
public:
	virtual void onBaseTick(Actor*, Level*, GameMode*) {}; //for now we can use onBaseTick better than thrift onTick fr
	virtual auto onTick(void) -> void {};
	virtual auto onEnable(void) -> void {};
	virtual auto onDisable(void) -> void {};
public:
	virtual auto onImGuiRender() -> void {};
	virtual auto onImGuiOptions(void) -> void {};
	virtual auto onRender(MinecraftUIRenderContext*) -> void {};
	virtual auto onDrawText(MinecraftUIRenderContext*, Font*, float*, std::string*, float*, float, unsigned int, const TextMeasureData*, const CaretMeasureData*) -> void {};
public:
	virtual auto onKey(uint64_t, bool, bool*) -> void {};
	virtual auto onMouse(MouseAction, bool isDown, Vec2<short>, bool*) -> void {};
public:
	virtual auto onPacket(LoopbackPacketSender*, Packet*, bool*) -> void {};
};