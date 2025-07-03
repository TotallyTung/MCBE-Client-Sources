#pragma once
#include <vector>
#include <Features/FeatureManager.hpp>
#include <Features/Modules/Setting.hpp>



//
// Created by Tozic 7/15/2024.
//


class Module;

class ModernGui
{
public:
    struct CategoryPosition
    {
        float x = 0.f, y = 0.f;
        bool isDragging = false, isExtended = true, wasExtended = false;
        float yOffset = 0;
        float scrollEase = 0;
        glm::vec2 dragVelocity = glm::vec2(0, 0);
        glm::vec2 dragAcceleration = glm::vec2(0, 0);
    };

    const float catWidth = 200.f;
    const float catHeight = 30.f;

    const float catGap = 40;
    int lastDragged = -1;
    std::vector<CategoryPosition> catPositions;
    std::shared_ptr<Module> lastMod = nullptr;
    bool isBinding = false;
    bool isBoolSettingBinding = false;
    BoolSetting* lastBoolSetting = nullptr;
    ColorSetting* lastColorSetting = nullptr;
    bool displayColorPicker = false;
    bool resetPosition = false;
    uint64_t lastReset = 0;

    // Colour
    ImColor textColor = ImColor(255, 255, 255);
    ImColor darkBlack = ImColor(24, 24, 24 );
    ImColor mainColor = ImColor(22, 22, 22);
    ImColor grayColor = ImColor(40, 40, 40);
    ImColor enumBackGround = ImColor(30, 30, 30);

    // scaleToPoint
    ImVec4 scaleToPoint(const ImVec4& _this, const ImVec4& point, float amount);
    bool isMouseOver(const ImVec4& rect);
    ImVec4 getCenter(ImVec4& vec);
    bool isMouseOverGuiElement();
    void render(float animation, float inScale, int& scrollDirection, char* h, float blur, float midclickRounding, bool isPressingShift);
    void onWindowResizeEvent(class WindowResizeEvent& event);
};
