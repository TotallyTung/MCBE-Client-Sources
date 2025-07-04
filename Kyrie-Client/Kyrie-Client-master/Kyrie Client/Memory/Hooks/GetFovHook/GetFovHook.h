#pragma once

class GetFovHook {
private:
	static float getFov(__int64 a1, float a2, char a3);
public:
	static void init();
};