// Created by Tony on 2024-10-10 10:01:54

#include "GlintColor.h"

GlintColor::GlintColor() : Module("GlintColor", "GlintColor.", Category::CLIENT) {
	addSlider<float>("intensity", "NULL", ValueType::FLOAT_T, &intensity, 0.01f, 0.1f);
	addBoolCheck("night", "NULL", &night);
	addBoolCheck("custom", "NULL", &custom);
	addColorPicker("Color", "NULL", &color);
}