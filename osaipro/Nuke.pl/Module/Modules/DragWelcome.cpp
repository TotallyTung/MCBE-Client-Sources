#include "DragWelcome.h"
#include "../../Menu/ClickGui.h"
#include "HudEditor.h"
#include "../../Library/ImGUI/imgui_internal.h"
std::string placeholderText;
DragWelcome::DragWelcome() : IModule(VK_INSERT, Category::CLIENT, "hudeditor to move") {
}
DragWelcome::~DragWelcome() {
}
const char* DragWelcome::getModuleName() {
    return ("DragWelcome");
}
void DragWelcome::onEnable() {
}
void DragWelcome::onDisable() {
}

void DragWelcome::onImGuiRender(ImDrawList* d) {
}

#include <Windows.h>
void DragWelcome::onPostRender(MinecraftUIRenderContext* renderCtx) {
    // --- Draggable Welcome --- //
    static bool isDragging = false;
    static Vec2 windowPos = { 100, 100 };
    static Vec2 windowSize = { 300, 150 };
    static Vec2 dragOffset = { 0, 0 };
    Vec2 currentMousePos = *Game.getClientInstance()->getMousePos();
    Vec2 scaledMousePos = { currentMousePos.x / 2.0f, currentMousePos.y / 2.0f };
    bool leftMouseDown = GameData::isLeftClickDown();
        placeholderText = "welcome to osai.pro";
    float textWidth = DrawUtils::getTextWidth(&placeholderText, 1.0f);
    float textHeight = DrawUtils::getFontHeight(1.0f);
    Vec2 textPos;
    textPos.x = windowPos.x;
    textPos.y = windowPos.y - (windowSize.y - textHeight) / 2;
    float dragMargin = 2.0f;
    float dragAreaLeft = textPos.x - dragMargin;
    float dragAreaRight = textPos.x + textWidth + dragMargin;
    float dragAreaTop = textPos.y - dragMargin;
    float dragAreaBot = textPos.y + textHeight + dragMargin;
    if (leftMouseDown) {
        if (!isDragging) {
            if (scaledMousePos.x >= dragAreaLeft && scaledMousePos.x <= dragAreaRight &&
                scaledMousePos.y >= dragAreaTop && scaledMousePos.y <= dragAreaBot) {
                if (moduleMgr->getModule<HudEditor>()->isEnabled()) {
                    isDragging = true;
                    dragOffset.x = scaledMousePos.x - windowPos.x;
                    dragOffset.y = scaledMousePos.y - windowPos.y;
                }
            }
        }
        else {
            windowPos.x = scaledMousePos.x - dragOffset.x;
            windowPos.y = scaledMousePos.y - dragOffset.y;
        }
    }
    else {
        isDragging = false;
    }
    Vec4 rectTopLeftBottomRight = { textPos.x - dragMargin, textPos.y - dragMargin, textPos.x + textWidth + dragMargin, textPos.y + textHeight + dragMargin };
    DrawUtils::fillRectangle(rectTopLeftBottomRight, MC_Color(0.f, 0.f, 0.f), 1.0f); 
    DrawUtils::drawText(textPos, &placeholderText, MC_Color(1.f, 1.f, 1.f), 1.0f);
    DrawUtils::flush();
}

void DragWelcome::onPreRender(MinecraftUIRenderContext* renderCtx) {
}
