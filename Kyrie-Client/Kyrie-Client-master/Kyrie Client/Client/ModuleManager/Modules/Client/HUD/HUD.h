#pragma once
#include "../../Module.h"

class HUD : public Module {
private:
	bool showPosition = true;
	bool showFPS = true;
public:

	HUD();
	virtual const char* getModuleName() override;
	virtual void onImGuiRender(ImDrawList* d) override;
};