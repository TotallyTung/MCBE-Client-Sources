#pragma once
#include "../../Module.h"

void getModuleListByCategory(const Category& c, std::vector<std::shared_ptr<Module>>& modList);

struct clickGuiParticles {
	Vec2<float> startPos;
	Vec2<float> endPos;
	float speed;
	float size;
	float duration = 1.f;
	clickGuiParticles(const Vec2<float>& startpos, const Vec2<float>& endpos, const float& size2, const float& speed2) {
		this->startPos = startpos;
		this->endPos = endpos;
		this->size = size2;
		this->speed = speed2;
	};
};

struct ClickWindow {
	const char* windowName = nullptr;
	bool extended = true;
	float extendedDuration = 0.f;
	bool isDragging = false;
	Category category;
	Vec2<float> pos;
	Vec2<float> posReal = pos;
	std::vector<std::shared_ptr<Module>> modList;
	ClickWindow(const Vec2<float>& Pos,const Category& c) {
		this->pos = Pos;
		this->posReal = Pos;
		this->extended = true;
		if (c == Category::COMBAT) this->windowName = "Combat";
		else if (c == Category::MISC) this->windowName = "Misc";
		else if (c == Category::RENDER) this->windowName = "Render";
		else if (c == Category::MOVEMENT) this->windowName = "Movement";
		else if (c == Category::PLAYER) this->windowName = "Player";
		else if (c == Category::CLIENT) this->windowName = "Client";
		//else if (c == Category::UNKNOW) this->windowName = "Unknow";

		getModuleListByCategory(c, modList);
	}
};



class ClickGui : public Module {
private:
	bool initialized = false;
	std::vector<std::shared_ptr<ClickWindow>> windowList;
	std::vector<std::shared_ptr<clickGuiParticles>> particles;
private:
	//Some setting stuff
	Vec2<float> startDragPos = Vec2<float>(0.f, 0.f);
	int lastKeyPressed = 0xFFFFFF;
	bool* isChoosingKeybind = nullptr;
public:
	bool gradientBG = true;
	float blurStrength = 2.5f;
	bool showParticles = true;
	void init();
	void onKeyUpdate(unsigned char key, bool isDown);
	void Render(ImDrawList* d);

	ClickGui();
	~ClickGui();
	virtual const char* getModuleName() override;
	virtual void onEnable() override;
	virtual void onDisable() override;
};
