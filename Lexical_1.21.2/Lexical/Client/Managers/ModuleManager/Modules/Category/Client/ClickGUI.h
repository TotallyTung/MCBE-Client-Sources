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

		// Animation
		float selectedAnim = 0.f;

		std::vector<Module*> moduleList;

		ClickWindow(std::string windowName, Vec2<float> startPos, Category c);
	};

	std::vector<ClickWindow*> windowList;
	float openAnim = 0.0f;
	bool initialized = false;

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
