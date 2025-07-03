#pragma once
#include "../../ModuleBase/Module.h"

class Arraylist : public Module {
public:
	bool bottom = true;
private:
	bool showModes = true;
	UIColor color = UIColor(0, 0, 255);
	int modeColorEnum = 0;
	int Enum2 = 0;
	bool outline = false;
	int offset = 15;
	float textSize = 1.f;
	float slider = 1.f;
	float textSize2 = 1.f;
	int opacity = 125;
	float spacing = -1;
	float offScreenPosition = 0.f;
	float spacing2 = -1;
	float size = 1;
	static bool sortByLength(Module* lhs, Module* rhs);
public:
	Arraylist();

	void onD2DRender() override;
};