#include "BlockReach.h"

BlockReach::BlockReach() : Module("BlockReach", "Exteneded reach for placing/breaking blocks.", Category::PLAYER) {
	addSlider<float>("Reach", "NULL", ValueType::FLOAT_T, &blockReach, 3.f, 20.f);
}