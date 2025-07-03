#include "ClickGUI.h"
#include "../../../../../../Utils/TimerUtil.h"
#include "../../../../../../Libs/json.hpp"
#include "../../../ModuleManager.h"

ClickGUI::ClickGUI() : Module("ClickGUI", "Display all modules", Category::CLIENT, VK_INSERT) {
	registerSetting(new ColorSetting("Color", "NULL", &mainColor, UIColor(0, 101, 90), true));
	registerSetting(new ColorSetting("Disabled Color", "NULL", &disabledColor, UIColor(0, 25, 25), false));
	registerSetting(new NumberSetting<float>("Blur", "Background blur intensity", &blurStrength, 20.f, 0.f, 20.f));
	registerSetting(new NumberSetting<float>("Background Alpha", "Background aloha intensity", &alphahhh, 100.f, 0.f, 255.f));
	registerSetting(new NumberSetting<float>("Disabled Alpha", "Background aloha intensity", &alphahhhb, 64.f, 0.f, 255.f));
	registerSetting(new ToggleSetting("Description", "Show Description", &showDescription, true));
	registerSetting(new ToggleSetting("BlurCGUIWindow", "Show Description", &blueWINDOWH, false));

}

ClickGUI::~ClickGUI() {
	for (auto& window : windowList) {
		delete window;
	}
	windowList.clear();
}

ClickGUI::ClickWindow::ClickWindow(std::string windowName, Vec2<float> startPos, Category c) {
	this->name = windowName;
	this->category = c;
	this->pos = startPos;
	this->extended = true;

	for (auto& mod : ModuleManager::moduleList) {
		if (mod->getCategory() == c) {
			this->moduleList.push_back(mod);
		}
	}

	std::sort(this->moduleList.begin(), this->moduleList.end(), [](Module* lhs, Module* rhs) {
		return lhs->getModuleName() < rhs->getModuleName();
		});
}
bool resetbool111;
void ClickGUI::onDisable() {
	Game::clientInstance->grabMouse();
	resetbool111 = false;
	isLeftClickDown = false;
	isRightClickDown = false;
	isHoldingLeftClick = false;
	isHoldingRightClick = false;

	draggingWindowPtr = nullptr;

	capturingKbSettingPtr = nullptr;
	draggingSliderSettingPtr = nullptr;

	openAnim = 0.0f;
}

void ClickGUI::onEnable() {
	Game::clientInstance->releasebMouse();
	openAnim = 0.0f;
	for (auto& window : windowList) {
		window->pos.y = 0;
	}
    startPos = Vec2<float>(25.f, 35.f);
    initialized = true;
}

bool ClickGUI::isVisible() {
	return false;
}

void ClickGUI::onKeyUpdate(int key, bool isDown) {
	if (!isEnabled()) {
		if (key == getKeybind() && isDown) {
			setEnabled(true);
		}
	}
	else {
		if (isDown) {
			if (key < 192) {
				if (capturingKbSettingPtr != nullptr) {
					if (key != VK_ESCAPE)
						*capturingKbSettingPtr->value = key;
					capturingKbSettingPtr = nullptr;
					return;
				}
			}
			if (key == getKeybind() || key == VK_ESCAPE) {
				setEnabled(false);
			}
			if (key == VK_DELETE) {
				resetbool111 = true;
			}
		}
	}
}

void ClickGUI::onMouseUpdate(Vec2<float> mousePosA, char mouseButton, char isDown) {

	//MouseButtons
	//0 = mouse move
	//1 = left click
	//2 = right click
	//3 = middle click
	//4 = scroll   (isDown: 120 (SCROLL UP) and -120 (SCROLL DOWN))

	switch (mouseButton) {
	case 0:
		mousePos = mousePosA;
		break;
	case 1:
		isLeftClickDown = isDown;
		isHoldingLeftClick = isDown;
		break;
	case 2:
		isRightClickDown = isDown;
		isHoldingRightClick = isDown;
		break;
	case 4:
		float moveVec = (isDown < 0) ? -15.f : 15.f;
		for (auto& window : windowList) {
			if (window == draggingWindowPtr)
				continue;

			window->pos.y += moveVec;
			if (resetbool111) {
				window->pos.y = 0;
				resetbool111 = false;
			}
		}
		break;
	}

	if (draggingWindowPtr != nullptr) {
		if (!isHoldingLeftClick)
			draggingWindowPtr = nullptr;
	}

	if (capturingKbSettingPtr != nullptr) {
		if (isRightClickDown) {
			*capturingKbSettingPtr->value = 0;
			capturingKbSettingPtr = nullptr;
			isRightClickDown = false;
		}
	}

	if (draggingSliderSettingPtr != nullptr) {
		if (!isHoldingLeftClick)
			draggingSliderSettingPtr = nullptr;
	}
}

