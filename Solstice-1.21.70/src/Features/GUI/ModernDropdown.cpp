//
// Created by Tozic on 7/15/2024.
//

#include "ModernDropdown.hpp"
#include <Features/Modules/ModuleCategory.hpp>
#include <Features/Modules/Visual/ClickGui.hpp>
#include <Utils/FontHelper.hpp>
#include <Utils/MiscUtils/ImRenderUtils.hpp>
#include <Utils/MiscUtils/MathUtils.hpp>
#include <Features/Modules/Setting.hpp>
#include <Features/Modules/Visual/Interface.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Rendering/GuiData.hpp>
#include <Utils/Keyboard.hpp>
#include <Utils/StringUtils.hpp>
#include <Utils/MiscUtils/ColorUtils.hpp>

ImVec4 ModernGui::scaleToPoint(const ImVec4& _this, const ImVec4& point, float amount)
{
    return {point.x + (_this.x - point.x) * amount, point.y + (_this.y - point.y) * amount,
        point.z + (_this.z - point.z) * amount, point.w + (_this.w - point.w) * amount };
}

bool ModernGui::isMouseOver(const ImVec4& rect)
{
    ImVec2 mousePos = ImGui::GetIO().MousePos;
    return mousePos.x >= rect.x && mousePos.y >= rect.y && mousePos.x < rect.z && mousePos.y < rect.w;
}

ImVec4 ModernGui::getCenter(ImVec4& vec)
{
    float centerX = (vec.x + vec.z) / 2.0f;
    float centerY = (vec.y + vec.w) / 2.0f;
    return { centerX, centerY, centerX, centerY };
}

