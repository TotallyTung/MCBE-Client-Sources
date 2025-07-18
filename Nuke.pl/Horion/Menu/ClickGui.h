#pragma once

#include <map>
#include <vector>

#include "../../Memory/GameData.h"
#include "../DrawUtils.h"
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
	Vec2 pos;
	Vec2 size;
	bool isExtended = true;
	bool isInAnimation = false;
	float animation = 0.1f;
	const char* name;
	std::vector<std::shared_ptr<IModule>> moduleList;
	std::map<unsigned int, std::shared_ptr<ClickModule>> moduleMap;

	int yOffset = 0;
};

class ClickGui {
private:
	inline static std::shared_ptr<ClickWindow> getWindow(const char* id);
	inline static std::shared_ptr<ClickModule> getClickModule(std::shared_ptr<ClickWindow> window, const char* id);

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
			case Category::WORLD:
				categoryName = "World";
				break;
			case Category::MISC:
				categoryName = "Exploit";
				break;
			case Category::CLIENT:
				categoryName = "Hud";
				break;
			}
		}
		return categoryName;
	}

public:
	static void init();
	static void render();
	static void onKeyUpdate(int key, bool isDown);
	static void onWheelScroll(bool direction);  // true = up, false = down
	static void onLoadConfig(void* confVoid);
	static void onSaveConfig(void* confVoid);
};