void ClickGUI::InitClickGUI() {
	setEnabled(false);

	windowList.push_back(new ClickWindow("Combat", startPos, Category::COMBAT));
	startPos.x += 265.f;
	windowList.push_back(new ClickWindow("Movement", startPos, Category::MOVEMENT));
	startPos.x += 265.f;
	windowList.push_back(new ClickWindow("Render", startPos, Category::RENDER));
	startPos.x += 265.f;
	windowList.push_back(new ClickWindow("Player", startPos, Category::PLAYER));
	startPos.x += 265.f;
	windowList.push_back(new ClickWindow("World", startPos, Category::WORLD));
	startPos.x += 265.f;
	windowList.push_back(new ClickWindow("Misc", startPos, Category::MISC));
	startPos.x += 265.f;
	windowList.push_back(new ClickWindow("Client", startPos, Category::CLIENT));

	initialized = true;
}


void ClickGUI::Render() {
    if (!initialized)
        return;

    if (Game::canUseMoveKeys())
        Game::clientInstance->releasebMouse();

    static Vec2<float> oldMousePos = mousePos;
    mouseDelta = mousePos.sub(oldMousePos);
    oldMousePos = mousePos;

    Vec2<float> screenSize = Game::clientInstance->guiData->windowSizeReal;
    float deltaTime = D2D::deltaTime;

    float textSize = 0.94f;
    float textPaddingX = 5.f * textSize;
    float textPaddingY = 2.f * textSize;
    float textHeight = D2D::getTextHeight("", textSize);
    std::string descriptionText = "NULL";

    openAnim += deltaTime * 2.f;
    if (openAnim > 1.f)
        openAnim = 1.f;

    if (blurStrength > 0.1f)
        D2D::addBlur(Vec4<float>(0.f, 0.f, screenSize.x, screenSize.y), blurStrength * openAnim, true);
    const float cornerRadius = 15.f;
    D2D::fillRectangle(Vec4<float>(0.f, 0.f, screenSize.x, screenSize.y), UIColor(0, 0, 0, (int)(180 * openAnim)));

    for (auto& window : windowList) {
        if (window == draggingWindowPtr) {
            window->pos = window->pos.add(mouseDelta);
        }

        static CustomFont* customFontMod = ModuleManager::getModule<CustomFont>();
        float fontPercent = (float)customFontMod->fontSize / 25.f;

        const float windowCornerRadius = 14.f;
        const float moduleCornerRadius = 7.f;
        float wbgPaddingXTEST = 3.f * textSize * fontPercent;
        Vec4<float> hRectPos = Vec4<float>(
            window->pos.x + wbgPaddingXTEST,
            window->pos.y,
            window->pos.x + (int)(250.f * fontPercent) + (textPaddingX * 2.f) - wbgPaddingXTEST,
            window->pos.y + textHeight + (textPaddingY * 2.f)
        );

        Vec2<float> hTextPos = Vec2<float>(hRectPos.x + textPaddingX, hRectPos.y + textPaddingY);

        if (hRectPos.contains(mousePos)) {
            if (isLeftClickDown) {
                draggingWindowPtr = window;
                isLeftClickDown = false;
            }
            else if (isRightClickDown) {
                window->extended = !window->extended;
                isRightClickDown = false;
            }
        }
        UIColor headerColor1 = mainColor;

        if (blueWINDOWH) {
            D2D::addBlur(hRectPos, 5.f, true);
        }
        if (window->extended) {
            Vec2<float> triPos1(hRectPos.z - 15.f, hRectPos.y + (hRectPos.w - hRectPos.y) / 2.f + 3.f);
            Vec2<float> triPos2(triPos1.x - 6.f, triPos1.y - 6.f);
            Vec2<float> triPos3(triPos1.x + 6.f, triPos1.y - 6.f);
            D2D::fillTriangle(triPos1, triPos2, triPos3, UIColor(240, 240, 255), 0.f);
        }
        else {
            Vec2<float> triPos1(hRectPos.z - 15.f, hRectPos.y + (hRectPos.w - hRectPos.y) / 2.f - 3.f);
            Vec2<float> triPos2(triPos1.x - 6.f, triPos1.y + 6.f);
            Vec2<float> triPos3(triPos1.x + 6.f, triPos1.y + 6.f);
            D2D::fillTriangle(triPos1, triPos2, triPos3, UIColor(240, 240, 255), 0.f);
        }


        if (window->extended) {
            float moduleSpace = 3.f * textSize * fontPercent;
            float settingSpace = 3.f * textSize * fontPercent;
            float yHeight = 0.f;
            yHeight += moduleSpace;
            for (auto& mod : window->moduleList) {
                yHeight += textHeight + (textPaddingY * 2.f);

                if (mod->extended) {
                    yHeight += settingSpace;
                    for (auto& setting : mod->getSettingList()) {
                        if (setting->type != SettingType::COLOR_S) {
                            yHeight += textHeight + (textPaddingY * 2.f);
                        }
                        else {
                            ColorSetting* colorSetting = static_cast<ColorSetting*>(setting);
                            yHeight += textHeight + (textPaddingY * 2.f);
                            if (colorSetting->extended) {
                                yHeight += settingSpace;
                                for (auto& slider : colorSetting->colorSliders) {
                                    yHeight += textHeight + (textPaddingY * 2.f) + settingSpace;
                                }
                                yHeight -= settingSpace;
                            }
                        }
                        yHeight += settingSpace;
                    }
                }
                yHeight += moduleSpace;
            }
            float wbgPaddingX = 3.f * textSize * fontPercent;
            float gapFix = windowCornerRadius;
            Vec4<float> wbgRectPos = Vec4<float>(
                hRectPos.x + wbgPaddingX,
                hRectPos.w - gapFix,
                hRectPos.z - wbgPaddingX,
                hRectPos.w + yHeight
            );

            D2D::fillRoundedRectangle(wbgRectPos, UIColor(15, 20, 30, alphahhh), windowCornerRadius, 0.f);

            D2D::fillRoundedRectangle(hRectPos, headerColor1, windowCornerRadius, 0.f);
            D2D::fillRoundedRectangle(hRectPos, UIColor(255, 255, 255, (int)(15 * window->selectedAnim)), windowCornerRadius, 0.f);
            D2D::drawText(hTextPos, window->name, UIColor(240, 240, 255), textSize, true);
            if (blueWINDOWH) {
                D2D::addBlur(wbgRectPos, 5.f, true);
            }
            float yOffset = hRectPos.w + moduleSpace;
            for (auto& mod : window->moduleList) {
                float modPaddingX = wbgPaddingX + (3.f * textSize * fontPercent);
                Vec4<float> mRectPos = Vec4<float>(
                    hRectPos.x + modPaddingX,
                    yOffset,
                    hRectPos.z - modPaddingX,
                    yOffset + textHeight + (textPaddingY * 2.f)
                );
                Vec2<float> mTextPos = Vec2<float>(mRectPos.x + textPaddingX, mRectPos.y + textPaddingY);
                if (mRectPos.contains(mousePos)) {
                    descriptionText = mod->getDescription();
                    if (isLeftClickDown) {
                        mod->toggle();
                        isLeftClickDown = false;
                    }
                    else if (isRightClickDown) {
                        mod->extended = !mod->extended;
                        isRightClickDown = false;
                    }
                }
                updateSelectedAnimRect(mRectPos, mod->selectedAnim);
                UIColor moduleBg = mod->isEnabled()
                    ? UIColor(mainColor.r, mainColor.g, mainColor.b, mainColor.a)
                    : UIColor(disabledColor.r, disabledColor.b, disabledColor.g, alphahhhb);
                D2D::fillRoundedRectangle(mRectPos, moduleBg, moduleCornerRadius, 0.f);
                D2D::fillRoundedRectangle(mRectPos, UIColor(255, 255, 255, (int)(15 * mod->selectedAnim)), moduleCornerRadius, 0.f);


                if (blueWINDOWH) {
                    D2D::addBlur(mRectPos, 5.f, true);
                }
                if (mod->extended) {
                    Vec2<float> triPos1(mRectPos.z - 10.f, mRectPos.y + (mRectPos.w - mRectPos.y) / 2.f + 2.f);
                    Vec2<float> triPos2(triPos1.x - 4.f, triPos1.y - 4.f);
                    Vec2<float> triPos3(triPos1.x + 4.f, triPos1.y - 4.f);
                    D2D::fillTriangle(triPos1, triPos2, triPos3, UIColor(240, 240, 255), 0.f);
                }
                else {
                    Vec2<float> triPos1(mRectPos.z - 10.f, mRectPos.y + (mRectPos.w - mRectPos.y) / 2.f - 2.f);
                    Vec2<float> triPos2(triPos1.x - 4.f, triPos1.y + 4.f);
                    Vec2<float> triPos3(triPos1.x + 4.f, triPos1.y + 4.f);
                    D2D::fillTriangle(triPos1, triPos2, triPos3, UIColor(240, 240, 255), 0.f);
                }

                UIColor textColor = mod->isEnabled()
                    ? UIColor(255, 255, 255)
                    : ColorUtil::lerp(UIColor(175, 175, 175), UIColor(220, 220, 220), mod->selectedAnim);

                D2D::drawText(mTextPos, mod->getModuleName(), textColor, textSize, true);
                yOffset += textHeight + (textPaddingY * 2.f);

                if (mod->extended) {
                    yOffset += settingSpace;
                    float startY = yOffset;

                    float settingPaddingX = 8.f * textSize * fontPercent;
                    float settingPaddingZ = 4.f * textSize * fontPercent;

                    for (auto& setting : mod->getSettingList()) {
                        std::string settingName = setting->name;
                        Vec4<float> sRectPos = {
                            mRectPos.x + settingPaddingX,
                            yOffset,
                            mRectPos.z - settingPaddingZ,
                            yOffset + textHeight + (textPaddingY * 2.f)
                        };
                        Vec2<float> sTextPos = { sRectPos.x + textPaddingX, sRectPos.y + textPaddingY };

                        if (sRectPos.contains(mousePos)) {
                            descriptionText = setting->description;
                        }

                        updateSelectedAnimRect(sRectPos, setting->selectedAnim);
                        yOffset += textHeight + (textPaddingY * 2.f);

                        switch (setting->type) {
                        case SettingType::BOOL_S: {
                            ToggleSetting* boolSetting = static_cast<ToggleSetting*>(setting);
                            bool& boolValue = *boolSetting->value;

                            if (sRectPos.contains(mousePos) && isLeftClickDown) {
                                boolValue = !boolValue;
                                isLeftClickDown = false;
                            }

                            if (boolValue) D2D::fillRectangle(sRectPos, mainColor);
                            if (blueWINDOWH) D2D::addBlur(sRectPos, 8.f, true);
                            D2D::drawText(sTextPos, settingName, UIColor(255, 255, 255), textSize);
                            break;
                        }
                        case SettingType::KEYBIND_S: {
                            KeybindSetting* keybindSetting = static_cast<KeybindSetting*>(setting);
                            int& keyValue = *keybindSetting->value;

                            if (sRectPos.contains(mousePos) && isLeftClickDown) {
                                capturingKbSettingPtr = (capturingKbSettingPtr == keybindSetting) ? nullptr : keybindSetting;
                                isLeftClickDown = false;
                            }

                            std::string keybindName = (setting == capturingKbSettingPtr) ? "..." : (keyValue != 0 ? KeyNames[keyValue] : "None");
                            Vec2<float> keybindTextPos = { sRectPos.z - textPaddingX - D2D::getTextWidth(keybindName, textSize), sTextPos.y };

                            D2D::drawText(sTextPos, settingName + ":", UIColor(255, 255, 255), textSize);
                            D2D::drawText(keybindTextPos, keybindName, UIColor(255, 255, 255), textSize);
                            break;
                        }
                        case SettingType::ENUM_S: {
                            EnumSetting* enumSetting = static_cast<EnumSetting*>(setting);
                            int& enumValue = *enumSetting->value;

                            if (sRectPos.contains(mousePos)) {
                                if (isLeftClickDown) {
                                    enumValue = (enumValue + 1) % enumSetting->enumList.size();
                                    isLeftClickDown = false;
                                }
                                else if (isRightClickDown) {
                                    enumValue = (enumValue - 1 + enumSetting->enumList.size()) % enumSetting->enumList.size();
                                    isRightClickDown = false;
                                }
                            }

                            std::string modeName = enumSetting->enumList[enumValue];
                            Vec2<float> modeTextPos = { sRectPos.z - textPaddingX - D2D::getTextWidth(modeName, textSize), sTextPos.y };

                            D2D::drawText(sTextPos, settingName + ":", UIColor(255, 255, 255), textSize);
                            D2D::drawText(modeTextPos, modeName, UIColor(255, 255, 255), textSize);
                            break;
                        }
                        case SettingType::COLOR_S: {
                            ColorSetting* colorSetting = static_cast<ColorSetting*>(setting);

                            if (sRectPos.contains(mousePos) && isRightClickDown) {
                                colorSetting->extended = !colorSetting->extended;
                                isRightClickDown = false;
                            }

                            float colorBoxSize = std::round(textHeight / 1.5f);
                            float colorBoxPaddingX = textPaddingX + (2.f * textSize);
                            Vec4<float> colorBoxRect = { sRectPos.z - colorBoxPaddingX - colorBoxSize, (sRectPos.y + sRectPos.w - colorBoxSize) / 2.f, sRectPos.z - colorBoxPaddingX, (sRectPos.y + sRectPos.w + colorBoxSize) / 2.f };

                            D2D::drawText(sTextPos, settingName + ":", UIColor(255, 255, 255), textSize);
                            D2D::fillRectangle(colorBoxRect, *colorSetting->colorPtr);

                            if (colorSetting->extended) {
                                yOffset += settingSpace;
                                for (auto& slider : colorSetting->colorSliders) {
                                    Vec4<float> colorSliderRect = { sRectPos.x + settingPaddingX, yOffset, sRectPos.z - settingPaddingZ, yOffset + textHeight + (textPaddingY * 2.f) };
                                    Vec2<float> colorSliderTextPos = { colorSliderRect.x + textPaddingX, colorSliderRect.y + textPaddingY };

                                    updateSelectedAnimRect(colorSliderRect, slider->selectedAnim);
                                    if (colorSliderRect.contains(mousePos) && isLeftClickDown) {
                                        draggingSliderSettingPtr = slider;
                                        isLeftClickDown = false;
                                    }

                                    uint8_t& value = *slider->valuePtr;
                                    float minValue = static_cast<float>(slider->minValue);
                                    float maxValue = static_cast<float>(slider->maxValue);

                                    if (draggingSliderSettingPtr == slider) {
                                        float draggingPercent = (mousePos.x - colorSliderRect.x) / (colorSliderRect.z - colorSliderRect.x);
                                        value = static_cast<int>(minValue + std::round((maxValue - minValue) * std::clamp(draggingPercent, 0.f, 1.f)));
                                    }

                                    float valuePercent = (value - minValue) / (maxValue - minValue);
                                    Vec4<float> valueRectPos = { colorSliderRect.x, colorSliderRect.y, colorSliderRect.x + (colorSliderRect.z - colorSliderRect.x) * std::clamp(valuePercent, 0.f, 1.f), colorSliderRect.w };

                                    char valueText[10];
                                    sprintf_s(valueText, 10, "%i", static_cast<int>(value));
                                    std::string valueTextStr(valueText);
                                    Vec2<float> valueTextPos = { colorSliderRect.z - textPaddingX - D2D::getTextWidth(valueTextStr, textSize, (draggingSliderSettingPtr != slider)), colorSliderTextPos.y };

                                    D2D::fillRectangle(valueRectPos, mainColor);
                                    if (blueWINDOWH) {
                                        D2D::addBlur(valueRectPos, 8.f, true);
                                        D2D::addBlur(colorSliderRect, 8.f, true);
                                    }
                                    D2D::fillRectangle(colorSliderRect, UIColor(255, 255, 255, static_cast<int>(25 * slider->selectedAnim)));
                                    D2D::drawText(colorSliderTextPos, slider->name + ":", UIColor(255, 255, 255), textSize);
                                    D2D::drawText(valueTextPos, valueTextStr, UIColor(255, 255, 255), textSize, (draggingSliderSettingPtr != slider));
                                    yOffset += textHeight + (textPaddingY * 2.f) + settingSpace;
                                }
                                yOffset -= settingSpace;
                            }
                            break;
                        }
                        case SettingType::SLIDER_S: {
                            SliderSettingBase* sliderSettingBase = static_cast<SliderSettingBase*>(setting);

                            if (sRectPos.contains(mousePos) && isLeftClickDown) {
                                draggingSliderSettingPtr = sliderSettingBase;
                                isLeftClickDown = false;
                            }
                            static std::unordered_map<SliderSettingBase*, float> animatedValues;
                            static std::unordered_map<SliderSettingBase*, float> animationSpeeds;
                            if (animatedValues.find(sliderSettingBase) == animatedValues.end()) {
                                if (sliderSettingBase->valueType == ValueType::INT_T) {
                                    NumberSetting<int>* intSlider = static_cast<NumberSetting<int>*>(sliderSettingBase);
                                    animatedValues[sliderSettingBase] = static_cast<float>(*intSlider->valuePtr);
                                }
                                else {
                                    NumberSetting<float>* floatSlider = static_cast<NumberSetting<float>*>(sliderSettingBase);
                                    animatedValues[sliderSettingBase] = *floatSlider->valuePtr;
                                }
                                animationSpeeds[sliderSettingBase] = 0.0f;
                            }

                            if (sliderSettingBase->valueType == ValueType::INT_T) {
                                NumberSetting<int>* intSlider = static_cast<NumberSetting<int>*>(sliderSettingBase);
                                int& value = *intSlider->valuePtr;
                                float minValue = static_cast<float>(intSlider->minValue);
                                float maxValue = static_cast<float>(intSlider->maxValue);

                                if (draggingSliderSettingPtr == sliderSettingBase) {
                                    float draggingPercent = (mousePos.x - sRectPos.x) / (sRectPos.z - sRectPos.x);
                                    value = static_cast<int>(minValue + std::round((maxValue - minValue) * std::clamp(draggingPercent, 0.f, 1.f)));
                                    animatedValues[sliderSettingBase] = static_cast<float>(value);
                                }
                                else {
                                    float targetValue = static_cast<float>(value);
                                    float currentValue = animatedValues[sliderSettingBase];
                                    float stiffness = 0.2f;
                                    float damping = 0.5f;
                                    float delta = targetValue - currentValue;
                                    animationSpeeds[sliderSettingBase] += delta * stiffness;
                                    animationSpeeds[sliderSettingBase] *= damping;
                                    animatedValues[sliderSettingBase] += animationSpeeds[sliderSettingBase];
                                    if (fabs(targetValue - currentValue) < 0.1f && fabs(animationSpeeds[sliderSettingBase]) < 0.1f) {
                                        animatedValues[sliderSettingBase] = targetValue;
                                        animationSpeeds[sliderSettingBase] = 0.0f;
                                    }
                                }

                                float animatedPercent = (animatedValues[sliderSettingBase] - minValue) / (maxValue - minValue);
                                Vec4<float> valueRectPos = {
                                    sRectPos.x,
                                    sRectPos.y,
                                    sRectPos.x + (sRectPos.z - sRectPos.x) * std::clamp(animatedPercent, 0.f, 1.f),
                                    sRectPos.w
                                };

                                char valueText[10];
                                sprintf_s(valueText, 10, "%i", value);
                                std::string valueTextStr(valueText);
                                Vec2<float> valueTextPos = {
                                    sRectPos.z - textPaddingX - D2D::getTextWidth(valueTextStr, textSize, (draggingSliderSettingPtr != sliderSettingBase)),
                                    sTextPos.y
                                };

                                D2D::fillRectangle(valueRectPos, mainColor);
                                if (blueWINDOWH) D2D::addBlur(valueRectPos, 8.f, true);
                                D2D::drawText(sTextPos, settingName + ":", UIColor(255, 255, 255), textSize);
                                D2D::drawText(valueTextPos, valueTextStr, UIColor(255, 255, 255), textSize, (draggingSliderSettingPtr != sliderSettingBase));
                            }
                            else if (sliderSettingBase->valueType == ValueType::FLOAT_T) {
                                NumberSetting<float>* floatSlider = static_cast<NumberSetting<float>*>(sliderSettingBase);
                                float& value = *floatSlider->valuePtr;
                                float minValue = floatSlider->minValue;
                                float maxValue = floatSlider->maxValue;

                                if (draggingSliderSettingPtr == sliderSettingBase) {
                                    float draggingPercent = (mousePos.x - sRectPos.x) / (sRectPos.z - sRectPos.x);
                                    value = minValue + (maxValue - minValue) * std::clamp(draggingPercent, 0.f, 1.f);
                                    animatedValues[sliderSettingBase] = value;
                                }
                                else {
                                    float targetValue = value;
                                    float currentValue = animatedValues[sliderSettingBase];
                                    float stiffness = 0.2f;
                                    float damping = 0.5f;
                                    float delta = targetValue - currentValue;
                                    animationSpeeds[sliderSettingBase] += delta * stiffness;
                                    animationSpeeds[sliderSettingBase] *= damping;
                                    animatedValues[sliderSettingBase] += animationSpeeds[sliderSettingBase];
                                    if (fabs(targetValue - currentValue) < 0.01f && fabs(animationSpeeds[sliderSettingBase]) < 0.01f) {
                                        animatedValues[sliderSettingBase] = targetValue;
                                        animationSpeeds[sliderSettingBase] = 0.0f;
                                    }
                                }

                                float animatedPercent = (animatedValues[sliderSettingBase] - minValue) / (maxValue - minValue);
                                Vec4<float> valueRectPos = {
                                    sRectPos.x,
                                    sRectPos.y,
                                    sRectPos.x + (sRectPos.z - sRectPos.x) * std::clamp(animatedPercent, 0.f, 1.f),
                                    sRectPos.w
                                };

                                char valueText[10];
                                sprintf_s(valueText, 10, "%.2f", value);
                                std::string valueTextStr(valueText);
                                Vec2<float> valueTextPos = {
                                    sRectPos.z - textPaddingX - D2D::getTextWidth(valueTextStr, textSize, (draggingSliderSettingPtr != sliderSettingBase)),
                                    sTextPos.y
                                };

                                D2D::fillRectangle(valueRectPos, mainColor);
                                if (blueWINDOWH) D2D::addBlur(valueRectPos, 8.f, true);
                                D2D::drawText(sTextPos, settingName + ":", UIColor(255, 255, 255), textSize);
                                D2D::drawText(valueTextPos, valueTextStr, UIColor(255, 255, 255), textSize, (draggingSliderSettingPtr != sliderSettingBase));
                            }
                            break;
                        }
                        }
                        D2D::fillRoundedRectangle(sRectPos, UIColor(255, 255, 255, static_cast<int>(10 * setting->selectedAnim)), moduleCornerRadius / 2.f, 0.f);
                        yOffset += settingSpace;
                    }

                    float endY = yOffset - settingSpace;
                    float sLineWidth = 2.f * textSize * fontPercent;
                    float sLinePaddingX = 2.f * textSize;
                    Vec2<float> triTop(mRectPos.x + sLinePaddingX, startY);
                    Vec2<float> triBottom(mRectPos.x + sLinePaddingX, endY);
                    Vec2<float> triRight(mRectPos.x + sLinePaddingX + sLineWidth * 2, (startY + endY) / 2.f);
                    D2D::fillTriangle(triTop, triBottom, triRight, UIColor(mainColor.r, mainColor.g, mainColor.b, 150), 0);
                    if (blueWINDOWH) D2D::addBlur(Vec4<float>(triTop.x, triTop.y, triRight.x, triBottom.y), 5.f, true);
                }
                yOffset += moduleSpace;
            }
        }
    }
    if (showDescription && descriptionText != "NULL" && draggingWindowPtr == nullptr && draggingSliderSettingPtr == nullptr) {
        Vec2<float> mousePadding = Vec2<float>(20.f, 20.f);
        float tooltipWidth = D2D::getTextWidth(descriptionText, 0.8f) + 15.f;
        float tooltipHeight = D2D::getTextHeight(descriptionText, 0.8f) + 8.f;
        Vec4<float> rectPos = Vec4<float>(
            mousePos.x + mousePadding.x,
            mousePos.y + mousePadding.y,
            mousePos.x + mousePadding.x + tooltipWidth,
            mousePos.y + mousePadding.y + tooltipHeight
        );
        if (rectPos.z > screenSize.x) {
            rectPos.x = screenSize.x - tooltipWidth - 5.f;
            rectPos.z = screenSize.x - 5.f;
        }
        if (rectPos.w > screenSize.y) {
            rectPos.y = screenSize.y - tooltipHeight - 5.f;
            rectPos.w = screenSize.y - 5.f;
        }
        Vec2<float> textPos = Vec2<float>(rectPos.x + 8.f, rectPos.y + 4.f);
        const float tooltipCornerRadius = 6.f;
        D2D::fillRoundedRectangle(rectPos, UIColor(20, 25, 35, 220), tooltipCornerRadius, 0.f);
        Vec2<float> pointerTip(mousePos.x + 10.f, mousePos.y + 10.f);
        Vec2<float> pointerLeft(pointerTip.x - 5.f, pointerTip.y + 5.f);
        Vec2<float> pointerRight(pointerTip.x + 5.f, pointerTip.y + 5.f);
        D2D::fillTriangle(pointerTip, pointerLeft, pointerRight, UIColor(20, 25, 35, 220), 0.f);
        if (blueWINDOWH) {
            D2D::addBlurWTINT(rectPos, 5.f, UIColor(mainColor.r, mainColor.g, mainColor.b), false);
        }
        D2D::drawText(textPos, descriptionText, UIColor(240, 240, 255), 0.8f, true);
    }
    isLeftClickDown = false;
    isRightClickDown = false;
}
void ClickGUI::updateSelectedAnimRect(Vec4<float>& rect, float& anim) {
	bool shouldUp = rect.contains(mousePos);

	if (draggingWindowPtr != nullptr)
		shouldUp = false;

	if (draggingSliderSettingPtr != nullptr) {
		if (&draggingSliderSettingPtr->selectedAnim != &anim)
			shouldUp = false;
		else
			shouldUp = true;
	}

	//anim += D2D::deltaTime * (shouldUp ? 5.f : -2.f);
	if (shouldUp)
		anim = 1.f;
	else
		anim -= D2D::deltaTime * 2.f;

	if (anim > 1.f)
		anim = 1.f;
	if (anim < 0.f)
		anim = 0.f;
}

