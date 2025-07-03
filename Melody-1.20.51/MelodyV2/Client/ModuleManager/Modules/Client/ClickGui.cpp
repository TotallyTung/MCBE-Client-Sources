// Created by Tony on 2024-10-10 10:01:54

#include "ClickGui.h"
#include "../../../Client.h"

#include "../../../../Libs/ImFX/imfx.h"

void getModuleListByCategory(Category category, std::vector<Module*>& modList) {
	for (Module* mod : *client->moduleMgr->getModuleList()) {
		if (mod->getCategory() == category) {
			modList.push_back(mod);
		}
	}
}

ClickGui::ClickGui() : Module("ClickGui", "A GUI use to enable/disable modules.", Category::CLIENT, VK_INSERT) {
	addEnumSetting("Mode", "NULL", { "New", "Old" }, &Mode);
	addSlider<float>("Blur", "NULL", ValueType::FLOAT_T, &blurStrength, 0.f, 20.f);
	addBoolCheck("Tooltips", "Show modules/settings tooltips.", &tooltips);
}

ClickGui::~ClickGui() {
	for (ClickWindow* window : windowList) {
		delete window;
	}
}

bool ClickGui::isVisible() {
	return false;
}

void ClickGui::onEnable() {
	mc.getClientInstance()->releasebMouse();
}

void ClickGui::onDisable() {
	if (mc.getLocalPlayer() != nullptr) mc.getClientInstance()->grabMouse();
	isSearching = false;
	searchingModule.clear();
	if (isChoosingKeyBindPtr != nullptr) *isChoosingKeyBindPtr = false;
}

void ClickGui::onMouseUpdate(char mouseButton, bool isDown) {
	if (mouseButton == 1) {
		isLeftClickDown = isDown;
		isHoldingLeftClick = isDown;
	}
	else if (mouseButton == 2) {
		isRightClickDown = isDown;
		isHoldingRightClick = isDown;
	}
}

void ClickGui::onKey(int key, bool isDown) {
	if (key == VK_SHIFT) isShiftDown = isDown;
	if (key == VK_BACK) isBackKeyDown = isDown;
	if (isDown) {
		if (key < 192) {
			lastKeyPress = key;
			if (isChoosingKeyBindPtr != nullptr && (*isChoosingKeyBindPtr))
				return;
		}
		if (key == this->getKeybind() || key == VK_ESCAPE) {
			this->setEnabled(false);
			return;
		}
		if (isSearching) {
			static auto isValid = [](char c) -> bool {
				if (c >= '0' && c <= ' 9') return true;
				if (c >= 'a' && c <= 'z') return true;
				if (c >= 'A' && c <= 'Z') return true;
				return false;
			};
			if (key == VK_BACK && !searchingModule.empty()) searchingModule.pop_back();
			else if (key == ' ') searchingModule += ' ';
			else if (isValid((char)key)) {
				if (isShiftDown) searchingModule += (char)key;
				else searchingModule += std::tolower(key);
			}
		}
	}
}

void ClickGui::init() {
	Vec2<float> startPos = Vec2<float>(25.f, 25.f);
	this->windowList.emplace_back(new ClickWindow(startPos, Category::COMBAT));
	startPos.x += 220.f;
	this->windowList.emplace_back(new ClickWindow(startPos, Category::MISC));
	startPos.x += 220.f;
	this->windowList.emplace_back(new ClickWindow(startPos, Category::RENDER));
	startPos.x += 220.f;
	this->windowList.emplace_back(new ClickWindow(startPos, Category::MOVEMENT));
	startPos.x += 220.f;
	this->windowList.emplace_back(new ClickWindow(startPos, Category::PLAYER));
	startPos.x += 220.f;
	this->windowList.emplace_back(new ClickWindow(startPos, Category::CLIENT));
}

