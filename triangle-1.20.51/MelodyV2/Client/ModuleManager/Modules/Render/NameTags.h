#pragma once
#include "../Module.h"

class NameTags : public Module {
private:
	float opacity = 0.4f;
	bool Renderlocalplayer = true;
public:
	NameTags();

	virtual void onImGuiRender(ImDrawList* d) override;
};