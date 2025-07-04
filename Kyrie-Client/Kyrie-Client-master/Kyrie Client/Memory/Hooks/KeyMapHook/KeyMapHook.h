#pragma once

class KeyMapHook {
private:
	static __int64 KeyMapCallBack(unsigned char key, bool isDown);
public:
	static void init();
};
