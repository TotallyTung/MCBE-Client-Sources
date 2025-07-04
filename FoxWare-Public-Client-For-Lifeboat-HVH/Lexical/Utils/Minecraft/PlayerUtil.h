#pragma once
#include "../../SDK/Game.h"

namespace PlayerUtil {
	bool canPlaceBlock(const BlockPos& blockPos, bool airPlace = false);
	bool tryPlaceBlock(const BlockPos& blockPos, bool airPlace = false, bool packetPlace = false);
}
