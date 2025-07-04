#include "ViewModel.h"


ViewModel::ViewModel() : IModule(0, Category::VISUAL, "Custom item view model") {
	//registerFloatSetting("float1", &this->float1, 0, -10, 10);
	this->registerBoolSetting("Reset", &this->Reset, this->Reset);
	this->registerBoolSetting("Translate", &this->Translate, this->Translate);
	this->registerBoolSetting("Scale", &this->Scale, this->Scale);
	this->registerBoolSetting("Rotate", &this->Rotate, this->Rotate);

	this->registerFloatSetting("TranslateX", &this->trans.x, trans.x, -2.f, 2.f);
	this->registerFloatSetting("TranslateY", &this->trans.y, trans.y, -2.f, 2.f);
	this->registerFloatSetting("TranslateZ", &this->trans.z, trans.z, -6.f, 2.f);

	this->registerFloatSetting("ScaleX", &this->scale.x, scale.x, -1.f, 2.f);
	this->registerFloatSetting("ScaleY", &this->scale.y, scale.y, 0.f, 2.f);
	this->registerFloatSetting("ScaleZ", &this->scale.z, scale.z, 0.f, 2.f);

	this->registerFloatSetting("RotateX", &this->rotat.x, rotat.x, 0.f, 2.f);
	this->registerFloatSetting("RotateY", &this->rotat.y, rotat.y, 0.f, 2.f);
	this->registerFloatSetting("RotateZ", &this->rotat.z, rotat.z, 0.f, 2.f);
	this->registerFloatSetting("RotatePos", &this->RotatePos, 1.f, 0.f, 6.3f);
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
		scale = vec3_t(1.f, 1.f, 1.f);
		trans = vec3_t(0.f, 0.f, 0.f);
		rotat = vec3_t(0.f, 0.f, 0.f);
		RotatePos = 0.f;
		Reset = false;
	}
}