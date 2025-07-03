#include "HurtColor.h"

HurtColor::HurtColor() : Module("HurtColor", "Changes hurt color.", Category::RENDER) {
	registerSetting(new ColorSetting("Hurt Color", "NULL", &hurtColor, UIColor(255, 0, 0, 175)));
}