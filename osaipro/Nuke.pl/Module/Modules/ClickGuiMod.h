#pragma once

#include "Module.h"

class ClickGuiMod : public IModule {
public:
	ClickGuiMod();
	~ClickGuiMod();

	bool showTooltips = true;
	float scale = 0.8f;
	float opacity = 0.8f;
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onDisable() override;
	virtual void onEnable() override;
	virtual bool allowAutoStart() override;
	virtual void onLoadConfig(void* conf) override;
	bool showParticles = false;
	virtual void onSaveConfig(void* conf) override;
	virtual void onImGuiRender(ImDrawList* d) override;
	/*
	static inline float percent = 0.f;
	SettingEnum guiMode;
	SettingEnum tooltipPos;
	bool simpleGUI = true;
	static inline bool canScroll = false;
	static inline bool settingcanScroll = false;
	static inline bool bottom = false;
	static inline bool keybind = true;
	static inline float Hoverpercent = 0.f;
	static inline bool issettingOpen = false;
	static inline float scrollDirection = 0.f;
	static inline float settingscrollDirection = 0.f;
	static inline bool shouldToggleLeftClick = false;
	static inline bool shouldToggleRightClick = false;
	static inline bool isLeftClickDown = false;
	static inline bool isRightClickDown = false;

	static void onMouseClickUpdate(int key, bool isDown);
	static void onWheelScroll(bool direction);
	virtual void onPostRender(MinecraftUIRenderContext* renderCtx) override;
	virtual void onImGuiRender(ImDrawList* d) override;*/
};
