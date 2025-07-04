#include "HurtColor.h"

HurtColor::HurtColor() : Module("HurtColor", "Changes hurt color.", Category::RENDER) {
	addColorPicker("Hurt Color", "NULL", &hurtColor);
	addBoolCheck("Fade", "Fade hurt colors.", &fade);
}