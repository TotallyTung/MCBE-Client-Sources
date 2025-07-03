#pragma once
#include "Manager/ClickGUIManager.hpp"

class FrostGUI {
public:
    struct CategoryPosition {
        float x = 0.f, y = 0.f;
        bool mIsDragging = false, isExtended = true, wasExtended = false;
        float yOffset = 0;
        float scrollEase = 0;
        Vector2<float> dragVelocity = Vector2<float>();
        Vector2<float> dragAcceleration = Vector2<float>();
    };

    const float catWidth = 200.f;
    const float catHeight = 35.f;

    const float catGap = 40;
    int lastDragged = -1;
    std::vector<CategoryPosition> catPositions;
    Module* lastMod = nullptr;

    Color mModuleBackgroundColor = Color(45, 45, 45);
    
    Color mCategoryHeaderColor = Color(30, 30, 30);

    Color mSettingsBackgroundColor = Color(35, 35, 35);
    Color mBoolBoxColor = Color(20, 20, 20);

    Color mEnumBoxColor = Color(45, 45, 45);
    Color mEnumBoxColorSecond = Color(35, 35, 35);

    void render(float animation, float inScale, int& scrollDirection) {
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);

        bool isEnabled = getModuleByName("clickgui")->isEnabled();
        std::string tooltip = "";

        float textSize = inScale * 1.f;
        float textHeight = ImRenderUtil::getTextHeight(textSize);

        if (catPositions.empty() && isEnabled)
        {
            float centerX = ImRenderUtil::getScreenSize().x / 2.f;
            float xPos = centerX - (mCategories.size() * (catWidth + catGap) / 2);
            for (std::string& category : mCategories)
            {
                CategoryPosition pos;
                pos.x = xPos;
                pos.y = catGap * 2;
                xPos += catWidth + catGap;
                catPositions.push_back(pos);
            }
        }

