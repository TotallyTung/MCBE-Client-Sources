// Created by Tony on 2024-10-10 10:01:54

#include "ArrayList.h"
#include "../../../Client.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>

int arraymod = 0;

ArrayList::ArrayList() : Module("ArrayList", "Display enabled modules list.", Category::CLIENT) {
    addBoolCheck("Bottom", "Make your arraylist at bottom.", &bottom);
    addSlider<float>("Size", "Changes arraylist size.", ValueType::FLOAT_T, &size, 0.5f, 1.5f);
    addSlider<float>("BG Opacity", "Changes background opacitys.", ValueType::FLOAT_T, &bgOpacity, 0.f, 1.f);
    addSlider<float>("Spacing", "NULL", ValueType::FLOAT_T, &spacing, 0.f, 5.f);
    addEnumSetting("Render Mode", "can't u just read", { "Melody", "Tony" }, &arraymod);

    srand(static_cast<unsigned int>(time(0))); // Seed for random number generation
}

bool sortByLength(Module* lhs, Module* rhs) {
    float textWidth1 = ImGuiUtils::getTextWidth(lhs->getModuleName());
    if (lhs->getModName() != "NULL") textWidth1 += ImGuiUtils::getTextWidth(std::string(" " + lhs->getModName()));
    float textWidth2 = ImGuiUtils::getTextWidth(rhs->getModuleName());
    if (rhs->getModName() != "NULL") textWidth2 += ImGuiUtils::getTextWidth(std::string(" " + rhs->getModName()));

    return (textWidth1 > textWidth2);
}

Particle::Particle(float startX, float startY) : x(startX), y(startY) {
    vx = (rand() % 100 - 50) / 100.0f; // Random velocity between -0.5 and 0.5
    vy = (rand() % 100 - 50) / 100.0f;
}

void Particle::update(const Vec4<float>& rectPos) {
    x += vx;
    y += vy;

    // Bounce off the rectangle bounds
    if (x <= rectPos.x || x >= rectPos.z) vx = -vx;
    if (y <= rectPos.y || y >= rectPos.w) vy = -vy;
}

void ArrayList::drawGlowingRectangle(ImDrawList* drawlist, const Vec4<float>& rectPos, const UIColor& color, float opacity) {
    ImVec4 glowColor = ImVec4(color.r / 255.f, color.g / 255.f, color.b / 255.f, opacity);

    // Draw the glowing rectangle
    drawlist->AddRect(ImVec2(rectPos.x, rectPos.y), ImVec2(rectPos.z, rectPos.w), ImColor(glowColor), 0.0f, ImDrawCornerFlags_All, 2.0f);

    // Initialize particles if not already done
    if (particles.empty()) {
        for (int i = 0; i < 3; ++i) {
            float particleX = rectPos.x + (rand() % int(rectPos.z - rectPos.x));
            float particleY = rectPos.y + (rand() % int(rectPos.w - rectPos.y));
            particles.emplace_back(particleX, particleY);
        }
    }

    // Update and draw particles
    for (Particle& particle : particles) {
        particle.update(rectPos);
        float particleSize = 2.0f;
        drawlist->AddCircleFilled(ImVec2(particle.x, particle.y), particleSize, ImColor(glowColor));
    }
}

