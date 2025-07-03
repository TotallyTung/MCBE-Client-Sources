#pragma once
#include <chrono>

namespace TimerUtil {
	unsigned __int64 getCurrentMs();
	float getTime();
	void reset();
	bool hasTimeElapsed(float sec);
}
