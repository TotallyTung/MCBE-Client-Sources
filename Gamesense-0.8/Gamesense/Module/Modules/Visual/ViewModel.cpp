#include "ViewModel.h"


ViewModel::ViewModel() : IModule(0, Category::VISUAL, "Adjust hand view model") {
	registerEnumSetting("Mode", &mode, 0);
	mode.addEntry("Default", 0);
	mode.addEntry("Gamesense", 1);
	registerBoolSetting("Reset", &Reset, Reset);
	registerBoolSetting("Translate", &translate, translate);
	registerBoolSetting("Scale", &scale, scale);
	registerBoolSetting("Rotate", &rotate, rotate);

	registerFloatSetting("TransX", &xPos, 0.f, -2.f, 2.f);
	registerFloatSetting("TransY", &yPos, 0.f, -2.f, 2.f);
	registerFloatSetting("TransZ", &zPos, 0.f, -2.f, 2.f);

	registerFloatSetting("ScaleX", &xScale, 1.f, 0.f, 2.f);
	registerFloatSetting("ScaleY", &yScale, 1.f, 0.f, 2.f);
	registerFloatSetting("ScaleZ", &zScale, 1.f, 0.f, 2.f);

	registerFloatSetting("RotateX", &xRotate, 1.f, 0.f, 2.f);
	registerFloatSetting("RotateY", &yRotate, 1.f, 0.f, 2.f);
	registerFloatSetting("RotateZ", &zRotate, 1.f, 0.f, 2.f);
}

ViewModel::~ViewModel() {
}

const char* ViewModel::getModuleName() {
	return "ViewModel";
}

void ViewModel::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr)
		return;

	if (Reset) {
		xPos, yPos, zPos = 1.f;
		xScale, yScale, zScale = 1.f;
		xRotate, yRotate, zRotate = 1.f;
		Reset = false;
	}
}

