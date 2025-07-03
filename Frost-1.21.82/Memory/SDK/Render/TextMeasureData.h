#pragma once

struct TextMeasureData
{ // Stores information about how the text should be displayed
public:
    TextMeasureData(float size, bool showShadow)
    {
        this->textSize = size;
        this->showShadow = showShadow;
        this->linePadding = 0;
        this->inObject = false;
    };

public:
    float textSize;
    int linePadding;
    bool showShadow;
    bool inObject;
    bool hideHyphen;
};