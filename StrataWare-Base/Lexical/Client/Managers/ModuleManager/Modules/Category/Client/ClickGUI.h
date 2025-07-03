#pragma once
#include "../../ModuleBase/Module.h"
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

class ClickGUI : public Module {
private:
	struct ClickWindow {
		std::string name;
		Category category;
		Vec2<float> pos;
		bool extended = true;
		float animProgress = 0.f;
		float targetHeight = 0.f;
		// Animation
		float selectedAnim = 0.f;
		Category currentCategory = Category::COMBAT;
		std::vector<Module*> moduleList;

		ClickWindow(std::string windowName, Vec2<float> startPos, Category c);
	};

	std::vector<ClickWindow*> windowList;
	float openAnim = 0.0f;
	bool initialized = false;
	bool blueWINDOWH;
	float alphahhh;
	float alphahhhb;
	Vec2<float> startPos = Vec2<float>(25.f, 35.f);

	UIColor disabledColor = (0, 0, 0);
	Vec2<float> mousePos = Vec2<float>(0.f, 0.f);
	Vec2<float> mouseDelta = Vec2<float>(0.f, 0.f);

	bool isLeftClickDown = false;
	bool isHoldingLeftClick = false;
	bool isRightClickDown = false;
	bool isHoldingRightClick = false;

	ClickWindow* draggingWindowPtr = nullptr;

	KeybindSetting* capturingKbSettingPtr = nullptr;
	std::vector<std::shared_ptr<clickGuiParticles>> circleList;
	SliderSettingBase* draggingSliderSettingPtr = nullptr;

	void updateSelectedAnimRect(Vec4<float>& rect, float& anim);
private:
	UIColor mainColor = UIColor(0, 0, 255, 255);	// 135
	float blurStrength = 5.f;
	bool showDescription = true;
public:
	ClickGUI();
	~ClickGUI();

	void onDisable() override;
	void onEnable() override;
	bool isVisible() override;
	void onKeyUpdate(int key, bool isDown) override;
	void onMouseUpdate(Vec2<float> mousePos, char mouseButton, char isDown);	// Don't need override

	void InitClickGUI();
	void Render();

	void onLoadConfig(void* conf) override;
	void onSaveConfig(void* conf) override;
};