void ArrayList::handleMouseClick(const Vec2<float>& mousePos) {
    float textSize = 1.f * size;
    float textPadding = 1.f * textSize;
    float textHeight = ImGuiUtils::getTextHeight(textSize) - 2.f; // riel textHeight
    textHeight += spacing;
    ImGuiIO& io = ImGui::GetIO();
    float xPos = io.DisplaySize.x - 1.f;
    float yPos = 0.f;
    if (bottom) yPos = io.DisplaySize.y;

    static std::vector<Module*> moduleList = *client->moduleMgr->getModuleList();
    std::sort(moduleList.begin(), moduleList.end(), sortByLength);

    int index = 0;
    for (Module* module : moduleList) {
        std::string moduleName = module->getModuleName();
        std::string modName = module->getModName();

        module->ArrayListDuration = Math::lerp((module->isEnabled() && module->isVisible()) ? 1.f : 0.f, module->ArrayListDuration, 0.2f);

        if (module->ArrayListDuration < 0.01f) continue;
        float textWidth = ImGuiUtils::getTextWidth(moduleName, textSize);
        if (modName != "NULL") textWidth += ImGuiUtils::getTextWidth(std::string(" " + modName), textSize);

        Vec2<float> textPos(xPos - (textWidth + textPadding) * module->ArrayListDuration, yPos);
        if (bottom) textPos.y -= textHeight + textPadding;
        Vec4<float> rectPos(textPos.x - textPadding * 3.f, textPos.y, textPos.x + textWidth + textPadding + 1.f, textPos.y + textHeight + textPadding);

        if (mousePos.x >= rectPos.x && mousePos.x <= rectPos.z && mousePos.y >= rectPos.y && mousePos.y <= rectPos.w) {
            if (mc.isRightClickDown()) {
                module->setEnabled(!module->isEnabled());
                break;
            }
        }

        ++index;
        if (bottom) yPos -= (textHeight + textPadding) * module->ArrayListDuration;
        else yPos += (textHeight + textPadding) * module->ArrayListDuration;
    }
}

void ArrayList::onImGuiRender(ImDrawList* drawlist) {
    float textSize = 1.f * size;
    float textPadding = 1.f * textSize;
    float textHeight = ImGuiUtils::getTextHeight(textSize) - 2.f; // riel textHeight
    textHeight += spacing;
    ImGuiIO& io = ImGui::GetIO();
    float xPos = io.DisplaySize.x - 1.f;
    float yPos = 0.f;
    if (bottom) yPos = io.DisplaySize.y;

    static std::vector<Module*> moduleList = *client->moduleMgr->getModuleList();
    std::sort(moduleList.begin(), moduleList.end(), sortByLength);

    static Colors* colorsMod = (Colors*)client->moduleMgr->getModule("Colors");

    int index = 0;
    for (Module* module : moduleList) {
        std::string moduleName = module->getModuleName();
        std::string modName = module->getModName();

        module->ArrayListDuration = Math::lerp((module->isEnabled() && module->isVisible()) ? 1.f : 0.f, module->ArrayListDuration, 0.2f);

        if (module->ArrayListDuration < 0.01f) continue;
        float textWidth = ImGuiUtils::getTextWidth(moduleName, textSize);
        if (modName != "NULL") textWidth += ImGuiUtils::getTextWidth(std::string(" " + modName), textSize);

        Vec2<float> textPos(xPos - (textWidth + textPadding) * module->ArrayListDuration, yPos);
        if (bottom) textPos.y -= textHeight + textPadding;
        Vec4<float> rectPos(textPos.x - textPadding * 3.f, textPos.y, textPos.x + textWidth + textPadding + 1.f, textPos.y + textHeight + textPadding);

        UIColor textColor = colorsMod->getColor(-index * colorsMod->getSeperationValue());

        if (arraymod == 0) { // Melody style
            ImGuiUtils::fillRectangle(rectPos, UIColor(0, 0, 0, int(255.f * bgOpacity)));
            ImGuiUtils::drawText(textPos, moduleName, textColor, textSize);
            if (modName != "NULL") ImGuiUtils::drawText(Vec2<float>(textPos.x + ImGuiUtils::getTextWidth(std::string(moduleName + " "), textSize), textPos.y), modName, UIColor(255, 255, 255, 255), textSize);
        }
        else if (arraymod == 1) { // Tony style
            // Draw glowing rectangle with moving particles
            drawGlowingRectangle(drawlist, rectPos, textColor, bgOpacity);
            ImGuiUtils::drawText(textPos, moduleName, textColor, textSize);
            if (modName != "NULL") ImGuiUtils::drawText(Vec2<float>(textPos.x + ImGuiUtils::getTextWidth(std::string(moduleName + " "), textSize), textPos.y), modName, UIColor(255, 255, 255, 255), textSize);
        }

        ++index;
        if (bottom) yPos -= (textHeight + textPadding) * module->ArrayListDuration;
        else yPos += (textHeight + textPadding) * module->ArrayListDuration;
    }
    ClientInstance* ci = mc.getClientInstance();
    Vec2<float> mousePos = ci->getMousePos();
    handleMouseClick(mousePos);
}
