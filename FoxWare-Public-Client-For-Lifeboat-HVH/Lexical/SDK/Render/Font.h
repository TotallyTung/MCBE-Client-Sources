#pragma once
#include <string>

class Font {
private:
	virtual void destructorFont();
	virtual void unk1();
	virtual void unk2();
	virtual void unk3();
	virtual void unk4();
	virtual void unk5();
public:
	virtual float getLineLength(std::string* str, float textSize, bool showColorSymbol);
	virtual float getLineHeight();
};
