#pragma once
#include "../Module.h"
#include <chrono>

class HUD : public IModule
{
private:
	int size = 25;
public:
	HUD();
	~HUD();

	vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	float fpsX = 2.5;
	float fpsY = windowSize.y - 40;
	float bpsX = 2.5f;
	float bpsY = windowSize.y - 30;
	float posX = 2.5f;
	float posY = 60;
	float pvpX = 2.5;
	float pvpY = 60;
	float invX = 100.f;
	float invY = 100.f;
	float xVal = 28.f;
	int range = 90;

	float opa = 0.3f;
	bool outline = true;
	bool armorHUD = true;
	bool totemc = false;
	bool playerList = false;
	bool compass = false;
	bool wlc = false;
	bool inv = true;
	bool fps = true;
	bool bps = true;
	bool xyz = true;
	float armorX = 2.5;
	float armorY = 60;

	SettingEnum TextMode = this;

	void drawCenteredText(vec2_t pos, std::string text, float size, float textOpacity = 1);

	std::string name = "";

	std::string message = "Gamesense";
	inline std::string &getCustomWatermarkMessage() { return message; };

	// Inherited via IModule
	virtual void onDisable();
	virtual void onEnable();
	virtual const char *getRawModuleName() override;
	virtual void onPostRender(C_MinecraftUIRenderContext *renderCtx);
	virtual void onPreRender(C_MinecraftUIRenderContext *renderCtx);
	virtual void onImGuiRender(ImDrawList* d);
	virtual bool isThereTotem();
	int pullSlot(int itemIDPog);
	virtual int countItem(int itemIDCount);
};