void ClickGui::render(ImDrawList* drawlist) {
	if (!initClickGui) {
		ClickGui::init();
		initClickGui = true;
	}
	if (!this->isEnabled()) {
		openDuration = 0.f;
		return;
	}
	static Colors* colorsMod = (Colors*)client->moduleMgr->getModule("Colors");
	ClientInstance* ci = mc.getClientInstance();
	ci->releasebMouse();
	ImGuiIO& io = ImGui::GetIO();
	Vec2<float> mousePos = ci->getMousePos();

	static const float windowWidth = 175.f;
	static const float textSize = 1.f;
	const float textHeight = ImGuiUtils::getTextHeight(textSize);
	static const float textPadding = 1.f * textSize;

	float roundValue = (Mode == 0) ? 0.0f : 4.f;

	static const UIColor whiteColor(255, 255, 255, 255);
	static const UIColor semiGrayColor(200, 200, 200, 255);
	static const UIColor selectedColor(255, 255, 255, 15);

	UIColor mainColor(colorsMod->getColor());
	//Color too bright :sob:
	mainColor.r = (int)(mainColor.r / 1.35f);
	mainColor.g = (int)(mainColor.g / 1.35f);
	mainColor.b = (int)(mainColor.b / 1.35f);

	openDuration = Math::lerp(1.f, openDuration, io.DeltaTime * 5.f);

	std::string tooltipString = "NULL";
	if (isLeftClickDown || isRightClickDown) isSearching = false;
	{
		// Render Background
		UIColor bgColor(0, 0, 0, (int)(100 * openDuration));
		if (blurStrength > 0.f) {
			ImFX::Begin(drawlist);
			ImFX::AddBlur(blurStrength * openDuration, ImVec4(0.f, 0.f, io.DisplaySize.x, io.DisplaySize.y));
			ImFX::End();
		}
		ImGuiUtils::fillRectangle(Vec4<float>(0.f, 0.f, io.DisplaySize.x, io.DisplaySize.y), bgColor);

		// Render Window
		for (ClickWindow* window : windowList) {
			// Dragging Logic
			if (window->isDragging) {
				window->pos.x += mousePos.x - startDragPos.x;
				window->pos.y += mousePos.y - startDragPos.y;
				startDragPos = mousePos;
				if (!isHoldingLeftClick) window->isDragging = false;
			}
			// Render category header
			Vec4<float> cRectPos = Vec4<float>(window->pos.x - textPadding,
				window->pos.y - textPadding,
				window->pos.x + windowWidth + textPadding,
				window->pos.y + textHeight + textPadding);
			static const UIColor cbgColor(29, 29, 29, 255);
			ImGuiUtils::fillRectangle(cRectPos, cbgColor, roundValue, ImDrawFlags_RoundCornersTop);
			ImGuiUtils::fillRectangle(Vec4<float>(cRectPos.x, cRectPos.w - textPadding * 3.f, cRectPos.z, cRectPos.w), cbgColor);
			ImGuiUtils::fillRectangle(Vec4<float>(cRectPos.x, cRectPos.w - textPadding * 2.f, cRectPos.z, cRectPos.w), UIColor(36, 36, 36, 255));

			float WtextWidth = ImGuiUtils::getTextWidth(window->windowName, textSize * 1.1f);
			Vec2<float> cTextPos = Vec2<float>(cRectPos.x + (cRectPos.z - cRectPos.x) / 2.f - WtextWidth / 2.f, cRectPos.y);
			ImGuiUtils::drawText(cTextPos, window->windowName, whiteColor, textSize * 1.1f, true);

			if (cRectPos.contains(mousePos)) {
				if (isLeftClickDown) {
					if (!window->isDragging) {
						window->isDragging = true;
						startDragPos = mousePos;
					}
					isLeftClickDown = false;
				}
				else if (isRightClickDown) {
					window->extended = !window->extended;
					isRightClickDown = false;
				}
			}

			float xOffset = cRectPos.x;
			float xEnd = cRectPos.z;
			float yOffset = cRectPos.w;

			if (window->extended) {
				drawlist->PushClipRect(ImVec2(window->pos.x, window->pos.y + textHeight + textPadding), ImVec2(window->pos.x + windowWidth, window->pos.y + ImGui::GetIO().DisplaySize.y * 0.65f));
				yOffset += window->scrolloffset;
				// Render modules
				int index = 0;
				for (Module* mod : window->modList) {
					mainColor = colorsMod->getColor(-index);
					index += colorsMod->getSeperationValue();

					if (!searchingModule.empty()) {
						std::string lowerSearch = searchingModule;
						std::transform(lowerSearch.begin(), lowerSearch.end(), lowerSearch.begin(), ::tolower);

						std::string lowerModule = mod->getModuleName();
						std::transform(lowerModule.begin(), lowerModule.end(), lowerModule.begin(), ::tolower);

						if (lowerModule.find(lowerSearch) == std::string::npos) {
							continue;
						}
					}

					Vec4<float> mRectPos = { xOffset, yOffset, xEnd, yOffset + textHeight + textPadding * 2.f };
					Vec4<float> mRectPos2 = { mRectPos.x + 1.f, mRectPos.y + 1.f, mRectPos.z - 1.f, mRectPos.w - 1.f };
					static const UIColor mbgColor = UIColor(21, 21, 21, 255);
					ImGuiUtils::fillRectangle(mRectPos, mbgColor);

					if (mod->isEnabled()) {
						if (Mode == 0) {
							UIColor altColor = mainColor;
							altColor.a *= 0.03f;
							drawlist->AddRectFilledMultiColor(
								ImVec2(mRectPos2.x, mRectPos2.y),
								ImVec2(mRectPos2.z, mRectPos2.w),
								altColor.toImColor(),
								mainColor.toImColor(),
								mainColor.toImColor(),
								altColor.toImColor()
							);
						}
						else {
							ImGuiUtils::fillRectangle(mRectPos2, mainColor, roundValue);
						}
					}

					Vec2<float> mTextPos = { mRectPos.x + textPadding * 4.f, mRectPos.y + textPadding };
					ImGuiUtils::drawText(
						mTextPos,
						mod->getModuleName(),
						(mod->isEnabled() || mRectPos2.contains(mousePos)) ? whiteColor : semiGrayColor,
						textSize,
						true
					);

					if (mRectPos2.contains(mousePos)) {
						tooltipString = mod->getDescription();
						ImGuiUtils::fillRectangle(mRectPos2, selectedColor, roundValue);

						if (isLeftClickDown) {
							mod->toggle();
							isLeftClickDown = false;
						}
						else if (isRightClickDown) {
							mod->extended = !mod->extended;
							isRightClickDown = false;
						}
					}


					if (mod->extended) {
						//UIColor sColor;
						//sColor.r = (int)(mainColor.r / 1.25f);
						//sColor.g = (int)(mainColor.g / 1.25f);
						//sColor.b = (int)(mainColor.b / 1.25f);
						//sColor.a = mainColor.a;
						yOffset += textHeight + textPadding * 2.f;
						for (Setting* setting : *mod->getSettings()) {
							mainColor = colorsMod->getColor(-index);
							index += 25;

							UIColor sColor;
							sColor.r = (int)(mainColor.r / 1.25f);
							sColor.g = (int)(mainColor.g / 1.25f);
							sColor.b = (int)(mainColor.b / 1.25f);
							sColor.a = mainColor.a;

							Vec4<float> sbgRectPos = Vec4<float>(mRectPos.x, yOffset, mRectPos.z, yOffset + textHeight + textPadding * 2.f);
							Vec4<float> sRectPos = Vec4<float>(sbgRectPos.x + textPadding * 5.f, sbgRectPos.y, sbgRectPos.z - textPadding * 5.f, sbgRectPos.w - textPadding);
							Vec2<float> sTextPos = Vec2<float>(sRectPos.x + textPadding * 4.f, sRectPos.y + textPadding);
							ImGuiUtils::fillRectangle(sbgRectPos, mbgColor);
							if (sRectPos.contains(mousePos)) tooltipString = setting->description;

							switch (setting->settingType) {
							case SettingType::KEYBIND: {
								ImGuiUtils::drawText(sTextPos, setting->settingName + ":", whiteColor, textSize, true);
								if (sRectPos.contains(mousePos)) {
									ImGuiUtils::fillRectangle(sRectPos, selectedColor, roundValue);
									if (isLeftClickDown) {
										if (isChoosingKeyBindPtr != nullptr && isChoosingKeyBindPtr != &setting->isChoosingKeybind)
											*isChoosingKeyBindPtr = false;
										setting->isChoosingKeybind = !setting->isChoosingKeybind;
										isChoosingKeyBindPtr = &setting->isChoosingKeybind;
										isLeftClickDown = false;
									}
									else if (isRightClickDown) {
										if (setting->isChoosingKeybind) {
											setting->valuePtr->_int = 0x0;
											setting->isChoosingKeybind = false;
										}
										isRightClickDown = false;
									}
								}
								std::string textStr;
								if (setting->isChoosingKeybind) {
									int numDots = (static_cast<int>(ImGui::GetTime() * 2) % 3) + 1; // Dynamic number of dots
									textStr = std::string(numDots, '.');
									if (lastKeyPress != -1) {
										setting->valuePtr->_int = lastKeyPress;
										setting->isChoosingKeybind = false;
									}
								}
								else {
									std::string keybindStr(KeyNames[setting->valuePtr->_int]);
									if (setting->valuePtr->_int == 0x0)
										keybindStr = "None";
									textStr = keybindStr;
								}
								ImGuiUtils::drawText(Vec2<float>(sRectPos.z - ImGuiUtils::getTextWidth(textStr, textSize) - textPadding * 4.f, sTextPos.y), textStr, whiteColor, textSize, true);
								yOffset += textHeight + textPadding * 2.f;
								break;
							}
							case SettingType::CLICKME: {
								ImGuiUtils::drawText(sTextPos, setting->settingName + ":", whiteColor, textSize, true);
								if (sRectPos.contains(mousePos)) {
									ImGuiUtils::fillRectangle(sRectPos, selectedColor, roundValue);
									configMgr = new ConfigManager();
									if (isLeftClickDown) {
										static Notifications* NotificationsMod = (Notifications*)client->moduleMgr->getModule("Notifications");
										configMgr->saveConfig();
										NotificationsMod->addNotifBox("Succexfully save config", 5.0f);
										isLeftClickDown = false;
									}
								}
								break;
							}
							case (SettingType::BOOL): {
								if (setting->valuePtr->_bool) {
									if (Mode == 0) drawlist->AddRectFilledMultiColor(ImVec2(sRectPos.x, sRectPos.y), ImVec2(sRectPos.z, sRectPos.w), mainColor.toImColor(), ImColor(0, 0, 0, 0), ImColor(0, 0, 0, 0), mainColor.toImColor());
									else ImGuiUtils::fillRectangle(sRectPos, sColor, roundValue);
								}
								ImGuiUtils::drawText(sTextPos, setting->settingName, (setting->valuePtr->_bool || sRectPos.contains(mousePos)) ? whiteColor : semiGrayColor, textSize, true);
								if (sRectPos.contains(mousePos)) {
									ImGuiUtils::fillRectangle(sRectPos, selectedColor, roundValue);
									if (isLeftClickDown) {
										setting->valuePtr->_bool = !setting->valuePtr->_bool;
										isLeftClickDown = false;
									}
								}
								yOffset += textHeight + textPadding * 2.f;
								break;
							}
							case (SettingType::ENUM): {
								ImGuiUtils::drawText(sTextPos, setting->settingName + ":", whiteColor, textSize, true);
								if (sRectPos.contains(mousePos)) {
									ImGuiUtils::fillRectangle(sRectPos, selectedColor, roundValue);
									if (isLeftClickDown) {
										(*setting->enumValue)++;
										if (*setting->enumValue == setting->enumList.size()) *setting->enumValue = 0;
										isLeftClickDown = false;
									}
									else if (isRightClickDown) {
										(*setting->enumValue)--;
										if (*setting->enumValue == -1) *setting->enumValue = setting->enumList.size() - 1;
										isRightClickDown = false;
									}
								}
								const std::string& enumName = setting->enumList[*setting->enumValue];
								ImGuiUtils::drawText(Vec2<float>(sRectPos.z - ImGuiUtils::getTextWidth(enumName, textSize) - textPadding * 4.f, sTextPos.y), enumName, whiteColor, textSize, true);
								yOffset += textHeight + textPadding * 2.f;
								break;
							}
							case (SettingType::COLOR): {
								ImGuiUtils::drawText(sTextPos, setting->settingName + ":", whiteColor, textSize, true);
								Vec4<float> colorRectPos(sRectPos.z - 20.f, sRectPos.y + 5.f, sRectPos.z - 5.f, sRectPos.w - 5.f);
								ImGuiUtils::fillRectangle(colorRectPos, *setting->colorPtr, roundValue);

								if (sRectPos.contains(mousePos)) {
									ImGuiUtils::fillRectangle(sRectPos, selectedColor, roundValue);
									if (isLeftClickDown) {
										setting->extended = !setting->extended;
										isLeftClickDown = false;
									}
									else if (isRightClickDown) {
										setting->extended = !setting->extended;
										isRightClickDown = false;
									}
								}

								if (setting->extended) {
									Vec4<float> pickerPos(sbgRectPos.x, sbgRectPos.w, sbgRectPos.z, sbgRectPos.w + (textHeight + textPadding * 2.f) * 4.f);
									ImGuiUtils::fillRectangle(pickerPos, mbgColor);
									static const Vec2<float> colorPickerSize = Vec2<float>(136.f, 100.f);
									Vec4<float> colorPickerPos(pickerPos.x + 5.f, pickerPos.y + 5.f, pickerPos.x + 5.f + colorPickerSize.x, pickerPos.y + 5.f + colorPickerSize.y);
									UIColor colorValue(*setting->colorPtr);
									static float r, g, b;

									static const ImU32 c_oColorBlack = ImGui::ColorConvertFloat4ToU32(ImVec4(0.f, 0.f, 0.f, 1.f));
									static const ImU32 c_oColorBlackTransparent = ImGui::ColorConvertFloat4ToU32(ImVec4(0.f, 0.f, 0.f, 0.f));
									static const ImU32 c_oColorWhite = ImGui::ColorConvertFloat4ToU32(ImVec4(1.f, 1.f, 1.f, 1.f));

									static ImVec4 cHueValue(1, 1, 1, 1);
									ImGui::ColorConvertHSVtoRGB(setting->hueDuration, 1, 1, cHueValue.x, cHueValue.y, cHueValue.z);
									ImU32 oHueColor = ImGui::ColorConvertFloat4ToU32(cHueValue);

									drawlist->AddRectFilledMultiColor(
										ImVec2(colorPickerPos.x, colorPickerPos.y),
										ImVec2(colorPickerPos.x + colorPickerSize.x, colorPickerPos.y + colorPickerSize.y),
										c_oColorWhite,
										oHueColor,
										oHueColor,
										c_oColorWhite
									);

									drawlist->AddRectFilledMultiColor(
										ImVec2(colorPickerPos.x, colorPickerPos.y),
										ImVec2(colorPickerPos.x + colorPickerSize.x, colorPickerPos.y + colorPickerSize.y),
										c_oColorBlackTransparent,
										c_oColorBlackTransparent,
										c_oColorBlack,
										c_oColorBlack
									);

									// Draw dot
									Vec4<float> dotPos = Vec4<float>(setting->pos.x + colorPickerPos.x, setting->pos.y + colorPickerPos.y, setting->pos.x + colorPickerPos.x + 3.f, setting->pos.y + colorPickerPos.y + 3.f);
									if (dotPos.x < colorPickerPos.x) {
										dotPos.z -= colorPickerPos.x - dotPos.x;
										dotPos.x -= colorPickerPos.x - dotPos.x;
									}
									if (dotPos.y < colorPickerPos.y) {
										dotPos.w -= colorPickerPos.y - dotPos.y;
										dotPos.y -= colorPickerPos.y - dotPos.y;
									}
									if (dotPos.z > colorPickerPos.z) {
										dotPos.x -= dotPos.z - colorPickerPos.z;
										dotPos.z -= dotPos.z - colorPickerPos.z;
									}
									if (dotPos.w > colorPickerPos.w) {
										dotPos.y -= dotPos.w - colorPickerPos.w;
										dotPos.w -= dotPos.w - colorPickerPos.w;
									}
									ImGuiUtils::fillRectangle(dotPos, whiteColor);
									static const float barWidth = 10.f;
									static const float barHeight = 100.f;
									Vec4<float> huePickerRect(colorPickerPos.z + 5.f, colorPickerPos.y, colorPickerPos.z + barWidth + 5.f, colorPickerPos.y + barHeight);
									static const ImColor colors[] = {
										ImColor(255, 0, 0),
										ImColor(255, 255, 0),
										ImColor(0, 255, 0),
										ImColor(0, 255, 255),
										ImColor(0, 0, 255),
										ImColor(255, 0, 255),
										ImColor(255, 0, 0)
									};

									for (int i = 0; i < 6; i++) {
										drawlist->AddRectFilledMultiColor(ImVec2(huePickerRect.x, huePickerRect.y + (barHeight / 6.f) * i), ImVec2(huePickerRect.z, huePickerRect.y + (barHeight / 6.f) * (i + 1)), colors[i], colors[i], colors[i + 1], colors[i + 1]);
									}
									// Draw hue bar duration
									float posY = huePickerRect.y + (huePickerRect.w - huePickerRect.y) * setting->hueDuration;
									ImGuiUtils::fillRectangle(Vec4<float>(huePickerRect.x, posY - 1.f, huePickerRect.z, posY + 1.f), whiteColor);

									Vec4<float> opacityPickerRect(huePickerRect.z + 5.f, colorPickerPos.y, huePickerRect.z + barWidth + 5.f, colorPickerPos.y + barHeight);
									drawlist->AddRectFilledMultiColor(ImVec2(opacityPickerRect.x, opacityPickerRect.y), ImVec2(opacityPickerRect.z, opacityPickerRect.w), ImColor(colorValue.r, colorValue.g, colorValue.b, 255), ImColor(colorValue.r, colorValue.g, colorValue.b, 255), ImColor(colorValue.r, colorValue.g, colorValue.b, 0), ImColor(colorValue.r, colorValue.g, colorValue.b, 0));
									// Draw opacity bar duration
									float posY2 = opacityPickerRect.y + (opacityPickerRect.w - opacityPickerRect.y) * (1.f - (float)setting->colorPtr->a / 255.f);
									ImGuiUtils::fillRectangle(Vec4<float>(opacityPickerRect.x, posY2 - 1.f, opacityPickerRect.z, posY2 + 1.f), whiteColor);

									if (colorPickerPos.contains(mousePos) && isLeftClickDown) {
										setting->isDragging = true;
										isLeftClickDown = false;
									}
									if (huePickerRect.contains(mousePos) && isLeftClickDown) {
										setting->isDragging2 = true;
										isLeftClickDown = false;
									}
									if (opacityPickerRect.contains(mousePos) && isLeftClickDown) {
										setting->isDragging3 = true;
										isLeftClickDown = false;
									}
									if (setting->isDragging) {
										if (!isHoldingLeftClick) setting->isDragging = false;
										else {
											float x = ((mousePos.x - colorPickerPos.x) / (colorPickerPos.z - colorPickerPos.x) * (colorPickerSize.x));
											float y = ((mousePos.y - colorPickerPos.y) / (colorPickerPos.w - colorPickerPos.y) * (colorPickerSize.y));
											if (x > colorPickerSize.x) x = colorPickerSize.x;
											if (x < 0.f) x = 0.f;
											if (y > colorPickerSize.y) y = colorPickerSize.y;
											if (y < 0.f) y = 0.f;
											setting->pos.x = x;
											setting->pos.y = y;
										}
									}
									if (setting->isDragging2) {
										if (!isHoldingLeftClick) setting->isDragging2 = false;
										else {
											setting->hueDuration = (mousePos.y - huePickerRect.y) / (huePickerRect.w - huePickerRect.y);
											if (setting->hueDuration > 1.f) setting->hueDuration = 1.f;
											else if (setting->hueDuration < 0.f) setting->hueDuration = 0.f;
										}
									}
									if (setting->isDragging3) {
										if (!isHoldingLeftClick) setting->isDragging3 = false;
										else {
											setting->colorPtr->a = (int)((1.f - (mousePos.y - opacityPickerRect.y) / (opacityPickerRect.w - opacityPickerRect.y)) * 255.f);
											if (setting->colorPtr->a > 255) setting->colorPtr->a = 255;
											else if (setting->colorPtr->a < 0) setting->colorPtr->a = 0;
										}
									}

									{ // Update value
										float sat = setting->pos.x / colorPickerSize.x;
										float brightness = 1.f - setting->pos.y / colorPickerSize.y;
										if (setting->pos.y == 0.f) brightness = 1.f;
										ImGui::ColorConvertHSVtoRGB(setting->hueDuration, sat, brightness, r, g, b);
										setting->colorPtr->r = (int)(r * 255.f);
										setting->colorPtr->g = (int)(g * 255.f);
										setting->colorPtr->b = (int)(b * 255.f);
									}

									yOffset += (textHeight + textPadding * 2.f) * 4.f;
								}
								yOffset += textHeight + textPadding * 2.f;
								break;
							}
							case (SettingType::SLIDER): {

								ValueType valueType = setting->valueType;
								if (valueType == ValueType::INT_T) {
									const float minValue = (float)setting->minValue->_int;
									const float maxValue = (float)setting->maxValue->_int - minValue;
									float value = (float)fmax(0, setting->valuePtr->_int - minValue);  // Value is now always > 0 && < maxValue
									if (value > maxValue) value = maxValue;
									value /= maxValue;  // Value is now in range 0 - 1
									const float endXlol = sRectPos.z - sRectPos.x;
									value *= endXlol;
									Vec4<float> sliderRect = sRectPos;
									// Draw Progress
									if (value != 0.f) {
										sliderRect.z = sliderRect.x + value;
										if (Mode == 0) {
											UIColor altColor = mainColor;
											altColor.a = altColor.a * 0.07f;
											drawlist->AddRectFilledMultiColor(ImVec2(sliderRect.x, sliderRect.y), ImVec2(sliderRect.z, sliderRect.w), mainColor.toImColor(), altColor.toImColor(), altColor.toImColor(), mainColor.toImColor());
										}
										else ImGuiUtils::fillRectangle(sliderRect, sColor, roundValue);
									}
									ImGuiUtils::drawText(sTextPos, setting->settingName + ":", whiteColor, textSize, true);
									ImGuiUtils::drawText(Vec2<float>(sRectPos.z - ImGuiUtils::getTextWidth(std::to_string(setting->valuePtr->_int), textSize) - textPadding * 4.f, sTextPos.y), std::to_string(setting->valuePtr->_int), whiteColor, textSize, true);

									if (sRectPos.contains(mousePos) && isLeftClickDown) {
										setting->isDragging = true;
										isLeftClickDown = false;
									}
									if (setting->isDragging) {
										if (!isHoldingLeftClick) setting->isDragging = false;
										else value = mousePos.x - sliderRect.x;
									}
									if (sRectPos.contains(mousePos) || setting->isDragging) ImGuiUtils::fillRectangle(sliderRect, selectedColor, roundValue);
									// Save Value
									{
										value /= endXlol;  // Now in range 0 - 1
										value *= maxValue;
										value += minValue;

										setting->valuePtr->_int = (int)roundf(value);
										if (setting->valuePtr->_int > setting->maxValue->_int) setting->valuePtr->_int = setting->maxValue->_int;
										else if (setting->valuePtr->_int < setting->minValue->_int) setting->valuePtr->_int = setting->minValue->_int;
									}
								}
								else if (valueType == ValueType::FLOAT_T) {
									const float minValue = setting->minValue->_float;
									const float maxValue = setting->maxValue->_float - minValue;
									float value = (float)fmax(0, setting->valuePtr->_float - minValue);  // Value is now always > 0 && < maxValue
									if (value > maxValue) value = maxValue;
									value /= maxValue;  // Value is now in range 0 - 1
									const float endXlol = sRectPos.z - sRectPos.x;
									value *= endXlol;
									Vec4<float> sliderRect = sRectPos;
									// Draw Progress
									if (value != 0.f) {
										sliderRect.z = sliderRect.x + value;
										if (Mode == 0) {
											UIColor altColor = mainColor;
											altColor.a = altColor.a * 0.07f;
											drawlist->AddRectFilledMultiColor(ImVec2(sliderRect.x, sliderRect.y), ImVec2(sliderRect.z, sliderRect.w), mainColor.toImColor(), altColor.toImColor(), altColor.toImColor(), mainColor.toImColor());
										}
										else ImGuiUtils::fillRectangle(sliderRect, sColor, roundValue);
									}
									ImGuiUtils::drawText(sTextPos, setting->settingName + ":", whiteColor, textSize, true);
									char str[20];
									sprintf_s(str, 10, "%.2f", setting->valuePtr->_float);
									ImGuiUtils::drawText(Vec2<float>(sRectPos.z - ImGuiUtils::getTextWidth(std::string(str), textSize) - textPadding * 4.f, sTextPos.y), (std::string(str)), whiteColor, textSize, true);

									if (sRectPos.contains(mousePos) && isLeftClickDown) {
										setting->isDragging = true;
										isLeftClickDown = false;
									}
									if (setting->isDragging) {
										if (!isHoldingLeftClick) setting->isDragging = false;
										else value = mousePos.x - sliderRect.x;
									}
									if (sRectPos.contains(mousePos) || setting->isDragging) ImGuiUtils::fillRectangle(sliderRect, selectedColor, roundValue);
									// Save Value
									{
										value /= endXlol;  // Now in range 0 - 1
										value *= maxValue;
										value += minValue;

										setting->valuePtr->_float = value;
										if (setting->valuePtr->_float > setting->maxValue->_float) setting->valuePtr->_float = setting->maxValue->_float;
										else if (setting->valuePtr->_float < setting->minValue->_float) setting->valuePtr->_float = setting->minValue->_float;
									}
								}
								yOffset += textHeight + textPadding * 2.f;
								break;
							}
							}
						}
					}
					else yOffset += textHeight + textPadding * 2.f;
				}
				drawlist->PopClipRect();
				if (Vec4<float>(window->pos.x, window->pos.y, window->pos.x + windowWidth, window->pos.y + io.DisplaySize.y * 0.65f).contains(mousePos)) {
					if (io.MouseWheel < 0 && window->scrolloffset > cRectPos.w - yOffset) window->scrolloffset += io.MouseWheel * 25.f;
					if (io.MouseWheel > 0 && window->scrolloffset < 0) window->scrolloffset += io.MouseWheel * 20.f;
				}

				if (window->scrolloffset > 0) window->scrolloffset = 0;
			}
			if (yOffset < window->pos.y + io.DisplaySize.y * 0.65f) ImGuiUtils::drawRectangle(Vec4<float>(cRectPos.x, cRectPos.y, cRectPos.z, yOffset), UIColor(36, 36, 36, 255), 2.f, roundValue, ImDrawFlags_RoundCornersTop);
			else ImGuiUtils::drawRectangle(Vec4<float>(cRectPos.x, cRectPos.y, cRectPos.z, window->pos.y + io.DisplaySize.y * 0.65f), UIColor(36, 36, 36, 255), 2.f, roundValue, ImDrawFlags_RoundCornersTop);
		}
	}

	if (this->tooltips && tooltipString != std::string("NULL")) {
		Vec2<float> textPos(mousePos.x + 5.f, mousePos.y + 20.f);
		Vec4<float> rectPos(textPos.x - 5.f, textPos.y, textPos.x + ImGuiUtils::getTextWidth(tooltipString, 0.9f) + 5.f, textPos.y + ImGuiUtils::getTextHeight(0.9f));
		ImGuiUtils::fillRectangle(rectPos, UIColor(21, 21, 21, 255), roundValue);
		ImGuiUtils::drawRectangle(rectPos, UIColor(36, 36, 36, 255), (Mode == 0) ? 1.f : 2.f, roundValue);
		ImGuiUtils::drawText(textPos, tooltipString, whiteColor, 0.9f);
	}

	Vec4<float> searchRegion = Vec4<float>(io.DisplaySize.x / 2.f - 275.f,
										   io.DisplaySize.y  / 1.25f,
										   io.DisplaySize.x / 2.f +  275.f,
										   io.DisplaySize.y);
	//ImGuiUtils::fillRectangle(searchRegion, whiteColor);

	static const float searchWidth = 400.f;
	static const float searchHeight = 40.f;

	static float searchDuration = 1.f;
	static float closeDuration = 3.f;
	if (searchRegion.contains(mousePos)) {
		searchDuration = Math::lerp(1.f, searchDuration, io.DeltaTime * 10.f);
		closeDuration = 3.f;
	}
	else {
		if (!isSearching && searchingModule.empty()) {
			if (closeDuration < 0.f) searchDuration = Math::lerp(0.f, searchDuration, io.DeltaTime * 10.f);
			else closeDuration -= io.DeltaTime;
		}
	}
	static const std::string SearchStr = "Search";
	Vec4<float> searchRectPos = Vec4<float>(io.DisplaySize.x / 2.f - searchWidth / 2.f,
											io.DisplaySize.y - searchHeight / 2.f + 10.f - 50.f * searchDuration,
											io.DisplaySize.x / 2.f + searchWidth / 2.f,
											io.DisplaySize.y  + searchHeight / 2.f + 10.f - 50.f * searchDuration);
	Vec4<float> typeRectPos = Vec4<float>(searchRectPos.x + 5.f,
										  searchRectPos.y + 5.f,
										  searchRectPos.z - ImGuiUtils::getTextWidth(SearchStr, textSize) - 10.f,
									      searchRectPos.w - 5.f);
	if (typeRectPos.contains(mousePos) && isLeftClickDown) {
		if (isChoosingKeyBindPtr != nullptr) *isChoosingKeyBindPtr = false;
		isSearching = true;
		isLeftClickDown = false;
	}
	roundValue = (Mode == 0) ? 0.0f : 7.5f;
	Vec2<float> typeTextPos = Vec2<float>(typeRectPos.x + 5.f, typeRectPos.y + textPadding * 2.f);
	ImGuiUtils::fillRectangle(searchRectPos, UIColor(29, 29, 29, 255), roundValue);
	ImGuiUtils::fillRectangle(typeRectPos, UIColor(21, 21, 21, 255), roundValue);
	ImGuiUtils::drawText(Vec2<float>(typeRectPos.z + 5.f, typeTextPos.y), SearchStr, whiteColor, textSize);

	static float holdTimes = 0.f;
	if (isBackKeyDown) {
		holdTimes += io.DeltaTime;
	}
	else holdTimes = 0.f;

	if (holdTimes > 0.5f) {
		if (!searchingModule.empty() && isSearching) searchingModule.pop_back();
	}

	float textWidth = ImGuiUtils::getTextWidth(searchingModule, textSize);
	if (!searchingModule.empty()) {
		drawlist->PushClipRect(ImVec2(typeRectPos.x + 5.f, typeRectPos.y), ImVec2(typeRectPos.z - 5.f, typeRectPos.w));
		if (typeRectPos.x + textWidth > typeRectPos.z - 15.f) {
			typeTextPos.x -= (typeRectPos.x + textWidth) - (typeRectPos.z - 15.f);
		}
		ImGuiUtils::drawText(typeTextPos, searchingModule, whiteColor, textSize);
		drawlist->PopClipRect();
	}
	static float barXAdd = 0.f;
	float mid = (typeRectPos.y + typeRectPos.w) / 2.f;
	barXAdd = Math::lerp(textWidth, barXAdd, io.DeltaTime * 7.5f);
	Vec4<float> barRect = Vec4<float>(typeTextPos.x + barXAdd,
									  mid - (textSize * 9.f),
									  typeTextPos.x + barXAdd + 2.f,
									  mid + (textSize * 9.f));
	static float barOpacity = 1.f;
	static bool up = false;
	if (!up) {
		barOpacity -= io.DeltaTime * 2.f;
		if (barOpacity < 0.f) {
			barOpacity = 0.f;
			up = true;
		}
	}
	else {
		barOpacity += io.DeltaTime * 2.f;
		if (barOpacity > 1.f) {
			barOpacity = 1.f;
			up = false;
		}
	}
	UIColor barColor(255, 255, 255, (int)(barOpacity * 255.f));
	if (isSearching) ImGuiUtils::fillRectangle(barRect, barColor);
	if (!isSearching && searchingModule.empty()) ImGuiUtils::drawText(typeTextPos, "Search for Module :)", UIColor(125, 125, 125, 255), textSize, false);
	lastKeyPress = -1;
}