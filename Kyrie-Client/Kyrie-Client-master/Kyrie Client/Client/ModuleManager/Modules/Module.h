#pragma once
#include <vector>
#include <memory>
#include <Windows.h>

#include "../../../Utils/RenderUtils.h"
#include "../../../Memory/GameData/GameData.h"

enum class Category {
	COMBAT = 0,
	MISC = 1,
	RENDER = 2,
	MOVEMENT = 3,
	PLAYER = 4,
	CLIENT = 5,
	UNKNOW = 6
};

enum class ValueType {
	FLOAT_T,
	DOUBLE_T,
	INT64_T,
	INT_T,
	BOOL_T,
	TEXT_T,
	ENUM_T,
	KEYBINT_T
};

class Module;

struct EnumEntry {
private:
	/*const */ const char* name;
	/*const */ unsigned char val;

public:
	/// <summary>Use this however you want</summary>
	void* ptr = nullptr;
	EnumEntry(const char* _name, const unsigned char value);
	const char* GetName();
	unsigned char GetValue();
};

class SettingEnum {
private:
	Module* owner = nullptr;
public:
	std::vector<EnumEntry> Entrys;
	int selected = -1;

	SettingEnum(std::vector<EnumEntry> entr, Module* mod = nullptr);
	SettingEnum(Module* mod = nullptr);

	SettingEnum& addEntry(EnumEntry entr);
	SettingEnum& addEntry(const char* name, int value);
	EnumEntry& GetEntry(int ind);
	EnumEntry& GetSelectedEntry();
	int GetCount();
	int getSelectedValue();
	int SelectNextValue(bool back = false);
};

struct SettingValue {
	union {
		float _float;
		double _double;
		__int64 int64;
		int _int;
		bool _bool;
		SettingEnum* Enum;
	};
};

class SettingEntry {
public:
	const char* name;
	const char* description = nullptr;
	ValueType valueType;
	SettingValue* value = nullptr;
	SettingValue* defaultValue = nullptr;
	SettingValue* minValue = nullptr;
	SettingValue* maxValue = nullptr;
	void* extraData; // Only used by enum for now
	// ClickGui stuff
	bool* shouldShow;
	bool isChoosingKeybind = false;
	float timeCounter = 0.f;
	bool isDragging = false;
	float enabledDuration = 0.f;
	float selectedDuration = 0.f;
	// int and float slider
	float PosZ = 0.f;

	~SettingEntry();
	void makeSureTheValueIsAGoodBoiAndTheUserHasntScrewedWithIt(); // LOL this is fully skidded from horon :sob:
};

class Module {
private:
	bool enabled = false;
	bool visible = true;
	SettingEnum toggleMod = SettingEnum(this);
	int keybind = 0x0;
	Category category = Category::UNKNOW;
	const char* description = nullptr;
	std::vector<SettingEntry*> settings;
public:
	//ClickGui stuff lol
	bool extended = false;
	float enabledDuration = 0.f;
	float selectedDuration = 0.f;
	float extendedDuration = 0.f;
	//Arraylist
	float ArrListDuration = 0.f;
protected:
	Module(int k, Category c, const char* d);
	void registerBoolSetting(const char* name, bool* boolPtr, bool defaultValue, const char* description = nullptr, bool* shouldShow = nullptr);
	void registerIntSetting(const char* name, int* intPtr, int defaultValue, int minValue, int maxValue, const char* description = nullptr, bool* shouldShow = nullptr);
	void registerFloatSetting(const char* name, float* floatPtr, float defaultValue, float minValue, float maxValue, const char* description = nullptr, bool* shouldShow = nullptr);
	void registerEnumSetting(const char* name, SettingEnum* intPtr, int defaultValue, const char* description = nullptr, bool* shouldShow = nullptr);
	void registerKeybindSetting(const char* name, int* intPtr, int defaultValue, const char* description = nullptr, bool* shouldShow = nullptr);
public:
	inline const Category& getCategory() { return this->category; };
	inline const char* getDescription() { return this->description; }
	inline const std::vector<SettingEntry*>& getSettings() { return this->settings; }
public:
	virtual ~Module();
	virtual const char* getModuleName();
	virtual const char* getModName();
	virtual int getKeybind();
	virtual bool isEnabled();
	virtual bool isVisible();
	virtual bool isHoldMode();
	virtual void setEnabled(const bool& enabled);
	virtual void setKeybind(const int& newKey);
	virtual void onEnable();
	virtual void onDisable();
	virtual void toggle();
	virtual void onModifySettingEnum(SettingEnum* se);
	virtual void onKeyUpdate(const int& key, const bool& isDown);
	virtual void onImGuiRender(ImDrawList* d);
	virtual void onImGui3DRender(ImDrawList* d, glmatrixf* refdef);
};