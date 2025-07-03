#pragma once
//
// Created by Solar on 11/18/2024.
//

#include <Hook/Hook.hpp>
#include <Hook/HookManager.hpp>
#include <SDK/Minecraft/mce.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>

class HoverTextRender
{
public:
    struct HoverTextInfo {
        bool mShow = false;
        std::string mText = "";
        glm::vec2 mPos{ 0,0 };
    };

    static inline HoverTextInfo mInfo;

    static inline int mTimeDisplayed = 0;
};

class HoverTextRenderer
{
public:
    std::byte      padding[48];          // Don't ask for this man it's just some shit string and a weird byte
    std::string    mFilteredContent;     // this+0x30
    glm::vec2      mCursorPosition;      // this+0x50
    glm::vec2      mOffset;              // this+0x58
    glm::vec2      mBoxDimensions;       // this+0x60
};

class HoverTextRendererHook : public Hook {
public:
    HoverTextRendererHook() : Hook() {
        mName = "HoverTextRenderer::render";
    }

    static std::unique_ptr<Detour> mHoverTextRendererRenderDetour;


    static void onHoverTextRendererRender(HoverTextRenderer* _this, void* mRenderContext, ClientInstance* mClient, glm::vec4* mRenderAABB, int mPass);
    void init() override;
};