#pragma once
#include "../Module.h"
class Watermark : public Module {
private:
	float size = 1.f;
public:
	bool Hotbar = false;
	Watermark();
	~Watermark();

	virtual void onNormalTick(Actor* actor) override;
	virtual void onImGuiRender(ImDrawList* drawlist) override;
};