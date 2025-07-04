#pragma once

class UIProfanityContext
{
private:
	char pad_0000[8]; //0x0000
	char pad_0009[7]; //0x0009
public:
	bool enabled; //0x0008
}; //Size: 0x0010