using json = nlohmann::json;

void ClickGUI::onLoadConfig(void* confVoid) {
	json* conf = reinterpret_cast<json*>(confVoid);
	std::string modName = this->getModuleName();

	if (conf->contains(modName)) {
		json obj = conf->at(modName);
		if (obj.is_null())
			return;

		if (obj.contains("enabled")) {
			this->setEnabled(obj.at("enabled").get<bool>());
		}

		for (auto& setting : getSettingList()) {
			std::string settingName = setting->name;

			if (obj.contains(settingName)) {
				json confValue = obj.at(settingName);
				if (confValue.is_null())
					continue;

				switch (setting->type) {
				case SettingType::BOOL_S: {
					ToggleSetting* boolSetting = static_cast<ToggleSetting*>(setting);
					(*boolSetting->value) = confValue.get<bool>();
					break;
				}
				case SettingType::KEYBIND_S: {
					KeybindSetting* keybindSetting = static_cast<KeybindSetting*>(setting);
					(*keybindSetting->value) = confValue.get<int>();
					break;
				}
				case SettingType::ENUM_S: {
					EnumSetting* enumSetting = static_cast<EnumSetting*>(setting);
					(*enumSetting->value) = confValue.get<int>();
					break;
				}
				case SettingType::COLOR_S: {
					ColorSetting* colorSetting = static_cast<ColorSetting*>(setting);
					(*colorSetting->colorPtr) = ColorUtil::HexStringToColor(confValue.get<std::string>());
					break;
				}
				case SettingType::SLIDER_S: {
					SliderSettingBase* sliderSettingBase = static_cast<SliderSettingBase*>(setting);
					if (sliderSettingBase->valueType == ValueType::INT_T) {
						NumberSetting<int>* intSlider = static_cast<NumberSetting<int>*>(sliderSettingBase);
						(*intSlider->valuePtr) = confValue.get<int>();
					}
					else if (sliderSettingBase->valueType == ValueType::FLOAT_T) {
						NumberSetting<float>* floatSlider = static_cast<NumberSetting<float>*>(sliderSettingBase);
						(*floatSlider->valuePtr) = confValue.get<float>();
					}
					break;
				}
				}
			}
		}

		for (auto& window : windowList) {
			std::string windowName = window->name;

			if (obj.contains(windowName)) {
				json confValue = obj.at(windowName);
				if (confValue.is_null())
					continue;

				if (confValue.contains("isExtended")) {
					window->extended = confValue["isExtended"].get<bool>();
				}

				if (confValue.contains("pos")) {
					window->pos.x = confValue["pos"]["x"].get<float>();
					window->pos.y = confValue["pos"]["y"].get<float>();
				}
			}
		}
	}
}

