//
// Created by vastrakai on 7/2/2024.
//

#include "Event.hpp"

class WindowResizeEvent : public Event
{
public:
    explicit WindowResizeEvent(int width, int height)
        : mWidth(width), mHeight(height) {}

    [[nodiscard]] int getWidth() const { return mWidth; }
    [[nodiscard]] int getHeight() const { return mHeight; }

    int mWidth, mHeight;
};
