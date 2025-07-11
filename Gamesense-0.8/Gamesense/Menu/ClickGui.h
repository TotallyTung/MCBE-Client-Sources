
#pragma once

#include <map>
#include <vector>

#include "../../Memory/GameData.h"
#include "../../utils/DrawUtils.h"
#include "../../Utils/GuiUtils.h"
#include "../Module/ModuleManager.h"

struct ClickModule {
	bool isExtended = false;
};

struct ClickWindow {
	ClickWindow() {
		pos.x = 0;
		pos.y = 0;

		size.x = 30;
		size.y = 30;
	}
	vec2_t pos;
	vec2_t size;

	vec4_t selectableSurface = vec4_t(pos.x, pos.y, pos.x + size.x, pos.y + size.y);

	bool isExtended = true;
	bool isInAnimation = false;
	float animation = 0;
	const char* name;
	std::map<unsigned int, std::shared_ptr<ClickModule>> moduleMap;

	int yOffset = 0;
};

class ClickGui {
private:
	inline static std::shared_ptr<ClickWindow> getWindow(const char* id);
	inline static std::shared_ptr<ClickModule> getClickModule(std::shared_ptr<ClickWindow> window, const char* id);

	static void renderLabel(const char* text);
	static void renderTooltip(std::string* text);

	static void renderCategory(Category category);
	inline static void getModuleListByCategory(Category category, std::vector<std::shared_ptr<IModule>>* modList);

	inline static const char* catToName(Category cat) {
		const char* categoryName;

		// Get Category Name
		{
			switch (cat) {
			case Category::COMBAT:
				categoryName = "Combat";
				break;
			case Category::VISUAL:
				categoryName = "Render";
				break;
			case Category::MOVEMENT:
				categoryName = "Movement";
				break;
			case Category::PLAYER:
				categoryName = "Player";
				break;
			case Category::EXPLOIT:
				categoryName = "Misc";
				break;
			case Category::OTHER:
				categoryName = "Client";
				break;
			case Category::UNUSED:
				categoryName = "";
				break;
			case Category::CUSTOM:
				categoryName = "Scripts";
				break;
			}
		}
		return categoryName;
	}

public:
	static void init();
	static void render();
	static void imguirender(ImDrawList* d);
	static void onKeyUpdate(int key, bool isDown);
	static void onMouseClickUpdate(int key, bool isDown);
	static void onWheelScroll(bool direction);  // true = up, false = down
	static void onLoadConfig(void* confVoid);
	static void onSaveConfig(void* confVoid);
	static void setScale(float newScale);
};
