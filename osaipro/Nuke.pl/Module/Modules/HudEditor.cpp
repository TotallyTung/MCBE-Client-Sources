 #include "HudEditor.h"
#include "../../Menu/ClickGui.h"
#include "../../Library/ImGUI/imgui_internal.h"
HudEditor::HudEditor() : IModule(VK_INSERT, Category::CLIENT, "The clickgui - toggle everything just by clicking on it!") {
}
HudEditor::~HudEditor() {
}
const char* HudEditor::getModuleName() {
	return ("HudEditor");
}
void HudEditor::onEnable() {
	Game.getClientInstance()->releaseMouse();
}
void HudEditor::onDisable() {
	Game.getClientInstance()->grabMouse();
}

void HudEditor::onImGuiRender(ImDrawList* d) {
}
#include <Windows.h> // Ensure this is included for GetAsyncKeyState
void HudEditor::onPostRender(MinecraftUIRenderContext* renderCtx) {
    // --- Exit Button --- //
    Vec2 currentMousePos = *Game.getClientInstance()->getMousePos();
    Vec2 scaledMousePos = { currentMousePos.x / 2.0f, currentMousePos.y / 2.0f };
    bool leftMouseDown = GameData::isLeftClickDown();
    if (moduleMgr->getModule<HudEditor>()->isEnabled()) {
        ImGuiIO& io = ImGui::GetIO();
        float screenWidth = io.DisplaySize.x;
        float screenHeight = io.DisplaySize.y;
        Vec2 buttonSize = { 100.0f, 50.0f };
        Vec2 buttonPos;
        buttonPos.x = (screenWidth / 2 - buttonSize.x);
        buttonPos.y = (screenHeight / 2 - buttonSize.y);
        Vec4 buttonRect(buttonPos.x, buttonPos.y, buttonPos.x + buttonSize.x, buttonPos.y + buttonSize.y);
        DrawUtils::fillRectangle(buttonRect, MC_Color(0.2f, 0.2f, 0.2f), 0.9f);
        DrawUtils::drawRectangle(buttonRect, MC_Color(1.f, 1.f, 1.f), 2.f, 1.f);
        std::string exitText = "Exit";
        float exitTextWidth = DrawUtils::getTextWidth(&exitText, 1.0f);
        float exitTextHeight = DrawUtils::getFontHeight(1.0f);
        Vec2 exitTextPos;
        exitTextPos.x = buttonPos.x + (buttonSize.x - exitTextWidth) * 0.5f;
        exitTextPos.y = buttonPos.y + (buttonSize.y - exitTextHeight) * 0.5f;
        DrawUtils::drawText(exitTextPos, &exitText, MC_Color(1.f, 1.f, 1.f), 1.0f);
        if (leftMouseDown &&
            scaledMousePos.x >= buttonPos.x && scaledMousePos.x <= (buttonPos.x + buttonSize.x) &&
            scaledMousePos.y >= buttonPos.y && scaledMousePos.y <= (buttonPos.y + buttonSize.y)) {
            moduleMgr->getModule<HudEditor>()->setEnabled(false);
        }
    }
}


void HudEditor::onPreRender(MinecraftUIRenderContext* renderCtx) {
}
