#include "ArrayList.h"
#include "../../../Client.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
UIColor secondaryColor = UIColor(255, 255, 255);
int arraymod = 0;
int ymod = 0;
int xmod = 0;
int colormode = 0;
bool reve328097302 = false;
ArrayList::ArrayList() : Module("ArrayList", "Display enabled modules list.", Category::CLIENT) {
    addBoolCheck("Bottom", "Make your arraylist at bottom.", &bottom);
    addBoolCheck("ReversedSorting", "Make your arraylist sorting reversed.", &reve328097302);
    addSlider<float>("Size", "Changes arraylist size.", ValueType::FLOAT_T, &size, 0.5f, 2.5f);
    addSlider<float>("BG Opacity", "The opacity of the background.", ValueType::FLOAT_T, &bgOpacity, 0.f, 1.f);
    addSlider<int>("Y-Modifier", "How far vertically.", ValueType::INT_T, &ymod, 0, 1000);
    addSlider<int>("X-Modifier", "How far horizontally.", ValueType::INT_T, &xmod, 0, 2000);
    addSlider<float>("Spacing", "The padding of the text.", ValueType::FLOAT_T, &spacing, 0.f, 20.f);
    addEnumSetting("Render Mode", "How you want the arraylist to be rendered.", { "A", "B" }, &arraymod);
    addColorPicker("SecondaryColor", "NULL", &secondaryColor);

    srand(static_cast<unsigned int>(time(0))); // Seed for random number generation
}

bool sortByLength(Module* lhs, Module* rhs) {
    float textWidth1 = ImGuiUtils::getTextWidth(lhs->getModuleName());
    if (lhs->getModName() != "NULL") textWidth1 += ImGuiUtils::getTextWidth(std::string(" " + lhs->getModName()));
    float textWidth2 = ImGuiUtils::getTextWidth(rhs->getModuleName());
    if (rhs->getModName() != "NULL") textWidth2 += ImGuiUtils::getTextWidth(std::string(" " + rhs->getModName()));
    if (reve328097302) //right
        return (textWidth1 > textWidth2);
    if (!reve328097302) //left
        return (textWidth1 < textWidth2);
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
    float xPos = io.DisplaySize.x - 1.f - xmod;
    float yPos = 0.f + ymod;
    if (bottom) yPos = io.DisplaySize.y;

    static std::vector<Module*> moduleList = *client->moduleMgr->getModuleList();
    std::sort(moduleList.begin(), moduleList.end(), sortByLength);

    int index = 0;
    for (Module* module : moduleList) {
        std::string moduleName = module->getModuleName();
        std::string modName = module->getModName();

        module->ArrayListDuration = Math::lerp((module->isEnabled() && module->isVisible()) ? 1.f : 0.f, module->ArrayListDuration, 0.2f);

        if (module->ArrayListDuration < 0.01f) continue;
      //  float textWidth = ImGuiUtils::getTextWidth(moduleName, textSize);
        float textWidth = 0.f;
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
    float xPos = io.DisplaySize.x - 1.f - xmod;
    float yPos = 0.f + ymod;
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
        if (modName != "NULL" && xmod <= 999999) textWidth += ImGuiUtils::getTextWidth(std::string(" " + modName), textSize);
        if (modName != "NULL" && xmod >= 999999) textWidth += ImGuiUtils::getTextWidth(std::string(" " + modName), textSize);
       // if (xPos <= 1000)
        Vec2<float> textPos(xPos - (textWidth + textPadding) * module->ArrayListDuration, yPos);
        Vec2<float> textPosMINUS(xPos - (0 + textPadding) * module->ArrayListDuration, yPos);
        if (bottom) textPos.y -= textHeight + textPadding;
        Vec4<float> rectPos(textPos.x - textPadding * 3.f, textPos.y, textPos.x + textWidth + textPadding + 1.f, textPos.y + textHeight + textPadding);
        Vec4<float> rectPosMINUS(textPosMINUS.x - textPadding * 3.f, textPosMINUS.y, textPosMINUS.x + textWidth + textPadding + 1.f, textPosMINUS.y + textHeight + textPadding);

        UIColor textColor = colorsMod->getColor(-index * colorsMod->getSeperationValue());

        if (arraymod == 0 && xPos >= 1000) { // Boost style
            ImGuiUtils::fillRectangle(rectPos, UIColor(0, 0, 0, int(255.f * bgOpacity)));
            ImGuiUtils::drawRectangle(rectPos, textColor, bgOpacity);
            ImGuiUtils::drawText(textPos, moduleName, textColor, textSize);
            if (modName != "NULL" && xmod <= 999999) ImGuiUtils::drawText(Vec2<float>(textPos.x + ImGuiUtils::getTextWidth(std::string(moduleName + " "), textSize), textPos.y), modName, UIColor(secondaryColor), textSize);
        }
        else if (arraymod == 1 && xPos >= 1000) { // Tony style
            // Draw glowing rectangle with moving particles
            ImGuiUtils::drawText(textPos, moduleName, textColor, textSize);
            if (modName != "NULL" && xmod <= 999999) ImGuiUtils::drawText(Vec2<float>(textPos.x + ImGuiUtils::getTextWidth(std::string(moduleName + " "), textSize), textPos.y), modName, UIColor(secondaryColor), textSize);
        }
        if (arraymod == 0 && xPos <= 1000) { // Boost style
            ImGuiUtils::fillRectangle(rectPosMINUS, UIColor(0, 0, 0, int(255.f * bgOpacity)));
            ImGuiUtils::drawRectangle(rectPosMINUS, textColor, bgOpacity);
            ImGuiUtils::drawText(textPosMINUS, moduleName, textColor, textSize);
            if (modName != "NULL" && xmod <= 999999) ImGuiUtils::drawText(Vec2<float>(textPosMINUS.x + ImGuiUtils::getTextWidth(std::string(moduleName + " "), textSize), textPosMINUS.y), modName, UIColor(secondaryColor), textSize);
        }
        else if (arraymod == 1 && xPos <= 1000) { // Tony style
            // Draw glowing rectangle with moving particles
            ImGuiUtils::drawText(textPosMINUS, moduleName, textColor, textSize);
            if (modName != "NULL" && xmod <= 999999) ImGuiUtils::drawText(Vec2<float>(textPosMINUS.x + ImGuiUtils::getTextWidth(std::string(moduleName + " "), textSize), textPosMINUS.y), modName, UIColor(secondaryColor), textSize);
        }

        ++index;
        if (bottom) yPos -= (textHeight + textPadding) * module->ArrayListDuration;
        else yPos += (textHeight + textPadding) * module->ArrayListDuration;
    }
    ClientInstance* ci = mc.getClientInstance();
    Vec2<float> mousePos = ci->getMousePos();
    handleMouseClick(mousePos);
}
