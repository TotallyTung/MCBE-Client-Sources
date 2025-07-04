#include "Fullbright.h"

Fullbright::Fullbright() : Module("Fullbright", "Modify your brightness/gamma.", Category::RENDER) {
	addSlider<float>("Intensity", "NULL", ValueType::FLOAT_T, &intensity, 0.f, 12.f);
}