        if (!catPositions.empty()) {
            for (size_t i = 0; i < mCategories.size(); i++) {
                // Mod math stuff
                const float modWidth = catWidth;
                const float modHeight = catHeight;
                float moduleY = -catPositions[i].yOffset;

                // Get all the modules and populate our vector
                const auto& modsInCategory = ClickGUIManager::getModulesInCategory(mCategories[i], mModules);

                // Calculate the catRect pos
                Vector4<float> catRect = Vector4<float>(catPositions[i].x, catPositions[i].y, catPositions[i].x + catWidth, catPositions[i].y + catHeight)
                    .scaleToPoint(Vector4<float>(ImRenderUtil::getScreenSize().x / 2, ImRenderUtil::getScreenSize().y / 2, ImRenderUtil::getScreenSize().x / 2, ImRenderUtil::getScreenSize().y / 2), inScale);

                /* Calculate the height of the catWindow including the settings */
                float settingsHeight = 0;

                int ModulesCount = 0;

                for (const auto& mod : modsInCategory) {
                    for (const auto& setting : mod->getSettings()) {
                        if (!setting->shouldRender())
                            continue;

                        switch (setting->getType())
                        {
                        case SettingType::BOOL: {
                            settingsHeight = MathUtil::lerp(settingsHeight, settingsHeight + modHeight, mod->mClickGUIAnim);
                            break;
                        }
                        case SettingType::ENUM: {
                            settingsHeight = MathUtil::lerp(settingsHeight, settingsHeight + 60.f, mod->mClickGUIAnim);
                            break;
                        }
                        case SettingType::SLIDER: {
                            settingsHeight = MathUtil::lerp(settingsHeight, settingsHeight + 40.f, mod->mClickGUIAnim);
                            break;
                        }
                        }
                    }
                }

                float catWindowHeight = catHeight + modHeight * modsInCategory.size() + settingsHeight;
                Vector4<float> catWindow = Vector4<float>(catPositions[i].x, catPositions[i].y, catPositions[i].x + catWidth, catPositions[i].y + moduleY + catWindowHeight)
                    .scaleToPoint(Vector4<float>(ImRenderUtil::getScreenSize().x / 2, ImRenderUtil::getScreenSize().y / 2, ImRenderUtil::getScreenSize().x / 2, ImRenderUtil::getScreenSize().y / 2), inScale);

                ImRenderUtil::fillRectangle(catWindow, mSettingsBackgroundColor, animation, 15);

                Color rgb = ColorUtil::getAstolfoRainbow(2.5, 1 * 0.4, 1, i * 25);

                // Can we scroll?
                if (ImRenderUtil::isMouseOver(catWindow) && catPositions[i].isExtended) {
                    if (scrollDirection > 0) {
                        catPositions[i].scrollEase += scrollDirection * catHeight;
                        if (catPositions[i].scrollEase > catWindowHeight - modHeight * 2)
                            catPositions[i].scrollEase = catWindowHeight - modHeight * 2;
                    }
                    else if (scrollDirection < 0) {
                        catPositions[i].scrollEase += scrollDirection * catHeight;
                        if (catPositions[i].scrollEase < 0)
                            catPositions[i].scrollEase = 0;
                    }
                    scrollDirection = 0;
                }

                // Lerp the category extending
                if (!catPositions[i].isExtended) {
                    catPositions[i].scrollEase = catWindowHeight - catHeight;
                    catPositions[i].wasExtended = false;
                }
                else if (!catPositions[i].wasExtended) {
                    catPositions[i].scrollEase = 0;
                    catPositions[i].wasExtended = true;
                }

                // Lerp the scrolling cuz smooth
                catPositions[i].yOffset = MathUtil::animate(catPositions[i].scrollEase, catPositions[i].yOffset, ImRenderUtil::getDeltaTime() * 10.5);

                for (const auto& mod : modsInCategory) {
                    ModulesCount++;

                    float ModuleRounding = 0.f;

                    if (ModulesCount == modsInCategory.size() && !mod->mSettingsExtended)
                        ModuleRounding = 15.f;

                    Color rgb = ColorUtil::getClientColor(2.5, 0.5f, 1, moduleY * 2.f);

                    // If the mod belongs to the category
                    if (mod->getCategory() == mCategories[i]) {
                        // Calculate the modRect pos
                        Vector4<float> modRect = Vector4<float>(catPositions[i].x, catPositions[i].y + catHeight + moduleY, catPositions[i].x + modWidth, catPositions[i].y + catHeight + moduleY + modHeight)
                            .scaleToPoint(Vector4<float>(ImRenderUtil::getScreenSize().x / 2, ImRenderUtil::getScreenSize().y / 2, ImRenderUtil::getScreenSize().x / 2, ImRenderUtil::getScreenSize().y / 2), inScale);

                        // Animate the setting animation percentage
                        float targetAnim = mod->mSettingsExtended ? 1.f : 0.f;
                        mod->mClickGUIAnim = MathUtil::animate(targetAnim, mod->mClickGUIAnim, ImRenderUtil::getDeltaTime() * 15.5);
                        mod->mClickGUIAnim = MathUtil::clamp(mod->mClickGUIAnim, 0.f, 1.f);

                        // Settings
                        if (mod->mClickGUIAnim > 0.001) {
                            for (const auto& setting : mod->getSettings()) {
                                if (!setting->shouldRender())
                                    continue;

                                switch (setting->getType()) {
                                case SettingType::BOOL: {
                                    moduleY = MathUtil::lerp(moduleY, moduleY + modHeight, mod->mClickGUIAnim);

                                    Vector4<float> rect = Vector4<float>(modRect.x, catPositions[i].y + catHeight + moduleY, modRect.z, catPositions[i].y + catHeight + moduleY + modHeight)
                                        .scaleToPoint(Vector4<float>(modRect.x, ImRenderUtil::getScreenSize().y / 2, modRect.z, ImRenderUtil::getScreenSize().y / 2), inScale);

                                    if (rect.y > catRect.y + 0.5f) {
                                        std::string setName = setting->getName();
                                        //ImRenderUtil::fillRectangle(rect, mSettingsBackgroundColor, animation);

                                        if (ImRenderUtil::isMouseOver(rect) && isEnabled) {
                                            tooltip = setting->getDescription();
                                            if (Util::mLeftDown) {
                                                *(bool*)setting->getValue() = !*(bool*)setting->getValue();
                                                Util::mLeftDown = false;
                                            }
                                        }

                                        setting->mBoolScale = 1;

                                        float scaledWidth = rect.getWidth() * setting->mBoolScale;
                                        float scaledHeight = rect.getHeight() * setting->mBoolScale;
                                        Vector2<float> center = Vector2<float>(rect.x + rect.getWidth() / 2.f, rect.y + rect.getHeight() / 2.f);
                                        Vector4<float> scaledRect = Vector4<float>(center.x - scaledWidth / 2.f, center.y - scaledHeight / 2.f, center.x + scaledWidth / 2.f, center.y + scaledHeight / 2.f);

                                        float cSetRectCentreX = rect.x + ((rect.z - rect.x) - ImRenderUtil::getTextWidth(&setName, textSize)) / 2;
                                        float cSetRectCentreY = rect.y + ((rect.w - rect.y) - textHeight) / 2;

                                        Vector4<float> smoothScaledRect = Vector4<float>(scaledRect.z - 19, scaledRect.y + 5, scaledRect.z - 5, scaledRect.w - 5);//
                                        Vector2<float> circleRect = Vector2<float>(smoothScaledRect.getCenter().x, smoothScaledRect.getCenter().y);

                                        ImRenderUtil::fillShadowCircle(circleRect, 5, *(bool*)setting->getValue() ? rgb : Color(15, 15, 15), mod->mClickGUIAnim, 40, 0);

                                        Vector4<float> booleanRect = Vector4<float>(rect.z - 23.5f, cSetRectCentreY - 2.5f, rect.z - 5, cSetRectCentreY + 17.5f);

                                        if (*(bool*)setting->getValue()) {
                                            ImRenderUtil::drawCheckMark(Vector2<float>(booleanRect.getCenter().x - 4, booleanRect.getCenter().y - 1), 1.3, rgb, mod->mClickGUIAnim);
                                            ImRenderUtil::drawCheckMark(Vector2<float>(booleanRect.getCenter().x - 4, booleanRect.getCenter().y - 1), 1.3, rgb, mod->mClickGUIAnim);
                                        }

                                        ImRenderUtil::drawText(Vector2(rect.x + 5.f, cSetRectCentreY), &setName, Color(255, 255, 255), textSize, mod->mClickGUIAnim, true);
                                    }
                                    break;
                                }
                                case SettingType::ENUM: {
                                    std::string setName = setting->getName();
                                    std::vector<std::string> enumValues = setting->getEnumValues();
                                    int* iterator = setting->getIterator();
                                    int numValues = static_cast<int>(enumValues.size());

                                    // Increment the yOffset
                                    moduleY = MathUtil::lerp(moduleY, moduleY + 60.f, mod->mClickGUIAnim);

                                    Vector4<float> rect = Vector4<float>(modRect.x, catPositions[i].y + catHeight + moduleY - 25.f, modRect.z, catPositions[i].y + catHeight + moduleY + modHeight)
                                        .scaleToPoint(Vector4<float>(modRect.x, ImRenderUtil::getScreenSize().y / 2, modRect.z, ImRenderUtil::getScreenSize().y / 2), inScale);

                                    // Animate the setting animation percentage
                                    float targetAnim = setting->mEnumExtended && mod->mSettingsExtended ? 1.f : 0.f;
                                    setting->mEnumSlide = MathUtil::animate(targetAnim, setting->mEnumSlide, ImRenderUtil::getDeltaTime() * 10);
                                    setting->mEnumSlide = MathUtil::clamp(setting->mEnumSlide, 0.f, 1.f);

                                    if (rect.y > catRect.y + 0.5f) {
                                        //ImRenderUtil::fillRectangle(rect, mModuleBackgroundColor, animation);

                                        if (ImRenderUtil::isMouseOver(rect) && isEnabled) {
                                            tooltip = setting->getDescription();
                                            if (Util::mLeftDown) {
                                                *iterator = (*iterator + 1) % enumValues.size();
                                            }
                                            else if (Util::mRightDown && mod->mSettingsExtended) {
                                                *iterator = (*iterator - 1) % enumValues.size();
                                            }
                                        }

                                        float cSetRectCentreY = rect.y + ((rect.w - rect.y) - textHeight) / 2;

                                        std::string enumValue = enumValues[*iterator];
                                        std::string settingName = setName;
                                        std::string settingString = enumValue;
                                        auto ValueLen = ImRenderUtil::getTextWidth(&settingString, textSize);

                                        float mSettingNameX = ImRenderUtil::getTextWidth(&settingName, textSize);

                                        Vector2<float> mCenter = Vector2<float>((rect.x + rect.z) / 2, (rect.y + rect.w) / 2);

                                        Vector2<float> mTextPos = Vector2<float>(mCenter.x - mSettingNameX / 2, rect.y + 3);
                                        

                                        Vector4<float> mEnumPos = Vector4<float>(rect.x + 5, rect.w - 35, rect.z - 5, rect.w - 5);

                                        Vector2<float> mEnumTextPos = Vector2<float>(mCenter.x - ValueLen / 2, mEnumPos.y + 6.f);

                                        //ImRenderUtil::fillRectangle(mEnumPos, mEnumBoxColor, mod->mClickGUIAnim, 10);
                                        ImRenderUtil::fillShadowRectangle(mEnumPos, Color(0, 0, 0), 0.85f, 20, 0, 10);
                                        ImRenderUtil::fillGradientRectangleHorizontal(mEnumPos, mEnumBoxColor, mEnumBoxColorSecond, mod->mClickGUIAnim, mod->mClickGUIAnim, 10.f);

                                        ImRenderUtil::drawText(mTextPos, &settingName, Color(255, 255, 255), textSize, mod->mClickGUIAnim, true);
                                        ImRenderUtil::drawText(mEnumTextPos, &settingString, Color(170, 170, 170), textSize, mod->mClickGUIAnim, true);
                                    }

                                    if (rect.y > catRect.y - modHeight) {
                                        ImRenderUtil::fillGradientRectangle(Vector4<float>(rect.x, rect.w, rect.z, rect.w + 10.f * setting->mEnumSlide * animation), Color(0, 0, 0), Color(0, 0, 0), 0.F, 0.45F * mod->mClickGUIAnim);
                                    }
                                    break;
                                }
                                case SettingType::SLIDER: {
                                    const float value = *(float*)setting->getValue();
                                    const float min = setting->getMin();
                                    const float max = setting->getMax();
                                    const int number = setting->getNumber();

                                    std::ostringstream oss;
                                    oss << std::fixed << std::setprecision(number) << (value);
                                    std::string str = oss.str();

                                    std::string rVal = str;

                                    std::string getName = setting->getName();
                                    std::string valueName = rVal;

                                    auto ValueLen = ImRenderUtil::getTextWidth(&valueName, textSize);

                                    moduleY = MathUtil::lerp(moduleY, moduleY + 40.f, mod->mClickGUIAnim);

                                    Vector4<float> rect = Vector4<float>(modRect.x, (catPositions[i].y + catHeight + moduleY - 5.f), modRect.z, catPositions[i].y + catHeight + moduleY + modHeight);
                                    //.scaleToPoint(Vector4<float>(modRect.x, ImRenderUtil::getScreenSize().y / 2, modRect.z, ImRenderUtil::getScreenSize().y / 2), inScale);

                                    if (rect.y > catRect.y + 0.5f)
                                    {
                                        Vector4<float> mSliderRect = Vector4<float>(rect.x + 5, rect.y, rect.z - 5, rect.w);
                                        //ImRenderUtil::fillRectangle(rect, mSettingsBackgroundColor, animation);
                                        //ImRenderUtil::fillRectangle(rect, settingsBackgroundColor, animation);

                                        const float sliderPos = (value - min) / (max - min) * (mSliderRect.z - mSliderRect.x);

                                        setting->mSliderEase = MathUtil::animate(sliderPos, setting->mSliderEase, ImRenderUtil::getDeltaTime() * 10);
                                        setting->mSliderEase = std::clamp(setting->mSliderEase, 0.f, mSliderRect.getWidth());

                                        if (ImRenderUtil::isMouseOver(rect) && isEnabled) {
                                            tooltip = setting->getDescription();
                                            if (Util::mLeftDown) {
                                                setting->mIsDragging = true;
                                            }
                                        }

                                        if (Util::mLeftClick && setting->mIsDragging && isEnabled) {
                                            const float newValue = std::fmax(std::fmin((ImRenderUtil::getMousePos().x - rect.x) / (rect.z - rect.x) * (max - min) + min, max), min);
                                            *(float*)setting->getValue() = newValue;
                                        }
                                        else {
                                            setting->mIsDragging = false;
                                        }

                                        Vector4<float> newbarlol = Vector4<float>(rect.x + 2, rect.y, rect.x + setting->mSliderEase - 5.f, rect.w - 3);

                                        ImRenderUtil::fillRectangle(Vector4<float>(mSliderRect.x, mSliderRect.y + 24, mSliderRect.z, mSliderRect.w - 9), Color(30, 30, 30), mod->mClickGUIAnim, 20);
                                        ImRenderUtil::drawRoundRect(Vector4<float>(mSliderRect.x, mSliderRect.y + 24, mSliderRect.z, mSliderRect.w - 9), 0, 20, Color(40, 40, 40), mod->mClickGUIAnim, 1);
                                        ImRenderUtil::fillRectangle(Vector4<float>(mSliderRect.x, mSliderRect.y + 25, mSliderRect.x + setting->mSliderEase, mSliderRect.w - 10), rgb, mod->mClickGUIAnim, 20);

                                        float mCirclePosX = mSliderRect.x + setting->mSliderEase - 5.5f;

                                        if ((max / 2) > value) {
                                            mCirclePosX = mSliderRect.x + setting->mSliderEase + 5.5f;
                                        }

                                        Vector2<float> mCirclePos = Vector2<float>(mCirclePosX, mSliderRect.y + 26.25f);
                                        ImRenderUtil::fillCircle(mCirclePos, 5.5f, Color(22, 22, 22), 1.f, 12.f);
                                        ImRenderUtil::drawCircle(mCirclePos, 5.5f, rgb, 1.f, 12.f, 2.f);

                                        //ImRenderUtil::fillShadowRectangle(Vector4<float>(rect.x, (catPositions[i].y + catHeight + moduleY + modHeight) - 3, rect.x + setting->mSliderEase, rect.w), rgb, mod->mClickGUIAnim, 50.f, 0);
                                        //ImRenderUtil::fillRectangleCustom(Vector4<float>(newbarlol.z - 6.f, newbarlol.y - 2, newbarlol.z + 6, newbarlol.w + 2), Color(255, 255, 255), animation, Vector4<float>(20, 20, 20, 20));
                                        ImRenderUtil::drawText(Vector2<float>((rect.z - 5.f) - ValueLen, rect.y + 2.5), &valueName, Color(170, 170, 170), textSize, mod->mClickGUIAnim, true);
                                        ImRenderUtil::drawText(Vector2<float>(rect.x + 5.f, rect.y + 2.5), &getName, Color(255, 255, 255), textSize, mod->mClickGUIAnim, true);
                                    }
                                    break;
                                }
                                }
                            }
                        }

                        mod->mSettingsExtendAnimation = MathUtil::animate(settingsHeight, mod->mSettingsExtendAnimation, ImRenderUtil::getDeltaTime() * 10);

                        if (modRect.y > catRect.y + 0.5f)
                        {
                            ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
                            // Draw the rect
                            if (mod->mClickGUIScale <= 1) {
                                ImRenderUtil::fillRectangle(modRect, mModuleBackgroundColor, animation, ModuleRounding, ImDrawFlags_RoundCornersBottom);
                            }

                            std::string mModuleName = mod->getName();

                            // Calculate the centre of the rect
                            Vector2<float> center = Vector2<float>(modRect.x + modRect.getWidth() / 2.f, modRect.y + modRect.getHeight() / 2.f);

                            mod->mClickGUIScale = MathUtil::animate(mod->isEnabled() ? 1 : 0, mod->mClickGUIScale, ImRenderUtil::getDeltaTime() * 10);

                            // Calculate scaled size based on mClickGUIScale
                            float scaledWidth = modRect.getWidth() * mod->mClickGUIScale;
                            float scaledHeight = modRect.getHeight() * mod->mClickGUIScale;

                            Vector4<float> scaledRect = Vector4<float>(center.x - scaledWidth / 2.f, center.y - scaledHeight / 2.f, center.x + scaledWidth / 2.f, center.y + scaledHeight / 2.f);

                            if (mod->mClickGUIScale > 0) {
                                ImRenderUtil::fillRectangle(scaledRect, rgb, animation * mod->mClickGUIScale, ModuleRounding, ImDrawFlags_RoundCornersBottom);
                                ImRenderUtil::fillShadowRectangle(scaledRect, rgb, animation* mod->mClickGUIScale, 30, ImDrawFlags_RoundCornersBottom, ModuleRounding);
                                //ImRenderUtil::fillGradientRectangleFlag(scaledRect, ColorUtil::getClientColor(2.5, 0.5f, 1, 1), ColorUtil::getClientColor(2.5, 0.5f, 1, 400), ModuleRounding, mod->mClickGUIScale, mod->mClickGUIScale, ImDrawFlags_RoundCornersBottom);
                            }

                            float cRectCentreX = modRect.x + ((modRect.z - modRect.x) - ImRenderUtil::getTextWidth(&mModuleName, textSize)) / 2;
                            float cRectCentreY = modRect.y + ((modRect.w - modRect.y) - textHeight) / 2;

                            Vector2<float> mModuleTextPos = Vector2<float>(cRectCentreX, cRectCentreY);

                            ImRenderUtil::drawText(mModuleTextPos, &mModuleName, Color(mod->isEnabled() ? Color(255, 255, 255) : Color(255, 255, 255)).lerp(Color(180, 180, 180), mod->mClickGUIAnim), textSize, animation, true);

                            if (ImRenderUtil::isMouseOver(modRect) && catPositions[i].isExtended && isEnabled)
                            {
                                if (ImRenderUtil::isMouseOver(catWindow) && catPositions[i].isExtended) {
                                    tooltip = mod->getDescription();
                                }

                                if (Util::mLeftDown)
                                {
                                    mod->toggle();
                                    Util::mLeftDown = false;
                                    //particleMgr.addParticles(5, GuiInfo::MousePos.x, GuiInfo::MousePos.y, 80, 2.f);
                                }
                                else if (Util::mRightClick)
                                {
                                    mod->mSettingsExtended = !mod->mSettingsExtended;
                                    Util::mRightClick = false;
                                }
                            }
                            ImGui::PopFont();
                        }
                        if (modRect.y > catRect.y - modHeight) {
                            // Render a slight glow effect
                            ImRenderUtil::fillGradientRectangle(Vector4<float>(modRect.x, modRect.w, modRect.z, modRect.w + 10.f * mod->mClickGUIAnim * animation), Color(0, 0, 0), Color(0, 0, 0), 0.F * animation, 0.45F * animation);
                        }
                        moduleY += modHeight;
                    }
                }

                std::string catName = mCategories[i];

                if (ImRenderUtil::isMouseOver(catRect) && Util::mRightDown)
                    catPositions[i].isExtended = !catPositions[i].isExtended;

                ImRenderUtil::fillRectangle(catRect, mCategoryHeaderColor, animation, 15, ImDrawFlags_RoundCornersTop);

                ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);

                // Calculate the centre of the rect
                float cRectCentreX = catRect.x + ((catRect.z - catRect.x) - ImRenderUtil::getTextWidth(&catName, textSize * 1.15)) / 2;
                float cRectCentreY = catRect.y + ((catRect.w - catRect.y) - textHeight) / 2;

                // Draw the string
                ImRenderUtil::drawText(Vector2(cRectCentreX, cRectCentreY), &catName, Color(255, 255, 255), textSize * 1.14, animation, false);

                ImGui::PopFont();

#pragma region DraggingLogic
                static bool dragging = false;
                static Vector2<float> dragOffset;
                if (catPositions[i].mIsDragging)
                {
                    if (Util::mLeftClick)
                    {
                        if (!dragging)
                        {
                            dragOffset = Vector2<float>(ImRenderUtil::getMousePos().x - catRect.x, ImRenderUtil::getMousePos().y - catRect.y);
                            dragging = true;
                        }
                        Vector2<float> newPosition = Vector2<float>(ImRenderUtil::getMousePos().x - dragOffset.x, ImRenderUtil::getMousePos().y - dragOffset.y);
                        newPosition.x = std::clamp(newPosition.toFloat().x, 0.f, ImRenderUtil::getScreenSize().x - catWidth);
                        newPosition.y = std::clamp(newPosition.toFloat().y, 0.f, ImRenderUtil::getScreenSize().y - catHeight);
                        catPositions[i].x = newPosition.x;
                        catPositions[i].y = newPosition.y;
                    }
                    else
                    {
                        catPositions[i].mIsDragging = false;
                        dragging = false;
                    }
                }
                else if (ImRenderUtil::isMouseOver(catRect) && Util::mLeftDown && isEnabled)
                {
                    Util::mLeftDown = false;
                    catPositions[i].mIsDragging = true;
                    dragOffset = Vector2<float>(ImRenderUtil::getMousePos().x - catRect.x, ImRenderUtil::getMousePos().y - catRect.y);
                }
#pragma endregion
            }

