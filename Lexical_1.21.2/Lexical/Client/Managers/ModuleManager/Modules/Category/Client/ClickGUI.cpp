#include "ClickGUI.h"
#include "../../../../../../Utils/TimerUtil.h"
#include "../../../../../../Libs/json.hpp"
#include "../../../ModuleManager.h"
bool blueWINDOWH;
ClickGUI::ClickGUI() : Module("ClickGUI", "Display all modules", Category::CLIENT, VK_INSERT) {
	registerSetting(new SliderSetting<float>("Blur", "Background blur intensity", &blurStrength, 4.f, 0.f, 20.f));
	registerSetting(new BoolSetting("Description", "Show Description", &showDescription, true));
	registerSetting(new BoolSetting("BlurCGUIWindow", "Show Description", &blueWINDOWH, false));
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

void ClickGUI::onDisable() {
	Game::clientInstance->grabMouse();

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

	Vec2<float> startPos = Vec2<float>(25.f, 35.f);
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

	Vec2<float> screenSize = Game::clientInstance->guiData->windowSizeReal;	//D2D::getWindowSize();
	float deltaTime = D2D::deltaTime;

	float textSize = 1.f;
	float textPaddingX = 3.f * textSize;
	float textPaddingY = 0.f * textSize;
	float textHeight = D2D::getTextHeight("", textSize);

	std::string descriptionText = "NULL";

	//UIColor mainColor = UIColor(115, 135, 255, 125);	// 135
	static Colors* colorMod = ModuleManager::getModule<Colors>();

	//openAnim = Math::lerp(openAnim, 1.f, deltaTime * 2.5f);
	openAnim += deltaTime * 2.f;
	if (openAnim > 1.f)
		openAnim = 1.f;

	// Background screen
	if (blurStrength > 0.1f)
		D2D::addBlur(Vec4<float>(0.f, 0.f, screenSize.x, screenSize.y), blurStrength * openAnim);
	D2D::fillRectangle(Vec4<float>(0.f, 0.f, screenSize.x, screenSize.y), UIColor(0, 0, 0, (int)(110 * openAnim)));

	// Render every single window
	for (auto& window : windowList) {
		// Logic for window

		if (window == draggingWindowPtr) {
			window->pos = window->pos.add(mouseDelta);
		}

		// Header RectPos
		static CustomFont* customFontMod = ModuleManager::getModule<CustomFont>();
		float fontPercent = (float)customFontMod->fontSize / 25.f;

		Vec4<float> hRectPos = Vec4<float>(window->pos.x,
			window->pos.y,
			window->pos.x + (int)(250.f * fontPercent) + (textPaddingX * 2.f),
			window->pos.y + textHeight + (textPaddingY * 2.f));

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
		int indexForMainColor = 0;
		updateSelectedAnimRect(hRectPos, window->selectedAnim);

		D2D::fillRectangle(hRectPos, colorMod->getColor(indexForMainColor * colorMod->getSeperationValue()));
		D2D::drawText(hTextPos, window->name, UIColor(255, 255, 255), textSize);

		D2D::fillRectangle(hRectPos, UIColor(255, 255, 255, (int)(25 * window->selectedAnim)));

		if (window->extended) {
			float moduleSpace = 2.f * textSize * fontPercent;
			float settingSpace = 2.f * textSize * fontPercent;
			float yHeight = 0.f;
			yHeight += moduleSpace;

			for (auto& mod : window->moduleList) {
				yHeight += textHeight + (textPaddingY * 2.f);

				if (mod->extended) {
					yHeight += settingSpace;
					for (auto& setting : mod->getSettingList()) {
						int currentPage = (mod->modulePagePtr != nullptr) ? *mod->modulePagePtr : -1;

						if (setting->type != SettingType::PAGE_S &&
							setting->name != "Visible" &&
							setting->name != "Keybind" &&
							setting->name != "Toggle" &&
							(mod->modulePagePtr != nullptr && setting->settingPage != currentPage))
							continue;
						if (setting->type != SettingType::COLOR_S) {
							yHeight += textHeight + (textPaddingY * 2.f);
						}
						else {
							ColorSetting* colorSetting = static_cast<ColorSetting*>(setting);
							yHeight += textHeight + (textPaddingY * 2.f);

							if (colorSetting->extended) {
								yHeight += settingSpace;

								yHeight += 120.f;

							}
						}

						yHeight += settingSpace;
					}
				}

				yHeight += moduleSpace;
			}

			// Window background rectPos
			float wbgPaddingX = 2.f * textSize * fontPercent;
			Vec4<float> wbgRectPos = Vec4<float>(hRectPos.x + wbgPaddingX, hRectPos.w, hRectPos.z - wbgPaddingX, hRectPos.w + yHeight);
			D2D::fillRectangle(wbgRectPos, UIColor(0, 0, 0, 75));
			if (blueWINDOWH) {
				D2D::addBlur(wbgRectPos, 8.f);
			}
			float yOffset = hRectPos.w + moduleSpace;
			if (indexForMainColor > 99999) { indexForMainColor = 0; }
			for (auto& mod : window->moduleList) {
				UIColor mainColor = colorMod->getColor(indexForMainColor * colorMod->getSeperationValue());
				if (mod->isEnabled()) {
					indexForMainColor++;
				}
				float modPaddingX = wbgPaddingX + (2.f * textSize * fontPercent);
				Vec4<float> mRectPos = Vec4<float>(hRectPos.x + modPaddingX,
					yOffset,
					hRectPos.z - modPaddingX,
					yOffset + textHeight + (textPaddingY * 2.f));

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

				D2D::fillRectangle(mRectPos, mod->isEnabled() ? mainColor : UIColor(0, 0, 0, 35));
				D2D::drawText(mTextPos, mod->getModuleName(), mod->isEnabled() ? UIColor(255, 255, 255) : ColorUtil::lerp(UIColor(175, 175, 175), UIColor(255, 255, 255), mod->selectedAnim), textSize);
				D2D::fillRectangle(mRectPos, UIColor(255, 255, 255, (int)(25 * mod->selectedAnim)));

				yOffset += textHeight + (textPaddingY * 2.f);

				if (mod->extended) {
					yOffset += settingSpace;
					float startY = yOffset;

					float settingPaddingX = 7.5f * textSize * fontPercent;
					float settingPaddingZ = 3.5f * textSize * fontPercent;

					for (auto& setting : mod->getSettingList()) {
						std::string settingName = setting->name;

						Vec4<float> sRectPos = Vec4<float>(mRectPos.x + settingPaddingX,
							yOffset,
							mRectPos.z - settingPaddingZ,
							yOffset + textHeight + (textPaddingY * 2.f));

						Vec2<float> sTextPos = Vec2<float>(sRectPos.x + textPaddingX, sRectPos.y + textPaddingY);

						if (sRectPos.contains(mousePos)) {
							descriptionText = setting->description;
						}

						updateSelectedAnimRect(sRectPos, setting->selectedAnim);
						int currentPage = -1;
						if (mod->modulePagePtr != nullptr) {
							currentPage = *mod->modulePagePtr;
						}

						if (setting->type != SettingType::PAGE_S &&
							setting->name != "Visible" &&
							setting->name != "Keybind" &&
							setting->name != "Toggle" &&
							(mod->modulePagePtr != nullptr && setting->settingPage != currentPage))
							continue;
						switch (setting->type) {
						case SettingType::BOOL_S: {
							BoolSetting* boolSetting = static_cast<BoolSetting*>(setting);
							bool& boolValue = (*boolSetting->value);

							if (sRectPos.contains(mousePos)) {
								if (isLeftClickDown) {
									boolValue = !boolValue;
									isLeftClickDown = false;
								}
							}

							if (boolValue)
								D2D::fillRectangle(sRectPos, mainColor);

							D2D::drawText(sTextPos, settingName, UIColor(255, 255, 255), textSize);

							yOffset += textHeight + (textPaddingY * 2.f);
							break;
						}
						case SettingType::KEYBIND_S: {
							KeybindSetting* keybindSetting = static_cast<KeybindSetting*>(setting);
							int& keyValue = (*keybindSetting->value);

							if (sRectPos.contains(mousePos)) {
								if (isLeftClickDown) {
									if (capturingKbSettingPtr == keybindSetting)
										capturingKbSettingPtr = nullptr;
									else
										capturingKbSettingPtr = keybindSetting;

									isLeftClickDown = false;
								}
							}

							std::string keybindName;
							if (setting == capturingKbSettingPtr) {
								keybindName = "...";
							}
							else {
								if (keyValue != 0)
									keybindName = KeyNames[keyValue];
								else
									keybindName = "None";
							}

							Vec2<float> keybindTextPos = Vec2<float>(sRectPos.z - textPaddingX - D2D::getTextWidth(keybindName, textSize), sTextPos.y);

							D2D::drawText(sTextPos, settingName + ":", UIColor(255, 255, 255), textSize);
							D2D::drawText(keybindTextPos, keybindName, UIColor(255, 255, 255), textSize);

							yOffset += textHeight + (textPaddingY * 2.f);
							break;
						}
						case SettingType::ENUM_S: {
							EnumSetting* enumSetting = static_cast<EnumSetting*>(setting);
							int& enumValue = (*enumSetting->value);

							if (sRectPos.contains(mousePos)) {
								if (isLeftClickDown) {
									enumValue++;
									if (enumValue > enumSetting->enumList.size() - 1)
										enumValue = 0;
									isLeftClickDown = false;
								}
								else if (isRightClickDown) {
									enumValue--;
									if (enumValue < 0)
										enumValue = (int)enumSetting->enumList.size() - 1;
									isRightClickDown = false;
								}
							}

							std::string modeName = enumSetting->enumList[enumValue];
							Vec2<float> modeTextPos = Vec2<float>(sRectPos.z - textPaddingX - D2D::getTextWidth(modeName, textSize), sTextPos.y);

							D2D::drawText(sTextPos, settingName + ":", UIColor(255, 255, 255), textSize);
							D2D::drawText(modeTextPos, modeName, UIColor(255, 255, 255), textSize);

							yOffset += textHeight + (textPaddingY * 2.f);
							break;
						}
						case SettingType::COLOR_S: {
							ColorSetting* colorSetting = static_cast<ColorSetting*>(setting);

							if (sRectPos.contains(mousePos)) {
								if (isRightClickDown) {
									colorSetting->extended = !colorSetting->extended;
									isRightClickDown = false;
								}
							}

							float colorBoxSize = std::round(textHeight / 1.5f);
							float colorBoxPaddingX = textPaddingX + (2.f * textSize);
							Vec4<float> colorBoxRect = Vec4<float>(sRectPos.z - colorBoxPaddingX - colorBoxSize,
								(sRectPos.y + sRectPos.w - colorBoxSize) / 2.f,
								sRectPos.z - colorBoxPaddingX,
								(sRectPos.y + sRectPos.w + colorBoxSize) / 2.f);

							D2D::drawText(sTextPos, settingName + ":", UIColor(255, 255, 255), textSize);
							D2D::fillRectangle(colorBoxRect, (*colorSetting->colorPtr));

							yOffset += textHeight + (textPaddingY * 2.f);

							if (colorSetting->extended) {
								yOffset += settingSpace;

								float colorStartX = sRectPos.x;
								float colorStartY = yOffset;

								for (auto& slider : colorSetting->colorSliders) {

									Vec4<float> colorSliderRect = Vec4<float>(sRectPos.x + settingPaddingX,
										yOffset,
										sRectPos.z - settingPaddingZ,
										yOffset + textHeight + (textPaddingY * 2.f));

									Vec2<float> colorSliderTextPos = Vec2<float>(colorSliderRect.x + textPaddingX, colorSliderRect.y + textPaddingY);

									updateSelectedAnimRect(colorSliderRect, slider->selectedAnim);

									if (colorSliderRect.contains(mousePos)) {
										if (isLeftClickDown) {
											draggingSliderSettingPtr = slider;
											isLeftClickDown = false;
										}
									}

									uint8_t& value = (*slider->valuePtr);
									float minValue = (float)slider->minValue;
									float maxValue = (float)slider->maxValue;

									if (draggingSliderSettingPtr == slider) {
										float draggingPercent = (mousePos.x - colorSliderRect.x) / (colorSliderRect.z - colorSliderRect.x);

										if (draggingPercent > 1.f)
											draggingPercent = 1.f;
										if (draggingPercent < 0.f)
											draggingPercent = 0.f;

										value = (int)minValue + (int)std::round((maxValue - minValue) * draggingPercent);
									}

									float valuePercent = (value - minValue) / (maxValue - minValue);

									if (valuePercent > 1.f)
										valuePercent = 1.f;
									if (valuePercent < 0.f)
										valuePercent = 0.f;

									Vec4<float> valueRectPos = Vec4<float>(colorSliderRect.x, colorSliderRect.y, colorSliderRect.x + (colorSliderRect.z - colorSliderRect.x) * valuePercent, colorSliderRect.w);

									char valueText[10];
									sprintf_s(valueText, 10, "%i", (int)value);
									std::string valueTextStr(valueText);

									Vec2<float> valueTextPos = Vec2<float>(colorSliderRect.z - textPaddingX - D2D::getTextWidth(valueTextStr, textSize, (draggingSliderSettingPtr != slider)), colorSliderTextPos.y);

									D2D::fillRectangle(valueRectPos, mainColor);
									D2D::drawText(colorSliderTextPos, slider->name + ":", UIColor(255, 255, 255), textSize);
									D2D::drawText(valueTextPos, valueTextStr, UIColor(255, 255, 255), textSize, (draggingSliderSettingPtr != slider));

									D2D::fillRectangle(colorSliderRect, UIColor(255, 255, 255, (int)(25 * slider->selectedAnim)));

									yOffset += textHeight + (textPaddingY * 2.f) + settingSpace;
								}

								yOffset -= settingSpace;

								float colorEndY = yOffset;
								float colorSLineWidth = 4.f * textSize * fontPercent;
								float colorSLinePaddingX = 1.f * textSize;
								Vec4<float> colorSLineRect = Vec4<float>(colorStartX + colorSLinePaddingX,
									colorStartY,
									colorStartX + colorSLinePaddingX + colorSLineWidth,
									colorEndY);

								D2D::fillRectangle(colorSLineRect, mainColor);
							}
							break;
						}
						case SettingType::SLIDER_S: {
							SliderSettingBase* sliderSettingBase = static_cast<SliderSettingBase*>(setting);

							if (sRectPos.contains(mousePos)) {
								if (isLeftClickDown) {
									draggingSliderSettingPtr = sliderSettingBase;
									isLeftClickDown = false;
								}
							}

							if (sliderSettingBase->valueType == ValueType::INT_T) {
								SliderSetting<int>* intSlider = static_cast<SliderSetting<int>*>(sliderSettingBase);
								int& value = (*intSlider->valuePtr);
								float minValue = (float)intSlider->minValue;
								float maxValue = (float)intSlider->maxValue;

								if (draggingSliderSettingPtr == sliderSettingBase) {
									float draggingPercent = (mousePos.x - sRectPos.x) / (sRectPos.z - sRectPos.x);

									if (draggingPercent > 1.f)
										draggingPercent = 1.f;
									if (draggingPercent < 0.f)
										draggingPercent = 0.f;

									value = (int)minValue + (int)std::round((maxValue - minValue) * draggingPercent);
								}

								float valuePercent = (value - minValue) / (maxValue - minValue);

								if (valuePercent > 1.f)
									valuePercent = 1.f;
								if (valuePercent < 0.f)
									valuePercent = 0.f;

								Vec4<float> valueRectPos = Vec4<float>(sRectPos.x, sRectPos.y, sRectPos.x + (sRectPos.z - sRectPos.x) * valuePercent, sRectPos.w);

								char valueText[10];
								sprintf_s(valueText, 10, "%i", value);
								std::string valueTextStr(valueText);

								Vec2<float> valueTextPos = Vec2<float>(sRectPos.z - textPaddingX - D2D::getTextWidth(valueTextStr, textSize, (draggingSliderSettingPtr != sliderSettingBase)), sTextPos.y);

								D2D::fillRectangle(valueRectPos, mainColor);
								D2D::drawText(sTextPos, settingName + ":", UIColor(255, 255, 255), textSize);
								D2D::drawText(valueTextPos, valueTextStr, UIColor(255, 255, 255), textSize, (draggingSliderSettingPtr != sliderSettingBase));
							}
							else if (sliderSettingBase->valueType == ValueType::FLOAT_T) {
								SliderSetting<float>* floatSlider = static_cast<SliderSetting<float>*>(sliderSettingBase);
								float& value = (*floatSlider->valuePtr);
								float minValue = floatSlider->minValue;
								float maxValue = floatSlider->maxValue;

								if (draggingSliderSettingPtr == sliderSettingBase) {
									float draggingPercent = (mousePos.x - sRectPos.x) / (sRectPos.z - sRectPos.x);

									if (draggingPercent > 1.f)
										draggingPercent = 1.f;
									if (draggingPercent < 0.f)
										draggingPercent = 0.f;

									value = minValue + (maxValue - minValue) * draggingPercent;
								}

								float valuePercent = (value - minValue) / (maxValue - minValue);

								if (valuePercent > 1.f)
									valuePercent = 1.f;
								if (valuePercent < 0.f)
									valuePercent = 0.f;

								Vec4<float> valueRectPos = Vec4<float>(sRectPos.x, sRectPos.y, sRectPos.x + (sRectPos.z - sRectPos.x) * valuePercent, sRectPos.w);

								char valueText[10];
								sprintf_s(valueText, 10, "%.2f", value);
								std::string valueTextStr(valueText);

								Vec2<float> valueTextPos = Vec2<float>(sRectPos.z - textPaddingX - D2D::getTextWidth(valueTextStr, textSize, (draggingSliderSettingPtr != sliderSettingBase)), sTextPos.y);

								D2D::fillRectangle(valueRectPos, mainColor);
								D2D::drawText(sTextPos, settingName + ":", UIColor(255, 255, 255), textSize);
								D2D::drawText(valueTextPos, valueTextStr, UIColor(255, 255, 255), textSize, (draggingSliderSettingPtr != sliderSettingBase));
							}

							yOffset += textHeight + (textPaddingY * 2.f);
							break;
						}
						case SettingType::PAGE_S: {
							PageSetting* pageSetting = static_cast<PageSetting*>(setting);
							int& pageValue = (*pageSetting->valuePtr);

							if (sRectPos.contains(mousePos)) {
								if (isLeftClickDown) {
									pageValue++;
									if (pageValue >= (int)pageSetting->pageNames.size())
										pageValue = 0;
									isLeftClickDown = false;
								}
								else if (isRightClickDown) {
									pageValue--;
									if (pageValue < 0)
										pageValue = (int)pageSetting->pageNames.size() - 1;
									isRightClickDown = false;
								}
							}

							std::string pageName = pageSetting->pageNames[pageValue];
							Vec2<float> pageTextPos = Vec2<float>(sRectPos.z - textPaddingX - D2D::getTextWidth(pageName, textSize), sTextPos.y);

							D2D::drawText(sTextPos, settingName + ":", UIColor(255, 255, 255), textSize);
							D2D::drawText(pageTextPos, pageName, UIColor(255, 255, 255), textSize);

							yOffset += textHeight + (textPaddingY * 2.f);
							break;
						}
						}
						D2D::fillRectangle(sRectPos, UIColor(255, 255, 255, (int)(25 * setting->selectedAnim)));
						yOffset += settingSpace;
					}
					float endY = yOffset - settingSpace;

					float sLineWidth = 4.f * textSize * fontPercent;
					float sLinePaddingX = 1.f * textSize;
					Vec4<float> sLineRect = Vec4<float>(mRectPos.x + sLinePaddingX,
						startY,
						mRectPos.x + sLinePaddingX + sLineWidth,
						endY);

					D2D::fillRectangle(sLineRect, mainColor);
				}

				yOffset += moduleSpace;
			}
		}
	}

	if (showDescription && descriptionText != "NULL" && draggingWindowPtr == nullptr && draggingSliderSettingPtr == nullptr) {
		Vec2<float> mousePadding = Vec2<float>(15.f, 15.f);
		Vec4<float> rectPos = Vec4<float>(mousePos.x + mousePadding.x,
			mousePos.y + mousePadding.y,
			mousePos.x + mousePadding.x + D2D::getTextWidth(descriptionText, 0.8f) + 2.f * 2.f,
			mousePos.y + mousePadding.y + D2D::getTextHeight(descriptionText, 0.8f));

		Vec2<float> textPos = Vec2<float>(rectPos.x, rectPos.y).add(Vec2<float>(2.f, 0.f));
		D2D::fillRectangle(rectPos, UIColor(0, 0, 0, 125));
		D2D::drawText(textPos, descriptionText, UIColor(255, 255, 255), 0.8f);

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
					BoolSetting* boolSetting = static_cast<BoolSetting*>(setting);
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
				case SettingType::PAGE_S: {
					PageSetting* pageSetting = static_cast<PageSetting*>(setting);
					(*pageSetting->valuePtr) = confValue.get<int>();
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
						SliderSetting<int>* intSlider = static_cast<SliderSetting<int>*>(sliderSettingBase);
						(*intSlider->valuePtr) = confValue.get<int>();
					}
					else if (sliderSettingBase->valueType == ValueType::FLOAT_T) {
						SliderSetting<float>* floatSlider = static_cast<SliderSetting<float>*>(sliderSettingBase);
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
			BoolSetting* boolSetting = static_cast<BoolSetting*>(setting);
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
		case SettingType::PAGE_S: {
			PageSetting* pageSetting = static_cast<PageSetting*>(setting);
			obj[settingName] = (*pageSetting->valuePtr);
			break;
		}
		case SettingType::SLIDER_S: {
			SliderSettingBase* sliderSettingBase = static_cast<SliderSettingBase*>(setting);
			if (sliderSettingBase->valueType == ValueType::INT_T) {
				SliderSetting<int>* intSlider = static_cast<SliderSetting<int>*>(sliderSettingBase);
				obj[settingName] = (*intSlider->valuePtr);
			}
			else if (sliderSettingBase->valueType == ValueType::FLOAT_T) {
				SliderSetting<float>* floatSlider = static_cast<SliderSetting<float>*>(sliderSettingBase);
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