void ModernGui::render(float animation, float inScale, int& scrollDirection, char* h, float blur, float midclickRounding, bool isPressingShift)
{
    static auto interfaceMod = gFeatureManager->mModuleManager->getModule<Interface>();
    bool lowercase = interfaceMod->mNamingStyle.mValue == NamingStyle::Lowercase || interfaceMod->mNamingStyle.mValue == NamingStyle::LowercaseSpaced;

    FontHelper::pushPrefFont(true, false, true);
    ImVec2 screen = ImRenderUtils::getScreenSize();
    float deltaTime = ImGui::GetIO().DeltaTime;
    auto drawList = ImGui::GetBackgroundDrawList();

    // If the reset position bool is true and lastReset was more than 100ms ago, reset the position
    if (resetPosition && NOW - lastReset > 100)
    {
        catPositions.clear();
        ImVec2 screen = ImRenderUtils::getScreenSize();
        auto categories = ModuleCategoryNames;
        if (catPositions.empty())
        {
            float centerX = screen.x / 2.f;
            float xPos = centerX - (categories.size() * (catWidth + catGap) / 2);
            for (std::string& category : categories)
            {
                CategoryPosition pos;
                pos.x = xPos;
                pos.y = catGap * 2;
                pos.x = std::round(pos.x / 2) * 2;
                pos.y = std::round(pos.y / 2) * 2;

                xPos += catWidth + catGap;
                catPositions.push_back(pos);
            }
        }
        resetPosition = false;
    }

    /*ImRenderUtils::fillRectangle(
            ImVec4(0, 0, screen.x, screen.y),
            ImColor(0, 0, 0), animation * 0.38f);*/
    drawList->AddRectFilled(ImVec2(0, 0), ImVec2(screen.x, screen.y), IM_COL32(0, 0, 0, 255 * animation * 0.38f));
    ImRenderUtils::addBlur(ImVec4(0.f, 0.f, screen.x, screen.y),
                           animation * blur, 0);

    // Draw a glow rect on the bottom 1/3 of the screen
    ImColor shadowRectColor = ColorUtils::getThemedColor(0);
    shadowRectColor.Value.w = 0.5f * animation;

    // Draw a gradient rect on the bottom 1/3 of the screen with the shadow color, and making the alpha taper off as it goes up
    float firstheight = (screen.y - screen.y / 3);
    // As the animation reaches 0, the height should go below the screen height
    firstheight = MathUtils::lerp(screen.y, firstheight, inScale);
    ImRenderUtils::fillGradientOpaqueRectangle(
        ImVec4(0, firstheight, screen.x, screen.y),
        shadowRectColor, shadowRectColor, 0.4f * inScale, 0.0f);

    static std::vector<std::string> categories = ModuleCategoryNames;
    static std::vector<std::shared_ptr<Module>>& modules = gFeatureManager->mModuleManager->getModules();

    static auto* clickGui = gFeatureManager->mModuleManager->getModule<ClickGui>();
    bool isEnabled = clickGui->mEnabled;
    std::string tooltip = "";

    float textSize = inScale;
    //float textHeight = ImRenderUtils::getTextHeight(textSize);
    float textHeight = ImGui::GetFont()->CalcTextSizeA(textSize * 18, FLT_MAX, -1, "").y;

    int screenWidth = (int)screen.x;
    int screenHeight = 10;

    float windowWidth = 220.0f;
    float windowHeight = 190.0f;
    float yOffset = 50.0f;
    float windowX = (screenWidth - windowWidth) * 0.5f;
    float windowY = screenHeight;

    if (displayColorPicker && isEnabled)
    {
        FontHelper::pushPrefFont(false, false , true);
        ColorSetting* colorSetting = lastColorSetting;
        // Display the color picker in the bottom middle of the screen
        ImGui::SetNextWindowPos(ImVec2(screen.x / 2 - 200, screen.y / 2));
        ImGui::SetNextWindowSize(ImVec2(400, 400));

        ImGui::Begin("Color Picker", &displayColorPicker, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
        {
            ImVec4 color = colorSetting->getAsImColor().Value;
            ImGui::ColorPicker4("Color", colorSetting->mValue, ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha);
            ImGui::Button("Close");
            if (ImGui::IsItemClicked())
            {
                // Set the color setting to the new color
                colorSetting->setFromImColor(ImColor(color));
                displayColorPicker = false;
            }
        }
        ImGui::End();
        ImGui::PopFont();

        if (ImGui::IsMouseClicked(0) && !ImRenderUtils::isMouseOver(ImVec4(screen.x / 2 - 200, screen.y / 2, screen.x / 2 + 200, screen.y / 2 + 400)))
        {
            displayColorPicker = false;
        }
    }

    if (!isEnabled) displayColorPicker = false;

    if (catPositions.empty() && isEnabled)
    {
        float centerX = screen.x / 2.f;
        float xPos = centerX - (categories.size() * (catWidth + catGap) / 2);
        for (std::string& category : categories)
        {
            CategoryPosition pos;
            pos.x = xPos;
            pos.y = catGap * 2;
            pos.x = std::round(pos.x / 2) * 2;
            pos.y = std::round(pos.y / 2) * 2;
            xPos += catWidth + catGap;
            catPositions.push_back(pos);
        }
    }

    if (!catPositions.empty())
    {
        for (size_t i = 0; i < categories.size(); i++)
        {
            // Mod math stuff
            const float modWidth = catWidth;
            const float modHeight = catHeight;
            float moduleY = -catPositions[i].yOffset;

            // Get all the modules and populate our vector
            const auto& modsInCategory = gFeatureManager->mModuleManager->getModulesInCategory(i);

            // Calculate the catRect pos
            ImVec4 catRect = ImVec4(catPositions[i].x, catPositions[i].y,
                                                    catPositions[i].x + catWidth, catPositions[i].y + catHeight)
                .scaleToPoint(ImVec4(screen.x / 2,
                                             screen.y / 2,
                                             screen.x / 2,
                                             screen.y / 2), inScale);

            /* Calculate the height of the catWindow including the settings */
            float settingsHeight = 0;

            for (const auto& mod : modsInCategory)
            {
                std::string modLower = mod->getName();

                std::transform(modLower.begin(), modLower.end(), modLower.begin(), [](unsigned char c)
                {
                    return std::tolower(c);
                });

                for (const auto& setting : mod->mSettings)
                {
                    switch (setting->mType)
                    {
                    case SettingType::Bool:
                        {
                            settingsHeight = MathUtils::lerp(settingsHeight, settingsHeight + modHeight, mod->cAnim);
                            break;
                        }
                    case SettingType::Enum:
                        {
                            EnumSetting* enumSetting = reinterpret_cast<EnumSetting*>(setting);
                            std::vector<std::string> enumValues = enumSetting->mValues;
                            int numValues = static_cast<int>(enumValues.size());

                            settingsHeight = MathUtils::lerp(settingsHeight, settingsHeight + modHeight, mod->cAnim);
                            if (setting->enumSlide > 0.01)
                            {
                                for (int j = 0; j < numValues; j++)
                                    settingsHeight = MathUtils::lerp(settingsHeight, settingsHeight + modHeight,
                                                                setting->enumSlide);
                            }
                            break;
                        }
                    case SettingType::Number:
                        {
                            settingsHeight = MathUtils::lerp(settingsHeight, settingsHeight + modHeight, mod->cAnim);
                            break;
                        }
                    case SettingType::Color:
                        {
                            settingsHeight = MathUtils::lerp(settingsHeight, settingsHeight + modHeight, mod->cAnim);
                            break;
                        }
                    }
                }
            }

            float catWindowHeight = catHeight + modHeight * modsInCategory.size() + settingsHeight;
            ImVec4 catWindow = ImVec4(catPositions[i].x, catPositions[i].y,
                                                      catPositions[i].x + catWidth,
                                                      catPositions[i].y + moduleY + catWindowHeight)
                .scaleToPoint(ImVec4(screen.x / 2,
                                             screen.y / 2,
                                             screen.x / 2,
                                             screen.y / 2), inScale);
            ImColor rgb = ColorUtils::getThemedColor(i * 20);

            // Can we scroll?
            if (ImRenderUtils::isMouseOver(catWindow) && catPositions[i].isExtended)
            {
                if (scrollDirection > 0)
                {
                    catPositions[i].scrollEase += scrollDirection * catHeight;
                    if (catPositions[i].scrollEase > catWindowHeight - modHeight * 2)
                        catPositions[i].scrollEase = catWindowHeight - modHeight * 2;
                }
                else if (scrollDirection < 0)
                {
                    catPositions[i].scrollEase += scrollDirection * catHeight;
                    if (catPositions[i].scrollEase < 0)
                        catPositions[i].scrollEase = 0;
                }
                scrollDirection = 0;
            }

            // Lerp the category extending
            if (!catPositions[i].isExtended)
            {
                catPositions[i].scrollEase = catWindowHeight - catHeight;
                catPositions[i].wasExtended = false;
            }
            else if (!catPositions[i].wasExtended)
            {
                catPositions[i].scrollEase = 0;
                catPositions[i].wasExtended = true;
            }

            // Lerp the scrolling cuz smooth
            catPositions[i].yOffset = MathUtils::animate(catPositions[i].scrollEase, catPositions[i].yOffset,
                                                    ImRenderUtils::getDeltaTime() * 10.5);

            ImVec4 clipRect = ImVec4(catRect.x, catRect.w, catRect.z, screen.y);
            drawList->PushClipRect(ImVec2(clipRect.x, clipRect.y), ImVec2(clipRect.z, clipRect.w), true);

            int modIndex = 0;
            int modCount = modsInCategory.size();
            bool endMod = false;
            bool moduleToggled = false;
            for (const auto& mod : modsInCategory)
            {
                ImDrawFlags flags = ImDrawFlags_RoundCornersBottom;
                float radius = 0.f;
                if (modIndex == modsInCategory.size() - 1) {
                    endMod = true;
                    radius = 15.f * (1.f - mod->cAnim);
                }

                std::string modLower = mod->getName();

                std::transform(modLower.begin(), modLower.end(), modLower.begin(), [](unsigned char c)
                {
                    return std::tolower(c);
                });

                ImColor rgb = ColorUtils::getThemedColor(moduleY * 2);

                // If the mod belongs to the category
                if (mod->getCategory() == categories[i])
                {
                    // Calculate the modRect pos
                    ImVec4 modRect = ImVec4(catPositions[i].x,
                                                            catPositions[i].y + catHeight + moduleY,
                                                            catPositions[i].x + modWidth,
                                                            catPositions[i].y + catHeight + moduleY + modHeight)
                        .scaleToPoint(ImVec4(screen.x / 2,
                                                     screen.y / 2,
                                                     screen.x / 2,
                                                     screen.y / 2), inScale);
                    //modRect.y -= 4.f;
                    // floor the y value of the modRect
                    modRect.y = std::floor(modRect.y);
                    modRect.x = std::floor(modRect.x);

                    // Animate the setting animation percentage
                    float targetAnim = mod->showSettings ? 1.f : 0.f;
                    mod->cAnim = MathUtils::animate(targetAnim, mod->cAnim, ImRenderUtils::getDeltaTime() * 12.5);
                    mod->cAnim = MathUtils::clamp(mod->cAnim, 0.f, 1.f);

                    // Settings
                    if (mod->cAnim > 0.001)
                    {
                        static bool wasDragging = false;
                        Setting* lastDraggedSetting = nullptr;
                        int sIndex = 0;
                        for (const auto& setting : mod->mSettings)
                        {
                            if (!setting->mIsVisible())
                            {
                                //Reset the animation if the setting is not visible
                                setting->sliderEase = 0;
                                setting->enumSlide = 0;
                                continue;
                            }

                            float radius = 0.f;
                            if (endMod && sIndex == mod->mSettings.size() - 1)
                                radius = 15.f;
                            else if (endMod)
                                radius = 15.f * (1.f - mod->cAnim);



                            bool endSetting = sIndex == mod->mSettings.size() - 1;
                            float setPadding = endSetting ? (-2.f * animation) : 0.f;

                            ImColor rgb = ColorUtils::getThemedColor(moduleY * 2);
                            // Base the alpha off the animation percentage
                            rgb.Value.w = animation;
                            switch (setting->mType)

                            {
                            case SettingType::Bool:
                                {
                                    BoolSetting* boolSetting = reinterpret_cast<BoolSetting*>(setting);
                                    moduleY = MathUtils::lerp(moduleY, moduleY + modHeight, mod->cAnim);

                                    ImVec4 rect = ImVec4(
                                            modRect.x, catPositions[i].y + catHeight + moduleY + setPadding, modRect.z,
                                            catPositions[i].y + catHeight + moduleY + modHeight)
                                        .scaleToPoint(
                                            ImVec4(modRect.x, screen.y / 2,
                                                           modRect.z, screen.y / 2),
                                            inScale);
                                    rect.y = std::floor(rect.y);
                                    // Clamp rect start Y to top of the modRect
                                    if (rect.y < modRect.y)
                                    {
                                        rect.y = modRect.y;
                                    }

                                    if (rect.y > catRect.y + 0.5f)
                                    {
                                        std::string setName = lowercase ? StringUtils::toLower(setting->mName) : setting->mName;
                                        ImRenderUtils::fillRectangle(rect, ImColor(30, 30, 30), animation, radius, ImGui::GetBackgroundDrawList(), ImDrawFlags_RoundCornersBottom);

                                        if (ImRenderUtils::isMouseOver(rect) && isEnabled && catPositions[i].isExtended)
                                        {
                                            tooltip = setting->mDescription;
                                            if (ImGui::IsMouseClicked(0) && !displayColorPicker && mod->showSettings)
                                            {
                                                //*(bool*)setting->getValue() = !*(bool*)setting->getValue();
                                                boolSetting->mValue = !boolSetting->mValue;
                                            }

                                            if (ImGui::IsMouseClicked(2) && !displayColorPicker && catPositions[i].isExtended)
                                            {
                                                lastBoolSetting = boolSetting;
                                                isBoolSettingBinding = true;
                                                ClientInstance::get()->playUi("random.pop", 0.75f, 1.0f);
                                            }
                                        }

                                        setting->boolScale = MathUtils::animate(
                                            boolSetting->mValue ? 1 : 0, setting->boolScale,
                                            ImRenderUtils::getDeltaTime() * 10);

                                        float scaledWidth = rect.getWidth();
                                        float scaledHeight = rect.getHeight();

                                        ImVec2 center = ImVec2(rect.x + rect.getWidth() / 2.f, rect.y + rect.getHeight() / 2.f);
                                        ImVec4 scaledRect = ImVec4(center.x - scaledWidth / 2.f, center.y - scaledHeight / 2.f, center.x + scaledWidth / 2.f, center.y + scaledHeight / 2.f);

                                        float cSetRectCentreX = rect.x + ((rect.z - rect.x) - ImRenderUtils::getTextWidth(&setName, textSize)) / 2;
                                        float cSetRectCentreY = rect.y + ((rect.w - rect.y) - textHeight) / 2;

                                        ImVec4 smoothScaledRect = ImVec4(scaledRect.z - 19, scaledRect.y + 5, scaledRect.z - 5, scaledRect.w - 5);//
                                        ImVec2 circleRect = ImVec2(smoothScaledRect.getCenter().x, smoothScaledRect.getCenter().y);

                                        // Lerp shadow color using boolScale
                                        ImColor targetShadowCol = ImColor(15, 15, 15);
                                        ImColor shadowCol = MathUtils::lerpImColor(targetShadowCol, rgb, setting->boolScale);

                                        ImRenderUtils::fillShadowCircle(circleRect, 5, shadowCol, animation * mod->cAnim, 40, 0);

                                        ImVec4 booleanRect = ImVec4(rect.z - 23.5f, cSetRectCentreY - 2.5f, rect.z - 5, cSetRectCentreY + 17.5f);
                                        booleanRect = booleanRect.scaleToPoint(ImVec4(rect.z, rect.y, rect.z, rect.w), animation);

                                        float rectXDiff = booleanRect.z - booleanRect.x;

                                        if (setting->boolScale > 0.01) {
                                            // Make the min y of the boolean rect the top of the setting rect
                                            if (booleanRect.y < modRect.w) {
                                                booleanRect.y = modRect.w;
                                            }
                                            ImGui::GetForegroundDrawList()->PushClipRect(ImVec2(booleanRect.x, booleanRect.y), ImVec2(booleanRect.x + rectXDiff * setting->boolScale, booleanRect.w), true);

                                            ImRenderUtils::drawCheckMark(ImVec2(booleanRect.getCenter().x - (4 * animation),
                                                                                booleanRect.getCenter().y - (1 * animation)), 1.3 * animation,
                                                                         rgb, mod->cAnim * animation);
                                            ImRenderUtils::drawCheckMark(ImVec2(booleanRect.getCenter().x - (4 * animation),
                                                                                booleanRect.getCenter().y - (1 * animation)), 1.3 * animation,
                                                                         rgb, mod->cAnim * animation);

                                            ImGui::GetForegroundDrawList()->PopClipRect();
                                        }

                                        ImRenderUtils::drawText(ImVec2(rect.x + 5.f, cSetRectCentreY), setName,
                                                               ImColor(255, 255, 255), textSize, animation, true);
                                    }
                                    break;
                                }
                            case SettingType::Enum:
                                {
                                    EnumSetting* enumSetting = reinterpret_cast<EnumSetting*>(setting);
                                    std::string setName = lowercase ? StringUtils::toLower(setting->mName) : setting->mName;
                                    std::vector<std::string> enumValues = enumSetting->mValues;
                                    if (lowercase)
                                    {
                                        for (std::string& value : enumValues)
                                        {
                                            value = StringUtils::toLower(value);
                                        }
                                    }
                                    int* iterator = &enumSetting->mValue;
                                    int numValues = static_cast<int>(enumValues.size());

                                    moduleY = MathUtils::lerp(moduleY, moduleY + modHeight, mod->cAnim);

                                    ImVec4 rect = ImVec4(
                                            modRect.x, catPositions[i].y + catHeight + moduleY + setPadding, modRect.z,
                                            catPositions[i].y + catHeight + moduleY + modHeight)
                                        .scaleToPoint(
                                            ImVec4(modRect.x, screen.y / 2,
                                                           modRect.z, screen.y / 2),
                                            inScale);
                                    rect.y = std::floor(rect.y);
                                    if (rect.y < modRect.y)
                                    {
                                        rect.y = modRect.y;
                                    }

                                    float targetAnim = setting->enumExtended && mod->showSettings ? 1.f : 0.f;
                                    setting->enumSlide = MathUtils::animate(
                                        targetAnim, setting->enumSlide, ImRenderUtils::getDeltaTime() * 10);
                                    setting->enumSlide = MathUtils::clamp(setting->enumSlide, 0.f, 1.f);

                                    if (setting->enumSlide > 0.001)
                                    {
                                        for (int j = 0; j < numValues; j++)
                                        {
                                            std::string enumValue = enumValues[j];

                                            moduleY = MathUtils::lerp(moduleY, moduleY + modHeight, setting->enumSlide);

                                            ImVec4 rect2 = ImVec4(
                                                    modRect.x, catPositions[i].y + catHeight + moduleY + setPadding, modRect.z,
                                                    catPositions[i].y + catHeight + moduleY + modHeight)
                                                .scaleToPoint(
                                                    ImVec4(modRect.x, screen.y / 2,
                                                                   modRect.z, screen.y / 2),
                                                    inScale);

                                            if (rect2.y > catRect.y + 0.5f)
                                            {
                                                float cSetRectCentreY = rect2.y + ((rect2.w - rect2.y) - textHeight)
                                                    / 2;

                                                ImRenderUtils::fillRectangle(rect2, ImColor(20, 20, 20), animation, radius, ImGui::GetBackgroundDrawList(), ImDrawFlags_RoundCornersBottom);

                                                if (*iterator == j)
                                                    ImRenderUtils::fillRectangle(
                                                        ImVec4(rect2.x, rect2.y, rect2.x + 1.5f, rect2.w),
                                                        rgb, animation);

                                                if (ImRenderUtils::isMouseOver(rect2) && ImGui::IsMouseClicked(0) &&
                                                    isEnabled && !displayColorPicker && mod->showSettings)
                                                {
                                                    *iterator = j;
                                                }

                                                ImRenderUtils::drawText(
                                                    ImVec2(rect2.x + 5.f, cSetRectCentreY), enumValue,
                                                    ImColor(255, 255, 255), textSize, animation, true);
                                            }
                                        }
                                    }

                                    if (rect.y > catRect.y + 0.5f)
                                    {
                                        ImRenderUtils::fillRectangle(rect, ImColor(30, 30, 30), animation, radius, ImGui::GetBackgroundDrawList(), ImDrawFlags_RoundCornersBottom);

                                        if (ImRenderUtils::isMouseOver(rect) && isEnabled && catPositions[i].isExtended)
                                        {
                                            tooltip = setting->mDescription;
                                            if (ImGui::IsMouseClicked(0) && !displayColorPicker && mod->showSettings)
                                            {
                                                *iterator = (*iterator + 1) % enumValues.size();
                                            }
                                            else if (ImGui::IsMouseClicked(1) && mod->showSettings && !displayColorPicker && mod->showSettings)
                                            {
                                                setting->enumExtended = !setting->enumExtended;
                                            }
                                        }

                                        float cSetRectCentreY = rect.y + ((rect.w - rect.y) - textHeight) / 2;

                                        std::string enumValue = enumValues[*iterator];
                                        std::string settingName = setName;
                                        std::string settingString = enumValue;
                                        auto ValueLen = ImRenderUtils::getTextWidth(&settingString, textSize);

                                        ImRenderUtils::drawText(ImVec2(rect.x + 5.f, cSetRectCentreY),
                                                               settingName, ImColor(255, 255, 255), textSize,
                                                               animation, true);
                                        ImRenderUtils::drawText(
                                            ImVec2((rect.z - 5.f) - ValueLen, cSetRectCentreY),
                                            settingString, ImColor(170, 170, 170), textSize, animation, true);
                                    }
                                    if (rect.y > catRect.y - modHeight)
                                    {
                                        ImRenderUtils::fillGradientOpaqueRectangle(
                                            ImVec4(rect.x, rect.w, rect.z,
                                                           rect.w + 10.f * setting->enumSlide * animation),
                                            ImColor(0, 0, 0), ImColor(0, 0, 0), 0.F * animation, 0.55F * animation);
                                    }
                                    break;
                                }
                            case SettingType::Number:
                                {
                                    NumberSetting* numSetting = reinterpret_cast<NumberSetting*>(setting);
                                    const float value = numSetting->mValue;
                                    const float min = numSetting->mMin;
                                    const float max = numSetting->mMax;

                                    char str[10];
                                    sprintf_s(str, 10, "%.2f", value);
                                    std::string rVal = str;

                                    std::string setName = lowercase ? StringUtils::toLower(setting->mName) : setting->mName;
                                    std::string valueName = rVal;

                                    moduleY = MathUtils::lerp(moduleY, moduleY + modHeight, mod->cAnim);

                                    ImVec4 backGroundRect = ImVec4(
                                            modRect.x, (catPositions[i].y + catHeight + moduleY), modRect.z,
                                            catPositions[i].y + catHeight + moduleY + modHeight)
                                        .scaleToPoint(
                                            ImVec4(modRect.x, screen.y / 2,
                                                           modRect.z, screen.y / 2),
                                            inScale);

                                    backGroundRect.y = std::floor(backGroundRect.y);
                                    if (backGroundRect.y < modRect.y)
                                    {
                                        backGroundRect.y = modRect.y;
                                    }

                                    ImVec4 rect = ImVec4(
                                            modRect.x + 7, (catPositions[i].y + catHeight + moduleY + setPadding), modRect.z - 7,
                                            catPositions[i].y + catHeight + moduleY + modHeight)
                                        .scaleToPoint(
                                            ImVec4(modRect.x, screen.y / 2,
                                                           modRect.z, screen.y / 2),
                                            inScale);
                                    rect.y = std::floor(rect.y);
                                    if (rect.y < modRect.y)
                                    {
                                        rect.y = modRect.y;
                                    }

                                    static float clickAnimation = 1.f;

                                    // If left click is down, lerp the alpha to 0.60f;
                                    if (ImGui::IsMouseDown(0) && ImRenderUtils::isMouseOver(rect))
                                    {
                                        clickAnimation = MathUtils::animate(0.60f, clickAnimation, ImRenderUtils::getDeltaTime() * 10);
                                    }
                                    else
                                    {
                                        clickAnimation = MathUtils::animate(1.f, clickAnimation, ImRenderUtils::getDeltaTime() * 10);
                                    }

                                    if (backGroundRect.y > catRect.y + 0.5f)
                                    {
                                        ImRenderUtils::fillRectangle(backGroundRect, ImColor(30, 30, 30), animation, radius, ImGui::GetBackgroundDrawList(), ImDrawFlags_RoundCornersBottom);

                                        const float sliderPos = (value - min) / (max - min) * (rect.z - rect.x);

                                        setting->sliderEase = MathUtils::animate(
                                            sliderPos, setting->sliderEase, ImRenderUtils::getDeltaTime() * 10);
                                        setting->sliderEase = std::clamp(setting->sliderEase, 0.f, rect.getWidth());

#pragma region Slider dragging
                                       if (ImRenderUtils::isMouseOver(rect) && isEnabled && catPositions[i].isExtended)
                                        {
                                            tooltip = setting->mDescription;
                                            if (ImGui::IsMouseDown(0) || ImGui::IsMouseDown(2))
                                            {
                                                setting->isDragging = true;
                                                lastDraggedSetting = setting;
                                            }
                                        }

                                        if (ImGui::IsMouseDown(0) && setting->isDragging && isEnabled)
                                        {
                                            if (lastDraggedSetting != setting)
                                            {
                                                setting->isDragging = false;
                                            } else
                                            {
                                                const float newValue = std::fmax(
                                                    std::fmin(
                                                        (ImRenderUtils::getMousePos().x - rect.x) / (rect.z - rect.x) * (
                                                            max - min) + min, max), min);
                                                numSetting->setValue(newValue);
                                            }
                                        }
                                        else if (ImGui::IsMouseDown(2) && setting->isDragging && isEnabled)
                                        {
                                            if (lastDraggedSetting != setting)
                                            {
                                                setting->isDragging = false;
                                            } else
                                            {
                                                float newValue = std::fmax(
                                                    std::fmin(
                                                        (ImRenderUtils::getMousePos().x - rect.x) / (rect.z - rect.x) * (
                                                            max - min) + min, max), min);
                                                // Round the value to the nearest value specified by midclickRounding
                                                newValue = std::round(newValue / midclickRounding) * midclickRounding;
                                                numSetting->mValue = newValue;
                                            }
                                        }
                                        else
                                        {
                                            setting->isDragging = false;
                                        }
#pragma endregion

                                        // "doesn't animate down when animating out" cuz its made by a 11 yr old smart nerd :yum:
                                        /* Original code (doesn't animate down when animating out)
                                        ImRenderUtils::fillRectangle(ImVec4(rect.x, (catPositions[i].y + catHeight + moduleY + modHeight) - 3, rect.x + setting->sliderEase, rect.w), rgb, animation);
                                        ImRenderUtils::fillShadowRectangle(ImVec4(rect.x, (catPositions[i].y + catHeight + moduleY + modHeight) - 3, rect.x + setting->sliderEase, rect.w), rgb, animation, 50.f, 0);*/

                                        float ySize = rect.w - rect.y;

                                        ImVec2 sliderBarMin = ImVec2(rect.x, rect.w - ySize / 8);
                                        ImVec2 sliderBarMax = ImVec2(rect.x + (setting->sliderEase * inScale), rect.w);
                                        sliderBarMin.y = sliderBarMax.y - 4 * inScale;

                                        ImVec4 sliderRect = ImVec4(sliderBarMin.x, sliderBarMin.y - 4.5f, sliderBarMax.x, sliderBarMax.y - 6.5f);

                                        // The slider bar
                                        ImRenderUtils::fillRectangle(sliderRect, rgb, animation, 15);

                                        // Circle (I am not sure)
                                        ImVec2 circlePos = ImVec2(sliderRect.z - 2.25f, sliderRect.getCenter().y);

                                        if (value <= min + 0.83f)
                                        {
                                            circlePos.x = sliderRect.z + 2.25f;
                                        }


                                        ImRenderUtils::fillCircle(circlePos, 5.5f * clickAnimation * animation, rgb, animation, 12);

                                        // Push a clip rect to prevent the shadow from going outside the slider bar
                                        ImGui::GetBackgroundDrawList()->PushClipRect(ImVec2(sliderRect.x, sliderRect.y), ImVec2(sliderRect.z, sliderRect.w), true);

                                        ImRenderUtils::fillShadowRectangle(sliderRect, rgb, animation * 0.75f, 15.f, 0);

                                        ImGui::GetBackgroundDrawList()->PopClipRect();

                                        auto ValueLen = ImRenderUtils::getTextWidth(&valueName, textSize);
                                        ImRenderUtils::drawText(
                                            ImVec2((backGroundRect.z - 5.f) - ValueLen, backGroundRect.y + 2.5f), valueName,
                                            ImColor(170, 170, 170), textSize, animation, true);
                                        ImRenderUtils::drawText(ImVec2(backGroundRect.x + 5.f, backGroundRect.y + 2.5f),
                                                               setName, ImColor(255, 255, 255), textSize,
                                                               animation, true);
                                    }
                                    break;
                                }
                            case SettingType::Color:
                                {
                                    ColorSetting* colorSetting = reinterpret_cast<ColorSetting*>(setting);
                                    ImColor color = colorSetting->getAsImColor();
                                    ImVec4 rgb = color.Value;
                                    std::string setName = lowercase ? StringUtils::toLower(setting->mName) : setting->mName;

                                    moduleY = MathUtils::lerp(moduleY, moduleY + modHeight, mod->cAnim);

                                    ImVec4 rect = ImVec4(
                                            modRect.x, catPositions[i].y + catHeight + moduleY + setPadding, modRect.z,
                                            catPositions[i].y + catHeight + moduleY + modHeight)
                                        .scaleToPoint(
                                            ImVec4(modRect.x, screen.y / 2,
                                                           modRect.z, screen.y / 2),
                                            inScale);
                                    rect.y = std::floor(rect.y);
                                    if (rect.y < modRect.y)
                                    {
                                        rect.y = modRect.y;
                                    }

                                    if (rect.y > catRect.y + 0.5f)
                                    {
                                        ImRenderUtils::fillRectangle(rect, ImColor(30, 30, 30), animation);

                                        if (ImRenderUtils::isMouseOver(rect) && isEnabled && catPositions[i].isExtended)
                                        {
                                            tooltip = setting->mDescription;
                                            if (ImGui::IsMouseClicked(0) && !displayColorPicker && mod->showSettings)
                                            {
                                                displayColorPicker = !displayColorPicker;
                                                lastColorSetting = colorSetting;
                                            }
                                        }

                                        float cSetRectCentreY = rect.y + ((rect.w - rect.y) - textHeight) / 2;
                                        ImRenderUtils::drawText(ImVec2(rect.x + 5.f, cSetRectCentreY), setName,
                                                               ImColor(255, 255, 255), textSize, animation, true);

                                        ImVec2 colorRect = ImVec2(rect.z - 20, rect.y + 5);
                                        ImRenderUtils::fillRectangle(ImVec4(rect.z - 20, rect.y + 5, rect.z - 5, rect.w - 5),
                                                                     colorSetting->getAsImColor(), animation);
                                    }
                                    break;
                                }
                            }

                            sIndex++;
                        }

                    }


                    if (modRect.y > catRect.y + 0.5f)
                    {
                        // Draw the rect
                        if (mod->cScale <= 1)
                        {
                            if (mod->mEnabled)
                                ImRenderUtils::fillRectangle(modRect, rgb, animation, radius, ImGui::GetBackgroundDrawList(), ImDrawCornerFlags_BotRight | ImDrawCornerFlags_BotLeft);
                            else
                                ImRenderUtils::fillRectangle(modRect, ImColor(30, 30, 30), animation, radius, ImGui::GetBackgroundDrawList(), ImDrawCornerFlags_BotRight | ImDrawCornerFlags_BotLeft);
                            ImRenderUtils::fillRectangle(modRect, grayColor, animation, radius, ImGui::GetBackgroundDrawList(), ImDrawCornerFlags_BotRight | ImDrawCornerFlags_BotLeft);
                        }

                        std::string modName = mod->getName();

                        // Calculate the centre of the rect
                        ImVec2 center = ImVec2(modRect.x + modRect.getWidth() / 2.f,
                                                               modRect.y + modRect.getHeight() / 2.f);

                        mod->cScale = MathUtils::animate(mod->mEnabled ? 1 : 0, mod->cScale,
                                                    ImRenderUtils::getDeltaTime() * 10);

                        // Calculate scaled size based on cScale
                        float scaledWidth = modRect.getWidth();
                        float scaledHeight = modRect.getHeight();

                        // Calculate new rectangle based on scaled size and center point
                        ImVec4 scaledRect = ImVec4(center.x - scaledWidth / 2.f,
                                                                   center.y - scaledHeight / 2.f,
                                                                   center.x + scaledWidth / 2.f,
                                                                   center.y + scaledHeight / 2.f);

                        // Interpolate between original rectangle and scaled rectangle
                        if (mod->cScale > 0)
                        {

                            //ImRenderUtils::fillRectangle(scaledRect, rgb, animation * mod->cScale + 0.01f);
                            ImColor rgb1 = rgb;
                            float modIndexY = moduleY + (scaledRect.y - scaledRect.w);

                            ImColor rgb2 = ColorUtils::getThemedColor(scaledRect.y + ((scaledRect.z - scaledRect.x)));
                            //ImRenderUtils::fillGradientOpaqueRectangle(scaledRect, rgb1, rgb2, animation * mod->cScale, animation * mod->cScale);
                            // Round only if we are rendering the last module and the settings aren't expanded
                            //ImRenderUtils::fillRectangle(scaledRect, rgb1, animation * mod->cScale, radius, ImGui::GetBackgroundDrawList(), ImDrawFlags_RoundCornersBottom);
                            ImRenderUtils::fillRoundedGradientRectangle(scaledRect, rgb1, rgb2, radius, animation * mod->cScale, animation * mod->cScale, flags);
                        }

                        float cRectCentreX = modRect.x + ((modRect.z - modRect.x) - ImRenderUtils::getTextWidth(
                            &modName, textSize)) / 2;
                        float cRectCentreY = modRect.y + ((modRect.w - modRect.y) - textHeight) / 2;

                        // cRectCentreX. vRectCentreY
                        //.lerp(ImVec2(modRect.x + 5, cRectCentreY), mod->cAnim) // if we want lerp to left on extend
                        ImVec2 modPosLerped = ImVec2(cRectCentreX, cRectCentreY);

                        ImRenderUtils::drawText(modPosLerped, modName,
                                               ImColor(mod->mEnabled
                                                           ? ImColor(255, 255, 255)
                                                           : ImColor(180, 180, 180)).Lerp(
                                                           mod->mEnabled
                                                           ? ImColor(255, 255, 255)
                                                           : ImColor(180, 180, 180), mod->cAnim), textSize, animation, true);

                        /*std::string bindName = (mod == lastMod && ClickGUIManager::isBinding) ? "Binding..." : std::to_string((char)mod->getKeybind()).c_str();
                        float bindNameLen = ImRenderUtils::getTextWidth(&bindName, textSize);
                        ImVec4 bindRect = ImVec4((modRect.z - 10) - bindNameLen, modRect.y + 2.5, modRect.z - 2.5, modRect.w - 2.5);
                        ImVec2 bindTextPos = ImVec2(bindRect.x + 3.5, cRectCentreY);

                        if (mod->getKeybind() != 7) {
                            ImRenderUtils::fillRectangle(bindRect, ImColor(29, 29, 29), 0.9, 4);
                            ImRenderUtils::drawText(bindTextPos, &bindName, ImColor(255, 255, 255), textSize, animation, true);
                        }
                        else if (mod->getKeybind() == 7 && mod == lastMod && ClickGUIManager::isBinding) {
                            ImRenderUtils::fillRectangle(bindRect, ImColor(29, 29, 29), 0.9, 4);
                            ImRenderUtils::drawText(bindTextPos, &bindName, ImColor(255, 255, 255), textSize, animation, true);
                        }*/

                        //GuiData* guidata = Global::getClientInstance()->getGuiData();
                        float renderx = screen.x / 2;
                        float rendery = (screen.y / 2) + 110;

                        if (ImRenderUtils::isMouseOver(modRect) && catPositions[i].isExtended && isEnabled && catPositions[i].isExtended)
                        {
                            if (ImRenderUtils::isMouseOver(catWindow) && catPositions[i].isExtended && catPositions[i].isExtended)
                            {
                                tooltip = mod->mDescription;
                            }

                            if (ImGui::IsMouseClicked(0) && !displayColorPicker && catPositions[i].isExtended)
                            {
                                if (!moduleToggled) mod->toggle();
                                ClientInstance::get()->playUi("random.pop", 0.75f, 1.0f);
                                moduleToggled = true;
                            }
                            else if (ImGui::IsMouseClicked(1) && !displayColorPicker && catPositions[i].isExtended)
                            {
                                // Only show if the module has settings
                                if (!mod->mSettings.empty()) mod->showSettings = !mod->showSettings;
                            }
                            else if (ImGui::IsMouseClicked(2)  && !displayColorPicker && catPositions[i].isExtended)
                            {
                                lastMod = mod;
                                isBinding = true;
                                ClientInstance::get()->playUi("random.pop", 0.75f, 1.0f);
                            }
                        }
                    }
                    if (modRect.y > catRect.y - modHeight)
                    {
                        // Render a slight glow effect
                        ImRenderUtils::fillGradientOpaqueRectangle(
                            ImVec4(modRect.x, modRect.w, modRect.z,
                                           modRect.w + 10.f * mod->cAnim * animation), ImColor(0, 0, 0),
                            ImColor(0, 0, 0), 0.F * animation, 0.55F * animation);
                    }
                    moduleY += modHeight;

                    modIndex++;
                }
            }
            drawList->PopClipRect();

            if (isBinding)
            {
                tooltip = "Currently binding " + lastMod->getName() + "..." +" Press ESC to unbind.";
                for (const auto& key : Keyboard::mPressedKeys)
                {
                    if (key.second && lastMod)
                    {
                        //lastMod->setKeybind(key.first == Keys::ESC ? 7 : key.first);
                        lastMod->mKey = key.first == VK_ESCAPE ? 0 : key.first;
                        isBinding = false;
                        if (key.first == VK_ESCAPE)
                        {
                            ClientInstance::get()->playUi("random.break", 0.75f, 1.0f);
                        } else
                        {
                            ClientInstance::get()->playUi("random.orb", 0.75f, 1.0f);
                        }
                    }
                }
            }

            if (isBoolSettingBinding)
            {
                tooltip = "Currently binding " + lastBoolSetting->mName + "... Press ESC to unbind.";
                for (const auto& key : Keyboard::mPressedKeys)
                {
                    if (key.second && lastBoolSetting)
                    {
                        lastBoolSetting->mKey = (key.first == VK_ESCAPE) ? 0 : key.first;
                        isBoolSettingBinding = false;

                        if (key.first == VK_ESCAPE)
                        {
                            ClientInstance::get()->playUi("random.break", 0.75f, 1.0f);
                        }
                        else
                        {
                            ClientInstance::get()->playUi("random.orb", 0.75f, 1.0f);
                        }
                    }
                }
            }

            std::string catName = lowercase ? StringUtils::toLower(categories[i]) : categories[i];

            if (ImRenderUtils::isMouseOver(catRect) && ImGui::IsMouseClicked(1))
                catPositions[i].isExtended = !catPositions[i].isExtended;

            catRect.w += 1.5f;
            ImRenderUtils::fillRectangle(catRect, darkBlack, animation, 15, ImGui::GetBackgroundDrawList(), ImDrawFlags_RoundCornersTop);

            ImVec4 lineRect = ImVec4(catRect.x, catRect.w - 0.75f, catRect.z, catRect.w + 0.75f);

            //ImRenderUtils::fillGradientOpaqueRectangle(lineRect, rgb, ColorUtils::getThemedColor(catRect.y + ((catRect.z - catRect.x))), animation, animation);



            FontHelper::pushPrefFont(true, true, true);
            // Calculate the centre of the rect
            float textHeight = ImGui::GetFont()->CalcTextSizeA(textSize * 18, FLT_MAX, -1, catName.c_str()).y;
            float cRectCentreX = catRect.x + ((catRect.z - catRect.x) - ImRenderUtils::getTextWidth(
                &catName, textSize * 1.15)) / 2;
            float cRectCentreY = catRect.y + ((catRect.w - catRect.y) - textHeight) / 2;

            std::string IconStr = "B";
            // TODO: please for the love of god make icon fkery like this into FontHelper.......
            // (also don't forget to check for case u idiot!!!!!!!111!!!!1)
            if (StringUtils::equalsIgnoreCase(catName, "Combat")) IconStr = "c";
            else if (StringUtils::equalsIgnoreCase(catName, "Movement")) IconStr = "f";
            else if (StringUtils::equalsIgnoreCase(catName, "Visual")) IconStr = "d";
            else if (StringUtils::equalsIgnoreCase(catName, "Player")) IconStr = "e";
            else if (StringUtils::equalsIgnoreCase(catName, "Misc")) IconStr = "a";


            ImGui::PushFont(FontHelper::Fonts["tenacity_icons_large"]);
            // Draw the icon
            ImRenderUtils::drawText(ImVec2(catRect.x + 10, cRectCentreY), IconStr, ImColor(255, 255, 255),
                                   textSize * 1.15, animation, true);
            ImGui::PopFont();

            // Draw the string
            ImRenderUtils::drawText(ImVec2(cRectCentreX, cRectCentreY), catName, ImColor(255, 255, 255),
                                   textSize * 1.15, animation, true);
            ImGui::PopFont();

            catPositions[i].x = std::clamp(catPositions[i].x, 0.f, screen.x - catWidth);
            catPositions[i].y = std::clamp(catPositions[i].y, 0.f, screen.y - catHeight);

#pragma region DraggingLogic
            static bool dragging = false;
            static ImVec2 dragOffset;
            if (catPositions[i].isDragging)
            {
                if (ImGui::IsMouseDown(0))
                {
                    if (!dragging)
                    {
                        dragOffset = ImVec2(ImRenderUtils::getMousePos().x - catRect.x,
                                                    ImRenderUtils::getMousePos().y - catRect.y);
                        dragging = true;
                    }
                    ImVec2 newPosition = ImVec2(ImRenderUtils::getMousePos().x - dragOffset.x,
                                                                ImRenderUtils::getMousePos().y - dragOffset.y);
                    newPosition.x = std::clamp(newPosition.x, 0.f,
                                               screen.x - catWidth);
                    newPosition.y = std::clamp(newPosition.y, 0.f,
                                               screen.y - catHeight);
                    // Round the position to an even number
                    newPosition.x = std::round(newPosition.x / 2) * 2;
                    newPosition.y = std::round(newPosition.y / 2) * 2;

                    catPositions[i].x = newPosition.x;
                    catPositions[i].y = newPosition.y;
                }
                else
                {
                    catPositions[i].isDragging = false;
                    dragging = false;
                }
            }
            else if (ImRenderUtils::isMouseOver(catRect) && ImGui::IsMouseClicked(0) && isEnabled)
            {
                catPositions[i].isDragging = true;
                dragOffset = ImVec2(ImRenderUtils::getMousePos().x - catRect.x,
                                            ImRenderUtils::getMousePos().y - catRect.y);
            }
#pragma endregion
        }

        if (!tooltip.empty())
        {
            ImVec2 toolTipHeight = ImGui::GetFont()->CalcTextSizeA(textSize * 14.4f, FLT_MAX, 0, tooltip.c_str());
            float textWidth = ImRenderUtils::getTextWidth(&tooltip, textSize * 0.8f);
            float textHeight = toolTipHeight.y;
            float padding = 2.5f;
            float offset = 8.f;

            ImVec4 tooltipRect = ImVec4(
                ImRenderUtils::getMousePos().x + offset - padding,
                ImRenderUtils::getMousePos().y + textHeight / 2 - textHeight - padding,
                ImRenderUtils::getMousePos().x + offset + textWidth + padding * 2,
                ImRenderUtils::getMousePos().y + textHeight / 2 + padding
            ).scaleToPoint(ImVec4(
                               screen.x / 2,
                               screen.y / 2,
                               screen.x / 2,
                               screen.y / 2
                           ), inScale);

            static float alpha = 1.f;

            // If mid or left click is down, lerp the alpha to 0.25f;
            if (ImGui::IsMouseDown(0) || ImGui::IsMouseDown(2))
            {
                alpha = MathUtils::animate(0.0f, alpha, ImRenderUtils::getDeltaTime() * 10);
            }
            else
            {
                alpha = MathUtils::animate(1.f, alpha, ImRenderUtils::getDeltaTime() * 10);
            }

            tooltipRect = tooltipRect.scaleToCenter(alpha);

            ImRenderUtils::fillRectangle(tooltipRect, ImColor(20, 20, 20), animation * alpha, 0.f, ImGui::GetForegroundDrawList());
            ImRenderUtils::drawText(ImVec2(tooltipRect.x + padding, tooltipRect.y + padding), tooltip,
                                   ImColor(255, 255, 255), (textSize * 0.8f) * alpha, animation * alpha, true, 0, ImGui::GetForegroundDrawList());
        }

        if (isEnabled)
        {
            scrollDirection = 0;
        }
    }
    ImGui::PopFont();
}

void ModernGui::onWindowResizeEvent(WindowResizeEvent& event)
{
    resetPosition = true;
    lastReset = NOW;
}
