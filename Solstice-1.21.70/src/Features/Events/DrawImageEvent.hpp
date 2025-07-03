//
// Created by vastrakai on 7/20/2024.
//

#pragma once

#include <Hook/Hook.hpp>
#include <SDK/Minecraft/mce.hpp>

// void* context, mce::TexturePtr* texture, glm::vec2* pos, glm::vec2* size, glm::vec2* uv, mce::Color* color

struct DrawImageEvent : public CancelableEvent {
    void* mContext;
    mce::TexturePtr* mTexture;
    glm::vec2* mPos;
    glm::vec2* mSize;
    glm::vec2* mUV;
    mce::Color* mColor;

    explicit DrawImageEvent(void* context, mce::TexturePtr* texture, glm::vec2* pos, glm::vec2* size, glm::vec2* uv, mce::Color* color) : mContext(context), mTexture(texture), mPos(pos), mSize(size), mUV(uv), mColor(color) {};
};