#pragma once
//#include "../Utils/DrawUtils.h"
//#include "../Utils/ColorUtil.h"
#include "../ModuleManager.h"
#include "Module.h"

class ArrayListNuke : public IModule {
private:
	SettingEnum mode = this;
	SettingEnum animation = this;

	float spacing = 0.f;
	float scale = 1.f;
	bool bottom = false;
	int opacity = 150;
public:
	bool showkb = true;
	bool focused = false;
	bool invert = false;
	bool modes = true;

	// Positions
	Vec2 windowSize = Game.getClientInstance()->getGuiData()->windowSize;
	float positionX = windowSize.x;
	float positionY = 0.f;

	virtual void onPostRender(MinecraftUIRenderContext* renderCtx);
	virtual const char* getModuleName();
	ArrayListNuke();
};