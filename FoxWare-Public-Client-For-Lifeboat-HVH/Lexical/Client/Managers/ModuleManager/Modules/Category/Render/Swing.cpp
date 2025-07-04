#include "Swing.h"

Swing::Swing() : Module("Swing", "Swing animation", Category::RENDER) {
	registerSetting(new SliderSetting<int>("Speed", "Swing speed", &swingSpeed, swingSpeed, 1, 20));
}