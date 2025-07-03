#pragma once
#include <string>
#include <vector>
#include "../../../../../SDK/Game.h"
#include "../../../../../Renderer/D2D.h"
#include "../../../../../Renderer/MCR.h"
#include "../../../../../Utils/TimerUtil.h"
#include "../../../../../Utils/Minecraft/InventoryUtil.h"
#include "../../../../../Utils/Minecraft/PlayerUtil.h"
#include "../../../../../Utils/Minecraft/TargetUtil.h"
#include "../../../../../Utils/Minecraft/WorldUtil.h"
#include "../../../../../Libs/json.hpp"

#include "Settings/Setting.h"
#include "Settings/ToggleSetting.h"
#include "Settings/ColorSetting.h"
#include "Settings/EnumSetting.h"
#include "Settings/KeybindSetting.h"
#include "Settings/NumberSetting.h"

enum class Category {
	COMBAT = 0,
	MOVEMENT = 1,
	RENDER = 2,
	PLAYER = 3,
	WORLD = 4,
	MISC = 5,
	CLIENT = 6
};

class Module {
private:
	std::string name;
	std::string description;
	Category category;
	bool enabled = false;
	bool visible = true;
	int keybind = 0x0;
	int toggleMode = 0;

	std::vector<Setting*> settings;
public:
	//Arraylist stuff
	float arraylistAnim = 0.f;

	//ClickGUI stuff
	float selectedAnim = 0.f;
	bool extended = false;
protected:
	inline Setting* registerSetting(Setting* setting) {
		this->settings.push_back(setting);
		return setting;
	}
public:
	Module(std::string moduleName, std::string des, Category c, int k = 0x0);
	~Module();

	inline std::string getModuleName() {
		return this->name;
	}

	inline std::string getDescription() {
		return this->description;
	}

	inline Category getCategory() { 
		return this->category;
	}

	inline std::vector<Setting*>& getSettingList() {
		return this->settings;
	}
public:
	virtual std::string getModeText();
	virtual bool isEnabled();
	virtual bool isVisible();
	virtual bool isHoldMode();
	virtual int getKeybind();
	virtual void setKeybind(int key);
	virtual bool runOnBackground();
	virtual void setEnabled(bool enable);
	virtual void toggle();
	virtual void onDisable();
	virtual void onEnable();
	virtual void onKeyUpdate(int key, bool isDown);
	virtual void onPlayerTick();
	virtual void onNormalTick(LocalPlayer* localPlayer);
	virtual void onWorldTick(Level* level);
	virtual void onUpdateRotation(LocalPlayer* localPlayer);
	virtual bool onSendPacket(Packet* packet);
	virtual bool onSendDelayedPacket(Packet* packet);
	virtual void onD2DRender();
	virtual void onMCRender(MinecraftUIRenderContext* renderCtx);
	virtual void onLevelRender();
	virtual void onLoadConfig(void* conf);
	virtual void onSaveConfig(void* conf);
	virtual void onMatrixRender(glm::mat4* matrix);
};