void ClickGUI::onSaveConfig(void* confVoid) {
	json* conf = reinterpret_cast<json*>(confVoid);
	std::string modName = this->getModuleName();
	json obj = (*conf)[modName];

	obj["enabled"] = this->isEnabled();

	for (auto& setting : getSettingList()) {
		std::string settingName = setting->name;

		switch (setting->type) {
		case SettingType::BOOL_S: {
			ToggleSetting* boolSetting = static_cast<ToggleSetting*>(setting);
			obj[settingName] = (*boolSetting->value);
			break;
		}
		case SettingType::KEYBIND_S: {
			KeybindSetting* keybindSetting = static_cast<KeybindSetting*>(setting);
			obj[settingName] = (*keybindSetting->value);
			break;
		}
		case SettingType::ENUM_S: {
			EnumSetting* enumSetting = static_cast<EnumSetting*>(setting);
			obj[settingName] = (*enumSetting->value);
			break;
		}
		case SettingType::COLOR_S: {
			ColorSetting* colorSetting = static_cast<ColorSetting*>(setting);
			obj[settingName] = ColorUtil::ColorToHexString((*colorSetting->colorPtr));
			break;
		}
		case SettingType::SLIDER_S: {
			SliderSettingBase* sliderSettingBase = static_cast<SliderSettingBase*>(setting);
			if (sliderSettingBase->valueType == ValueType::INT_T) {
				NumberSetting<int>* intSlider = static_cast<NumberSetting<int>*>(sliderSettingBase);
				obj[settingName] = (*intSlider->valuePtr);
			}
			else if (sliderSettingBase->valueType == ValueType::FLOAT_T) {
				NumberSetting<float>* floatSlider = static_cast<NumberSetting<float>*>(sliderSettingBase);
				obj[settingName] = (*floatSlider->valuePtr);
			}
			break;
		}
		}
	}

	for (auto& window : windowList) {
		obj[window->name]["isExtended"] = window->extended;
		obj[window->name]["pos"]["x"] = window->pos.x;
		obj[window->name]["pos"]["y"] = window->pos.y;
	}

	(*conf)[modName] = obj;
}