#include "Arraylist.h"
#include "../../../../../../Utils/TimerUtil.h"
#include "../../../ModuleManager.h"

Arraylist::Arraylist() : Module("Arraylist", "Display module enabled", Category::CLIENT) {
    registerSetting(new BoolSetting("Bottom", "Flip to bottom", &bottom, true));
    registerSetting(new BoolSetting("Show Modes", "Show Modes", &showModes, true));
    registerSetting(new SliderSetting<float>("Spacing X", "Adjust spacing X", &spacing, -3, -5, 5));
    registerSetting(new SliderSetting<float>("Spacing Y", "Adjust spacing Y", &spacing2, -3, -5, 5));
    registerSetting(new SliderSetting<float>("Size", "Control the size of modules", &size, 1.f, 0.1f, 2.0f));
    registerSetting(new BoolSetting("Outline", "NULL", &outline, false));
    registerSetting(new EnumSetting("Outline Mode", "NULL", { "Normal", "SideLine" }, &Enum2, 0));
    registerSetting(new SliderSetting<int>("Opacity", "NULL", &opacity, 130, 0, 255));
    registerSetting(new SliderSetting<int>("Offset", "NULL", &offset, 10, 0, 30));
}

bool Arraylist::sortByLength(Module* lhs, Module* rhs) {
    float textSize = 0.7;
    float textWidth1 = D2D::getTextWidth(lhs->getModuleName(), textSize);
    float textWidth2 = D2D::getTextWidth(rhs->getModuleName(), textSize);
    static Arraylist* arralistMod = ModuleManager::getModule<Arraylist>();
    if (arralistMod->showModes) {
        if (lhs->getModeText() != "NULL")
            textWidth1 += D2D::getTextWidth(" " + lhs->getModeText(), textSize);
        if (rhs->getModeText() != "NULL")
            textWidth2 += D2D::getTextWidth(" " + rhs->getModeText(), textSize);
    }
    std::string keybindText1 = "[" + std::string(KeyNames[lhs->getKeybind()]) + "]";
    std::string keybindText2 = "[" + std::string(KeyNames[rhs->getKeybind()]) + "]";
    if (keybindText1 != "[Unknown]")
        textWidth1 += D2D::getTextWidth(" " + keybindText1, textSize);
    if (keybindText2 != "[Unknown]")
        textWidth2 += D2D::getTextWidth(" " + keybindText2, textSize);
    return (textWidth1 > textWidth2);
}

