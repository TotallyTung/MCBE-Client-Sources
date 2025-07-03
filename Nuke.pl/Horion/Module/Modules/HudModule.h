#pragma once

#include "Module.h"

#include "../../DrawUtils.h"
#include "../ModuleManager.h"
#include "../../../Utils/ColorUtil.h"
#include "../../../Utils/ImGuiUtils.h"

class HudModule : public IModule {
public:

	float testValYe = 40.f * 1.f;
	float lineWidth2 = 1.f * 1.f;
	Vec2 windowSize = Game.getGuiData()->windowSize;

	float fpsX = 2.5;
	float fpsY = windowSize.y - 50;
	float bpsX = 2.5f;
	float bpsY = windowSize.y - 40;
	float posX = 2.5f;
	float posY = windowSize.y - 60;
	float invX = 100.f;
	float invY = 100.f;
	float xVal = 28.f;

	bool showPos = true;
	bool armorHud = true;
	bool futureColor = false;
	bool inv = false;
	bool wlc = false;
	bool watermark = true;
	bool bps = true;
	bool fps = true;
	bool compass = true;

	float opacity = 0.5;
	int range = 90;

	bool outline = false;

	SettingEnum WelcomeID = this;
	std::string modversion = "1.4";
	std::string type = " Release";
	std::string A9 = modversion + type;
	std::string name;
	HudModule();
	~HudModule();

	std::string playerName = "PlayerName";
	std::string w1 = "Welcome " + playerName;
	std::string w2 = "Welcome to " + Utils::getClientName() + " " + playerName;
	std::string w3 = Utils::getClientName() + " " + playerName;
	std::string w4 = "Xin Chao " + playerName;
	std::string w5 = "Star's blessing to " + playerName;
	std::string w6 = playerName + " on top";
	std::string w7 = "Meow " + playerName;
	// Inherited via IModule

	virtual const char* getModuleName() override;
	virtual void onPostRender(MinecraftUIRenderContext* renderCtx) override;
	virtual void onImGuiRender(ImDrawList* drawlist) override;
	virtual void onPreRender(MinecraftUIRenderContext* renderCtx) override;
	void drawCenteredText(Vec2 pos, std::string text, float size, float textOpacity);
	void drawcText(Vec2 pos, std::string text, float size, float textOpacity);
};
