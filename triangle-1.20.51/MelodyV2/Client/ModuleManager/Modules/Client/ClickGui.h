#pragma once
#include "../Module.h"
#include "../../../CommandManager/CommandManager.h"
#include "../../../ConfigManager/ConfigManager.h"
void getModuleListByCategory(Category category, std::vector<Module*>& modList);
struct clickGuiParticles {
	Vec2<float> startPos;
	Vec2<float> endPos;
	float speed;
	float size;
	float duration = 1.f;
	float rotation;
	clickGuiParticles(const Vec2<float>& startpos, const Vec2<float>& endpos, const float& size2, const float& speed2, const float& rotation2) {
		this->startPos = startpos;
		this->endPos = endpos;
		this->size = size2;
		this->speed = speed2;
		this->rotation = rotation2;
	};
};

struct networkNode {
	Vec2<float> pos;
	float size;
	float glow;
	float lifeTime;
	networkNode(const Vec2<float>& position, float nodeSize, float glowEffect)
		: pos(position), size(nodeSize), glow(glowEffect), lifeTime(8.f) {
	}
};

struct ClickWindow {
	std::string windowName;
	Vec2<float> pos;
	float scrolloffset = 0.f;
	float yOffset;
	bool extended = true;
	bool isDragging = false;
	Category category;
	std::vector<Module*> modList;

	ClickWindow(const Vec2<float>& Pos, const Category& c) {
		this->pos = Pos;
		this->extended = true;
		if (c == Category::COMBAT) this->windowName = "Combat";
		else if (c == Category::MISC) this->windowName = "Misc";
		else if (c == Category::RENDER) this->windowName = "Render";
		else if (c == Category::MOVEMENT) this->windowName = "Movement";
		else if (c == Category::PLAYER) this->windowName = "Player";
		else if (c == Category::CLIENT) this->windowName = "Client";
		else if (c == Category::UNKNOW) this->windowName = "Unknow";
		else if (c == Category::WORLD) this->windowName = "World";
		else if (c == Category::EXPLOIT) this->windowName = "Exploit";
		else if (c == Category::UTILITY) this->windowName = "Utility";
		else if (c == Category::PACKETS) this->windowName = "Packets";

		getModuleListByCategory(c, modList);
	}
};

class Gui : public Module {
private:
	std::vector<ClickWindow*> windowList;
	std::vector<std::shared_ptr<clickGuiParticles>> particles;
	std::vector<std::shared_ptr<networkNode>> nodes;
	bool isLeftClickDown = false;
	bool isRightClickDown = false;
	bool isHoldingLeftClick = false;
	bool isHoldingRightClick = false;
	Vec2<float> startDragPos = Vec2<float>(0.f, 0.f);
	float openDuration = 0.f;
	bool initGui = false;
	void init();

	bool tooltips = true;
	ConfigManager* configMgr = nullptr;
	bool isShiftDown = false;
	bool isBackKeyDown = false;
	std::string searchingModule;
	bool isSearching = false;
	int lastKeyPress = -1;
	bool* isChoosingKeyBindPtr = nullptr;
public:
	int Mode = 1;
	float blurStrength = 7.5f;
	Gui();
	~Gui();

	void onMouseUpdate(char mouseButton, bool isDown);
	void onKey(int key, bool isDown);
	void render(ImDrawList* drawlist);


	virtual bool isVisible() override;
	virtual void onEnable() override;
	virtual void onDisable() override;
};