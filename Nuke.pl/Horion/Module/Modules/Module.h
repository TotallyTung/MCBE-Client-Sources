#pragma once

#include "../../../Memory/GameData.h"
#include "../../FriendList/FriendsManager.h"
#include "../../../Utils/keys.h"
//#include "../../DrawUtils.h"

class IModule;

enum class Category {
	COMBAT = 0,
	VISUAL = 1,
	MOVEMENT = 2,
	PLAYER = 3,
	WORLD = 4,
	MISC = 5,
	CLIENT = 6
};

struct EnumEntry {
private:
	/*const */ std::string name;
	/*const */ unsigned char val;

public:
	/// <summary>Use this however you want</summary>
	void* ptr = nullptr;
	EnumEntry(const std::string _name, const unsigned char value);
	std::string GetName();
	unsigned char GetValue();
};

struct AddResult;

class SettingEnum {
private:
	IModule* owner = nullptr;

public:
	std::vector<EnumEntry> Entrys;
	int selected = -1;

	SettingEnum(std::vector<EnumEntry> entr, IModule* mod = nullptr);
	SettingEnum(IModule* mod = nullptr);
	//SettingEnum();
	SettingEnum& addEntry(EnumEntry entr);
	SettingEnum& addEntry(const char* name, int value);
	EnumEntry& GetEntry(int ind);
	EnumEntry& GetSelectedEntry();
	int GetCount();
	int getSelectedValue();
	int SelectNextValue(bool back = false);
};

enum class ValueType {
	FLOAT_T,
	DOUBLE_T,
	INT64_T,
	INT_T,
	BOOL_T,
	TEXT_T,
	ENUM_T
};

struct SettingValue {
	union {
		float _float;
		double _double;
		__int64 int64;
		int _int;
		bool _bool;
		std::string* text;
		SettingEnum* Enum;
	};
};

struct SettingEntry {
	char name[0x20] = "";
	ValueType valueType;
	SettingValue* value = nullptr;
	SettingValue* defaultValue = nullptr;
	SettingValue* minValue = nullptr;
	SettingValue* maxValue = nullptr;
	void* extraData; // Only used by enum for now

	// ClickGui Data
	bool isDragging = false;  // This is incredibly hacky and i wanted to avoid this as much as possible but i want to get this clickgui done

	void makeSureTheValueIsAGoodBoiAndTheUserHasntScrewedWithIt();
};

class IModule {
private:
	bool enabled = false;
	int keybind = 0x0;
	bool visible = true;
	bool extended = false;
	Vec2 ModulePos;
private:
	Category category;
	const char* tooltip;

	std::vector<SettingEntry*> settings;

protected:
	IModule(int key, Category c, const char* tooltip);

	void registerFloatSetting(std::string name, float* floatPtr, float defaultValue, float minValue, float maxValue);
	void registerIntSetting(std::string name, int* intpTr, int defaultValue, int minValue, int maxValue);
	void registerEnumSetting(std::string name, SettingEnum* intPtr, int defaultValue);
	void registerBoolSetting(std::string name, bool* boolPtr, bool defaultValue);
	void registerStringSetting(std::string text);

	void clientMessageF(const char* fmt, ...);

public:
	virtual ~IModule();

	const Category getCategory() { return category; };

	inline Vec2* getModulePos() { return &ModulePos; };

	inline std::vector<SettingEntry*>* getSettings() { return &settings; };

	virtual const char* getModuleName() = 0;
	virtual const char* getRawModuleName();
	virtual const char* getModName();
	virtual int getKeybind();
	virtual void setKeybind(int key);
	virtual bool allowAutoStart();
	virtual void onWorldTick(GameMode*);
	virtual void onTick(GameMode*);
	virtual void onKeyUpdate(int key, bool isDown);
	virtual void onKey(int key, bool isDown, bool& shouldCancel);
	virtual void onEnable();
	virtual void onDisable();
	virtual void onAttack(Entity*);
	virtual void onPreRender(MinecraftUIRenderContext* renderCtx);
	virtual void onPostRender(MinecraftUIRenderContext* renderCtx);
	virtual void onLevelRender();
	virtual void onMove(MoveInputHandler*);
	virtual void onPlayerTick(Player*);
	virtual void onLoadConfig(void* conf);
	virtual void onSaveConfig(void* conf);
	virtual bool isFlashMode();
	virtual void setEnabled(bool enabled);
	virtual void toggle();
	virtual bool isEnabled();
	virtual bool isVisible();
	virtual void onSendPacket(Packet*);
	virtual void onSendClientPacket(Packet*);
	virtual bool callWhenDisabled();
	const char* getTooltip();
	virtual void onBaseTick(Entity*) {}
	virtual void onImGuiRender(ImDrawList* d);
	virtual void onJoiningServer();
};