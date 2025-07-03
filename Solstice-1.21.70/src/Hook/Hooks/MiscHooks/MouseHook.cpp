//
// Created by vastrakai on 6/29/2024.
//

#include "MouseHook.hpp"


#include "Features/Events/MouseEvent.hpp"

struct ImGuiWindow;
std::unique_ptr<Detour> MouseHook::mDetour = nullptr;

void* thisMouse = nullptr;

void MouseHook::onMouse(void* _this, char actionButtonId, int buttonData, short x, short y, short dx, short dy,
    uint8_t forceMotionlessPointer)
{
    auto oFunc = mDetour->getOriginal<&onMouse>();

    thisMouse = _this;

    if (forceMotionlessPointer == 2)
    {
        forceMotionlessPointer = true;
        return oFunc(_this, actionButtonId, buttonData, x, y, dx, dy, forceMotionlessPointer);
    }

    auto holder = nes::make_holder<MouseEvent>(_this, actionButtonId, buttonData, x, y, dx, dy, forceMotionlessPointer);
    gFeatureManager->mDispatcher->trigger(holder);

    mButtonStates[actionButtonId] = buttonData;

    bool cancel = holder->isCancelled();

    if (ImGui::GetCurrentContext() != nullptr) {
        ImGuiIO& io = ImGui::GetIO();

        if (x != 0 && y != 0) {
            io.MousePos = ImVec2(x, y);
        }


        switch (actionButtonId) {
        case 1:
            io.MouseDown[0] = buttonData;
            break;
        case 2:
            io.MouseDown[1] = buttonData;
            break;
        case 3:
            io.MouseDown[2] = buttonData;
            break;
        case 4:
            if(buttonData == 0x78 || buttonData == 0x7F)
            {
                io.AddMouseWheelEvent(0, 0.5); // Scroll up
            }
            else if (buttonData == 0x88 || buttonData == 0x80)
            {
                io.AddMouseWheelEvent(0, -0.5); // Scroll down
            }
            break;
        default:
            break;
        }
        // check if we are hovering over an imgui window and cancel left click
        ImGuiWindow* window = ImGui::GetCurrentContext()->HoveredWindow;

        if (window && io.MouseDown[0] && actionButtonId == 1)
            cancel = true;

        // If the mouse is captured by imgui, we don't want to call the original function
        if (!io.WantCaptureMouse && !cancel)
            return oFunc(_this, actionButtonId, buttonData, x, y, dx, dy, forceMotionlessPointer);

    }
    else if (!cancel) return oFunc(_this, actionButtonId, buttonData, x, y, dx, dy, forceMotionlessPointer);

}

void MouseHook::simulateMouseInput(char actionButtonId, int buttonData, short x, short y, short dx,
    short dy)
{
    if (!thisMouse)
    {
        spdlog::error("MouseHook::simulateMouseInput: thisMouse is nullptr");
        return;
    }

    static auto oFunc = SigManager::MouseDevice_feed;

    MemUtils::callFastcall<void>(oFunc, thisMouse, actionButtonId, buttonData, x, y, dx, dy, 2);
}

void MouseHook::init()
{
    mDetour = std::make_unique<Detour>("MouseDevice::feed", reinterpret_cast<void*>(SigManager::MouseDevice_feed), &MouseHook::onMouse);
}
