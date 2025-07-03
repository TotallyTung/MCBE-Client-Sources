#include "TimerUtil.h"
#include <Windows.h>

unsigned __int64 TimerUtil::getCurrentMs() {
	FILETIME f;
	GetSystemTimeAsFileTime(&f);
	(long long)f.dwHighDateTime;
	unsigned __int64 nano = ((__int64)f.dwHighDateTime << 32LL) + (__int64)f.dwLowDateTime;
	return (nano - 116444736000000000LL) / 10000;
}

float currentMs = TimerUtil::getTime();
float TimerUtil::getTime() {
	std::chrono::high_resolution_clock m_clock;
	return std::chrono::duration_cast<std::chrono::nanoseconds>(m_clock.now().time_since_epoch()).count() / 1000000.f;
}

void TimerUtil::reset() {
	currentMs = getTime();
}

bool TimerUtil::hasTimeElapsed(float sec) {
	if (getTime() - currentMs >= (sec)) {
		reset();
		return true;
	}
	return false;
}