#include "Arraylist.h"
ArrayList::ArrayList() : IModule(0, Category::HUD, "ArrayList") {
	//registerEnumSetting("Modes", &Modes, 0);
		//Modes = SettingEnum(this)
	//.addEntry(EnumEntry("Top", 0))
	//.addEntry(EnumEntry("Bottom", 1));

	registerBoolSetting("Bottom", &this->bottom, this->bottom);
	//registerBoolSetting("RGBAback", &this->Backgroundrgba, this->Backgroundrgba);
	registerBoolSetting("RGBA", &this->rgba, this->rgba);
	registerBoolSetting("Cool", &this->Cool, this->Cool);
	registerFloatSetting("Scale", &scale, scale, 0.5f, 1.5f);
	registerFloatSetting("Opacity", &this->opacity, this->opacity, 0.f, 1.0f);
	registerBoolSetting("BUG ARRAY", &this->bugs, this->bugs);

}

ArrayList::~ArrayList(){};


const char* ArrayList::getModuleName() {
	return ("ArrayList");
}