            static float ease = 1.f;

            if (!tooltip.empty()) {
                float textWidth = ImRenderUtil::getTextWidth(&tooltip, textSize * 0.8f);
                float textHeight = ImRenderUtil::getTextHeight(textSize * 0.8f);
                float padding = 3.5f;
                float offset = 9.f;

                static std::string currentTooltip = "";

                if (currentTooltip != tooltip) {
                    ease = 0;
                    currentTooltip = tooltip;
                }

                if (ImGui::IsMouseDown(0) || ImGui::IsMouseDown(1))
                {
                    ease = MathUtil::animate(0.0f, ease, ImRenderUtil::getDeltaTime() * 15);
                }
                else
                {
                    ease = MathUtil::animate(1.f, ease, ImRenderUtil::getDeltaTime() * 15);
                }

                Vector4<float> tooltipRect = Vector4<float>(
                    ImRenderUtil::getMousePos().x + offset - padding,
                    ImRenderUtil::getMousePos().y + textHeight / 2 - textHeight - padding,
                    ImRenderUtil::getMousePos().x + offset + textWidth + padding * 2,
                    ImRenderUtil::getMousePos().y + textHeight / 2 + padding
                ).scaleToPoint(Vector4<float>(
                    ImRenderUtil::getScreenSize().x / 2,
                    ImRenderUtil::getScreenSize().y / 2,
                    ImRenderUtil::getScreenSize().x / 2,
                    ImRenderUtil::getScreenSize().y / 2
                ), inScale);

                Vector4<float> easedTooltipRect = tooltipRect.scaleToCenterX(ease);

                ImGui::GetBackgroundDrawList()->PushClipRect(ImVec2(easedTooltipRect.x, easedTooltipRect.y), ImVec2(easedTooltipRect.z, easedTooltipRect.w), true);

                ImRenderUtil::fillRectangle(easedTooltipRect, Color(20, 20, 20), ease, ease);
                ImRenderUtil::drawText(Vector2(tooltipRect.x + padding, tooltipRect.y + padding), &tooltip, Color(255, 255, 255), textSize * 0.8f, ease, true);

                ImGui::GetBackgroundDrawList()->PopClipRect();
            }
            else {
                ease = MathUtil::animate(0.0f, ease, ImRenderUtil::getDeltaTime() * 10);
            }

            if (isEnabled) {
                Util::mLeftDown = false;
                Util::mRightDown = false;
                Util::mRightClick = false;
                InstanceManager::get<ClientInstance>()->releaseMouse();
            }
        }

        ImGui::PopFont();
    }
};
