// Created by Tony on 2024-10-10 10:01:54

#include "CustomFov.h"

CustomFov::CustomFov() : Module("CustomFov", "Modify ur Fov", Category::RENDER) {
	addSlider<float>("Fov", "NULL", ValueType::FLOAT_T, &fov, 10.f, 170.f);
}