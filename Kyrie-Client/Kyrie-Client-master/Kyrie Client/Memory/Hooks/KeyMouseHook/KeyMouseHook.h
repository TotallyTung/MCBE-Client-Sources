#pragma once

class KeyMouseHook {
private:
	static __int64 KeyMouseCallBack(__int64 a1, char mouseButton, char isDown, __int16 mouseX, __int16 mouseY, __int16 relativeMovementX, __int16 relativeMovementY, char a8);
public:
	static void init();
};