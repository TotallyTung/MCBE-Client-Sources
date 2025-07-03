// Created by Tony on 2024-10-10 10:01:54

#include "Fullbright.h"

Fullbright::Fullbright() : Module("Fullbright", "Modify your brightness/gamma.", Category::RENDER) {
	addSlider<float>("Intensity", "NULL", ValueType::FLOAT_T, &intensity, 0.f, 12.f);
}