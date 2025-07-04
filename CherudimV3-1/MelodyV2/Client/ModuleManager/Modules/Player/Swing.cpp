#include "Swing.h"

Swing::Swing() : Module("Swing", "Modify your swings speed.", Category::PLAYER) {
	addSlider<int>("Speed", "NULL", ValueType::INT_T, &swingSpeed, 1, 20);
}