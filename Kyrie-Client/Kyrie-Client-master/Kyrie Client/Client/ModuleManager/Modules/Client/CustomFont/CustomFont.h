#pragma once
#include "../../Module.h"

class CustomFont : public Module {
private:
	SettingEnum Fonts = SettingEnum(this);
public:
	bool textShadow = true;

	CustomFont();
	virtual const char* getModuleName() override;
	virtual bool isEnabled() override;
	virtual void onImGuiRender(ImDrawList* d) override;
	virtual void onModifySettingEnum(SettingEnum* se) override;
};