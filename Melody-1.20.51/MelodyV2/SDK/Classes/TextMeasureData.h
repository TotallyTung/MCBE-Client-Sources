// Created by Tony on 2024-10-10 10:01:54

#pragma once

class TextMeasureData {
public:
    TextMeasureData(float size, bool showShadow)
    {
        this->textSize = size;
        this->showShadow = showShadow;
        this->id = 0;
        this->inObject = false;
    };

public:
    float textSize;
    int id;
    bool showShadow;
    bool inObject;
};