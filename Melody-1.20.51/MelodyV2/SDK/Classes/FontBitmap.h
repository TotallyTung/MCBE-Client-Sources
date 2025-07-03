// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include <string>

class FontBitmap {
private:
	virtual void destructorFont();
	virtual void unk1();
	virtual void unk2();
	virtual void unk3();
	virtual void unk4();
	virtual void unk5();

public:
	virtual float getLineLength(std::string* str, float textSize, bool unknown);
	virtual float getLineHeight();
};
