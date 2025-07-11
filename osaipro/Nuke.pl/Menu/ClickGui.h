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

    Vec4 selectableSurface = Vec4(pos.x, pos.y, pos.x + size.x, pos.y + size.y);

	bool isExtended = true;
	bool isInAnimation = false;
	float animation = 0;
	const char* name;
	std::map<unsigned int, std::shared_ptr<ClickModule>> moduleMap;

	int yOffset = 0;
};
// In Particle21 class definition:
class Particle21 {
public:
	Vec2 startPos;
	Vec2 endPos;
	float size;
	float speed;
	float duration;
	float rotationAngle;
	int shapeType;  // 0 = square, 1 = circle, 2 = triangle

	// Constructor
	Particle21(Vec2 startPos, Vec2 endPos, float size, float speed, float duration, float rotationAngle, int shapeType)
		: startPos(startPos), endPos(endPos), size(size), speed(speed), duration(duration), rotationAngle(rotationAngle), shapeType(shapeType) {
	}
};



// Assume particles is part of a class, e.g., ClickGuiMod or similar
extern std::vector<std::shared_ptr<Particle21>> particles;

// Define the `drawList` as an ImDrawList pointer
//ImDrawList* drawList;  // Assume this is set somewhere in your code
class ClickGui {
private:
	inline static std::shared_ptr<ClickWindow> getWindow(const char* id);
	inline static std::shared_ptr<ClickModule> getClickModule(std::shared_ptr<ClickWindow> window, const char* id);

	static void renderLabel(const char* text);
	static void renderTooltip(std::string* text);

	static void renderCategory(Category category);
	inline static void getModuleListByCategory(Category category, std::vector<std::shared_ptr<IModule>>* modList);
public:
	static void onTick(GameMode* gm);
	static void init();
	static void render();
	static void onKeyPressed(int key, bool isDown, bool shouldCancel);
	static void onMouseClickUpdate(int key, bool isDown);
	static void onWheelScroll(bool direction);  // true = up, false = down
	static void onLoadConfig(void* confVoid);
	static void onSaveConfig(void* confVoid);
	static void setScale(float newScale);
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
				categoryName = "Misc";
				break;
			case Category::CUSTOM:
				categoryName = "Scripts";
				break;
			case Category::CLIENT:
				categoryName = "HUD";
				break;
			}
		}
		return categoryName;
	}
};
