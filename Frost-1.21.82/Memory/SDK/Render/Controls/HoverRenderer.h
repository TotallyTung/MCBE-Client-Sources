#pragma once

class HoverTextRenderer
{
public:
    std::byte      padding[48];
    std::string    mFilteredContent;     // this+0x30
    Vector2<float> mCursorPosition;      // this+0x50
    Vector2<float> mOffset;              // this+0x58
    Vector2<float> mBoxDimensions;       // this+0x60
};