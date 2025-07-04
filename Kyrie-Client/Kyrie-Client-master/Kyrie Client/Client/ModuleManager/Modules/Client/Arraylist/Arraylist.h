#pragma once
#include "../../Module.h"

class Arraylist : public Module {
public:
	Arraylist();
	virtual const char* getModuleName() override;
	virtual void onModifySettingEnum(SettingEnum* se) override;
	virtual void onImGuiRender(ImDrawList* d) override;
};