void Arraylist::onD2DRender() {
    LocalPlayer* localPlayer = Game::getLocalPlayer();
    if (localPlayer == nullptr)
        return;

    Vec2<float> windowSize = Game::clientInstance->guiData->windowSizeReal;
    Vec2<float> windowSizeScaled = Game::clientInstance->guiData->windowSizeScaled;
    float textSize = size;
    float textPaddingX = std::max(1.f, ((float)spacing + 3.f)) * textSize;
    float textPaddingY = (float)spacing2 * textSize;
    float textHeight = D2D::getTextHeight("", textSize);
    float lineWidth = 1.5f * textSize;
    Vec2<float> Pos = Vec2<float>(windowSize.x - (float)offset, (float)offset);
    if (bottom)
        Pos.y = windowSize.y - (textHeight + textPaddingY * 2.f) - (float)offset;

    UIColor modeColor = UIColor(255, 255, 255);
    static std::vector<Module*> moduleList = ModuleManager::moduleList;
    std::sort(moduleList.begin(), moduleList.end(), sortByLength);

    int index = 0;
    UIColor mainColor;
    Vec4<float> lastRect;
    float offScreenPosition = 0.f;

    for (auto& mod : moduleList) {
        std::string moduleName = mod->getModuleName();
        std::string modeText = mod->getModeText();
        std::string keybindText = "[" + std::string(KeyNames[mod->getKeybind()]) + "]";
        bool shouldRender = mod->isEnabled() && mod->isVisible();

        if (!showModes) modeText = "NULL";
        if (keybindText == "[Unknown]") keybindText = "";

        static Colors* colorMod = ModuleManager::getModule<Colors>();
        mainColor = colorMod->getColor(index * colorMod->getSeperationValue());
        modeColor = UIColor(mainColor);

        mod->arraylistAnim = Math::moveTowards(mod->arraylistAnim, shouldRender ? 1.05f : -0.1f, D2D::deltaTime * 10.f);
        mod->arraylistAnim = std::clamp(mod->arraylistAnim, 0.f, 1.f);

        slider = mod->arraylistAnim;

        if (mod->arraylistAnim > 0.f) {
            float fullTextWidth = D2D::getTextWidth(moduleName, textSize);
            if (modeText != "NULL") fullTextWidth += D2D::getTextWidth(" " + modeText, textSize);
            if (!keybindText.empty()) fullTextWidth += D2D::getTextWidth(" " + keybindText, textSize);

            Vec4<float> rectPos = Vec4<float>(
                Pos.x - textPaddingX - fullTextWidth - textPaddingX,
                Pos.y,
                Pos.x,
                Pos.y + textPaddingY + textHeight + textPaddingY
            );

            float animRect = (windowSize.x + 25.f - rectPos.x) * (1.f - mod->arraylistAnim);
            rectPos.x += animRect;
            rectPos.z += animRect;

            float offsetY = 2.0f;
            float offsetX = 0.8f;
            Vec2<float> textPos(rectPos.x + textPaddingX + offsetX, rectPos.y + textPaddingY - offsetY);

            D2D::fillRectangle(rectPos, UIColor(10, 10, 10, opacity));
            D2D::fillRectangle(Vec4<float>(rectPos.x, rectPos.y - 1.0f, rectPos.z, rectPos.w), UIColor(10, 10, 10, opacity));

            if (outline) {
                float increasedLineWidth = lineWidth * 1.7f;
                if (Enum2 == 1) {
                    float sideLineWidth = lineWidth * 1.7f;
                    D2D::fillRectangle(
                        Vec4<float>(
                            rectPos.z,
                            rectPos.y,
                            rectPos.z + sideLineWidth,
                            rectPos.w
                        ),
                        mainColor
                    );
                }

                else { // Horizontal
                    D2D::fillRectangle(Vec4<float>(rectPos.x - increasedLineWidth, rectPos.y, rectPos.x, rectPos.w), mainColor);
                    D2D::fillRectangle(Vec4<float>(rectPos.z, rectPos.y, rectPos.z + increasedLineWidth, rectPos.w), mainColor);

                    if (index == 0) {
                        if (bottom)
                            D2D::fillRectangle(Vec4<float>(rectPos.x - increasedLineWidth, rectPos.w, rectPos.z + increasedLineWidth, rectPos.w + increasedLineWidth), mainColor);
                        else
                            D2D::fillRectangle(Vec4<float>(rectPos.x - increasedLineWidth, rectPos.y - increasedLineWidth, rectPos.z + increasedLineWidth, rectPos.y), mainColor);
                    }
                    else {
                        if (bottom)
                            D2D::fillRectangle(Vec4<float>(lastRect.x - increasedLineWidth, rectPos.w - increasedLineWidth, rectPos.x, rectPos.w), mainColor);
                        else
                            D2D::fillRectangle(Vec4<float>(lastRect.x - increasedLineWidth, rectPos.y, rectPos.x, rectPos.y + increasedLineWidth), mainColor);
                    }
                }
            }

            UIColor grayColor(175, 175, 175, 255);
            D2D::drawText(textPos, moduleName, mainColor, textSize, true);
            textPos.x += D2D::getTextWidth(moduleName, textSize);

            if (modeText != "NULL") {
                textPos.x += D2D::getTextWidth(" ", textSize);
                D2D::drawText(textPos, modeText, grayColor, textSize, true);
                textPos.x += D2D::getTextWidth(modeText, textSize);
            }

            if (!keybindText.empty()) {
                textPos.x += D2D::getTextWidth(" ", textSize);
                D2D::drawText(textPos, keybindText, grayColor, textSize, true);
            }

            lastRect = rectPos;
        }

        float yAnim = mod->arraylistAnim * 979.25f;
        if (yAnim > 1.f) yAnim = 1.f;
        Pos.y += (textHeight + textPaddingY * 2.f) * yAnim * (bottom ? -1.f : 1.f);

        if (shouldRender)
            index++;
    }

    if (Enum2 == 0 && outline) {
        float increasedLineWidth = lineWidth * 1.7f;
        if (bottom)
            D2D::fillRectangle(Vec4<float>(lastRect.x - increasedLineWidth, lastRect.y - increasedLineWidth, lastRect.z + increasedLineWidth, lastRect.y), mainColor);
        else
            D2D::fillRectangle(Vec4<float>(lastRect.x - increasedLineWidth, lastRect.w, lastRect.z + increasedLineWidth, lastRect.w + increasedLineWidth), mainColor);
    }
}
