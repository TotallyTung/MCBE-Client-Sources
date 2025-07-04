#pragma once
#include "../../../SDK/GameMode.h"
#include <memory>

class GameModeGetPickRangeHook {
private:
	static float getPickRange(GameMode* a1, const enum InputMode* a2, bool a3);
public:
	static void init(uintptr_t** VTable);
};
