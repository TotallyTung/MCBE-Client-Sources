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
bool gradientBG = true;
bool showParticles = false;
bool showNetwork = false;
int maxlines = 0;
float minDist = 40.f;
float size = 1.f;
float rounding1 = 0.f;
float gradientlevel = 30.f;
float windowWidth1 = 1.1f;
Gui::Gui() : Module("Gui", "The main menu of the client. t", Category::CLIENT, VK_INSERT) {
	enumsetting("Mode", "NULL", { "New", "Old" }, &Mode);
	slider<float>("Blur", "NULL", ValueType::FLOAT_T, &blurStrength, 0.f, 20.f);
	boolsetting("Tooltips", "Show modules/settings tooltips.", &tooltips);
	boolsetting("Gradient", "NULL", &gradientBG);
	boolsetting("Particles", "NULL", &showParticles);
	boolsetting("Lines", "NULL", &showNetwork);
	slider<int>("Max Lines", "NULL", ValueType::INT_T, &maxlines, 0, 100);
	slider<float>("Line minDist", "NULL", ValueType::FLOAT_T, &minDist, 0.f, 300.f);
	slider<float>("Size", "NULL", ValueType::FLOAT_T, &size, 0.5f, 2.f);
	slider<float>("Window Width", "NULL", ValueType::FLOAT_T, &windowWidth1, 0.5f, 2.f);
	slider<float>("Rounding", "NULL", ValueType::FLOAT_T, &rounding1, 0.f, 10.f);
	slider<float>("Gradient Strength", "NULL", ValueType::FLOAT_T, &gradientlevel, 0.f, 100.f);
}

Gui::~Gui() {
	for (ClickWindow* window : windowList) {
		delete window;
	}
	this->particles.clear();
	this->nodes.clear();
}

bool Gui::isVisible() {
	return false;
}

void Gui::onEnable() {
	mc.getClientInstance()->releasebMouse();
	this->particles.clear();
	this->nodes.clear();
}

void Gui::onDisable() {
	if (mc.getLocalPlayer() != nullptr) mc.getClientInstance()->grabMouse();
	isSearching = false;
	searchingModule.clear();
	if (isChoosingKeyBindPtr != nullptr) *isChoosingKeyBindPtr = false;
	this->particles.clear();
	this->nodes.clear();
}

void Gui::onMouseUpdate(char mouseButton, bool isDown) {
	if (mouseButton == 1) {
		isLeftClickDown = isDown;
		isHoldingLeftClick = isDown;
	}
	else if (mouseButton == 2) {
		isRightClickDown = isDown;
		isHoldingRightClick = isDown;
	}
}

void Gui::onKey(int key, bool isDown) {
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

void Gui::init() {
	Vec2<float> startPos(30.f, 30.f);
	float spacingX = 260.f;
	float spacingY = 260.f;
	int columns = 4;
	int count = 0;

	std::vector<Category> categories = {
		Category::COMBAT, Category::MISC, Category::RENDER, Category::MOVEMENT,
		Category::PLAYER, Category::CLIENT, Category::WORLD, Category::EXPLOIT,
		Category::UTILITY, Category::PACKETS
	};

	for (const auto& category : categories) {
		this->windowList.emplace_back(new ClickWindow(startPos, category));
		count++;

		if (count % columns == 0) {
			startPos.x = 30.f;
			startPos.y += spacingY;
		}
		else {
			startPos.x += spacingX;
		}
	}
}


void Gui::render(ImDrawList* drawlist) {
	if (!initGui) {
		Gui::init();
		initGui = true;
	}
	if (!this->isEnabled()) {
		openDuration = 0.f;
		return;
	}
	static ClientManager* colorsMod = (ClientManager*)client->moduleMgr->getModule("ClientManager");
	ClientInstance* ci = mc.getClientInstance();
	ci->releasebMouse();
	ImGuiIO& io = ImGui::GetIO();
	Vec2<float> mousePos = ci->getMousePos();
	const float baseWidth = 210.f;
	const float windowWidth = baseWidth * windowWidth1 * size;
	const float textSize = 1.2f * 1.3f * size;
	const float textHeight = ImGuiUtils::getTextHeight(textSize * 0.9f);
	const float textPadding = 2.f * textSize;
	float roundValue = (Mode == 0) ? 0.f : rounding1;
	UIColor mainColor = colorsMod->getColor();
	openDuration = Math::lerp(1.f, openDuration, io.DeltaTime * 5.f);
	const UIColor whiteColor(240, 240, 240, 255 * openDuration);
	const UIColor semiGrayColor(160, 160, 160, 220 * openDuration);
	const UIColor selectedColor(50, 120, 255, 5 * openDuration);
	std::string tooltipString = "NULL";
	if (isLeftClickDown || isRightClickDown) isSearching = false;
	{
		UIColor bgColor(0, 0, 0, (int)(100 * openDuration));
		if (blurStrength > 0.f) {
			ImFX::Begin(drawlist);
			ImFX::AddBlur(blurStrength * openDuration, ImVec4(0.f, 0.f, io.DisplaySize.x, io.DisplaySize.y));
			ImFX::End();
		}
		ImGuiUtils::fillRectangle(Vec4<float>(0.f, 0.f, io.DisplaySize.x, io.DisplaySize.y), bgColor);
		{
			if (gradientBG)
				drawlist->AddRectFilledMultiColor(ImVec2(0.f, 0.f), ImGui::GetIO().DisplaySize, ImColor(0, 0, 0, (int)(gradientlevel * 1.f)), ImColor(0, 0, 0, (int)(gradientlevel * 1.f)), ImColor(mainColor.r, mainColor.g, mainColor.b, (int)(gradientlevel * 1.f)), ImColor(mainColor.r, mainColor.g, mainColor.b, (int)(gradientlevel * 1.f)));
		}
		{
			float start = Math::random(0.f, ImGui::GetIO().DisplaySize.x);
			if (showParticles) {
				if (particles.size() < 125) {
					int count = 1 * (int)(ceil(60.f / ImGui::GetIO().Framerate));
					for (int i = 0; i < count; i++)
						particles.emplace_back(new clickGuiParticles(
							Vec2<float>(start, ImGui::GetIO().DisplaySize.y + 10.f),
							Vec2<float>(start, ImGui::GetIO().DisplaySize.y / Math::random(1.5f, 2.5f)),
							Math::random(3.f, 10.f),
							Math::random(1.f, 7.f),
							Math::random(0.f, 360.f)
						));
				}
			}
			for (int i = 0; i < particles.size(); i++) {
				std::shared_ptr<clickGuiParticles> p = particles[i];
				if (p->startPos.y <= p->endPos.y || !showParticles) {
					p->duration = std::max(p->duration - 0.02f, 0.f);
				}
				float alpha = std::max(p->duration, 0.f);
				float angle = p->rotation * (3.14159265f / 180.f);
				float cosA = cos(angle);
				float sinA = sin(angle);
				ImVec2 p1(p->startPos.x, p->startPos.y - p->size);
				ImVec2 p2(p->startPos.x - p->size, p->startPos.y + p->size);
				ImVec2 p3(p->startPos.x + p->size, p->startPos.y + p->size);
				auto rotate = [&](ImVec2& point) {
					float dx = point.x - p->startPos.x;
					float dy = point.y - p->startPos.y;
					point.x = p->startPos.x + dx * cosA - dy * sinA;
					point.y = p->startPos.y + dx * sinA + dy * cosA;
					};
				rotate(p1);
				rotate(p2);
				rotate(p3);
				drawlist->AddTriangleFilled(p1, p2, p3, ImColor(mainColor.r, mainColor.g, mainColor.b, (int)(150 * alpha)));
				p->startPos.y -= p->speed * (60.f / ImGui::GetIO().Framerate);
				if (p->duration <= 0.f) {
					particles.erase(particles.begin() + i);
					i--;
				}
			}
		}

		if (showNetwork) {
			if (nodes.size() < maxlines) {
				for (int i = 0; i < 5; i++) {
					nodes.emplace_back(new networkNode(
						Vec2<float>(Math::random(10.f, io.DisplaySize.x - 50.f),
							Math::random(10.f, io.DisplaySize.y - 50.f)),
						Math::random(3.f, 8.f),
						Math::random(1.f, 3.f)
					));
				}
			}
			for (int i = 0; i < nodes.size(); i++) {
				std::shared_ptr<networkNode> node = nodes[i];
				float movementSpeed = 0.5f;
				node->pos.x += sinf(node->pos.y * 0.01f) * movementSpeed;
				node->pos.y += cosf(node->pos.x * 0.01f) * movementSpeed;
				Vec2<float> repulsion(0.f, 0.f);
				for (int j = 0; j < nodes.size(); j++) {
					if (i == j) continue;
					float distance = (node->pos - nodes[j]->pos).length();
					if (distance < minDist && distance > 0.f) {
						Vec2<float> diff = node->pos - nodes[j]->pos;
						diff = diff / distance;
						repulsion += diff * (minDist - distance) * 0.1f;
					}
				}
				node->pos += repulsion;
				node->pos.x = std::clamp(node->pos.x, 0.f, io.DisplaySize.x);
				node->pos.y = std::clamp(node->pos.y, 0.f, io.DisplaySize.y);
				float circleRadius = node->size * 0.5f;
				drawlist->AddCircleFilled(ImVec2(node->pos.x, node->pos.y), circleRadius,
					ImColor(222, 222, 222, 222));
				float fadeStart = 150.f;
				float maxDistance = 200.f;
				for (int j = i + 1; j < nodes.size(); j++) {
					float distance = (nodes[i]->pos - nodes[j]->pos).length();
					float alphaFactor = 1.0f;
					if (distance > fadeStart) {
						alphaFactor = 1.0f - std::clamp((distance - fadeStart) / (maxDistance - fadeStart), 0.f, 1.f);
					}
					int alpha = (int)(222 * alphaFactor);
					if (alpha > 0) {
						drawlist->AddLine(ImVec2(nodes[i]->pos.x, nodes[i]->pos.y),
							ImVec2(nodes[j]->pos.x, nodes[j]->pos.y),
							ImColor(222, 222, 222, alpha), 1.5f);
					}
				}
			}
		}
		// Render Window
		for (ClickWindow* window : windowList) {
			// Dragging Logic
			if (window->isDragging) {
				window->pos.x += mousePos.x - startDragPos.x;
				window->pos.y += mousePos.y - startDragPos.y;
				startDragPos = mousePos;
				if (!isHoldingLeftClick) window->isDragging = false;
			}
			Vec4<float> cRectPos = Vec4<float>(
				window->pos.x - textPadding - 3.f,
				window->pos.y - textPadding,
				window->pos.x + windowWidth + 3.f + textPadding,
				window->pos.y + textHeight + textPadding
			);
			static const UIColor cbgColor(29, 29, 29, 255);
			const UIColor overallmain(mainColor.r, mainColor.g, mainColor.b, 200.f * openDuration);
			ImGuiUtils::fillRectangle(cRectPos, overallmain, roundValue, ImDrawFlags_RoundCornersTop);
			float WtextWidth = ImGuiUtils::getTextWidth(window->windowName, textSize * 1.1f);
			Vec2<float> cTextPos = Vec2<float>(cRectPos.x + textPadding, cRectPos.y);
			ImGuiUtils::drawText(cTextPos, window->windowName, whiteColor, textSize * 0.9f, true);

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
					float animationFactor = openDuration;
					float startYOffset = window->pos.y;
					float finalYOffset = yOffset;
					float animatedYOffset = startYOffset + (finalYOffset - startYOffset) * animationFactor;
					Vec4<float> mRectPos = { xOffset, animatedYOffset, xEnd, animatedYOffset + textHeight + textPadding * 2.f };
					Vec4<float> mRectPos2 = { mRectPos.x + 1.f, mRectPos.y + 1.f, mRectPos.z - 1.f, mRectPos.w - 1.f };
					static const UIColor mbgColor = UIColor(33, 33, 33, 0);
					static const UIColor mbgColor2 = UIColor(0, 0, 0, 50);
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
							ImGuiUtils::fillRectangle(mRectPos2, overallmain, 0.f);
						}
					}
					if (!mod->isEnabled()) {
						ImGuiUtils::fillRectangle(mRectPos2, mbgColor2, 0.f);
					}

					Vec2<float> mTextPos = { mRectPos.x + textPadding * 4.f + textPadding + textPadding, mRectPos.y + textPadding };
					ImGuiUtils::drawText(
						mTextPos,
						mod->getModuleName(),
						(mod->isEnabled() || mRectPos2.contains(mousePos)) ? whiteColor : semiGrayColor,
						textSize * 0.9,
						true
					);

					if (mRectPos2.contains(mousePos)) {
						tooltipString = mod->getDescription();
						ImGuiUtils::fillRectangle(mRectPos2, selectedColor, 0.f);

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
							static const UIColor mbgColor3 = UIColor(0, 0, 0, 50);
							ImGuiUtils::fillRectangle(sbgRectPos, mbgColor3);
							Vec4<float> leftSidelineRectPos = Vec4<float>(sbgRectPos.x, sbgRectPos.y, sbgRectPos.x + 7.f, sbgRectPos.w);
							ImGuiUtils::fillRectangle(leftSidelineRectPos, overallmain);
							if (sRectPos.contains(mousePos)) tooltipString = setting->description;

							switch (setting->settingType) {
							case SettingType::KEYBIND: {
								ImGuiUtils::drawText(sTextPos, setting->settingName + ":", whiteColor, textSize * 0.9, true);
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
								ImGuiUtils::drawText(Vec2<float>(sRectPos.z - ImGuiUtils::getTextWidth(textStr, textSize * 0.9) - textPadding * 4.f, sTextPos.y), textStr, whiteColor, textSize * 0.9, true);
								yOffset += textHeight + textPadding * 2.f;
								break;
							}
							case SettingType::CLICKME: {
								ImGuiUtils::drawText(sTextPos, setting->settingName + ":", whiteColor, textSize * 0.9, true);
								if (sRectPos.contains(mousePos)) {
									ImGuiUtils::fillRectangle(sRectPos, selectedColor, roundValue);
									configMgr = new ConfigManager();
									if (isLeftClickDown) {
										static Notifications* NotificationsMod = (Notifications*)client->moduleMgr->getModule("Notifications");
										configMgr->saveConfig();
										NotificationsMod->addNotifBox("Saved.", 5.0f);
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
								ImGuiUtils::drawText(sTextPos, setting->settingName, (setting->valuePtr->_bool || sRectPos.contains(mousePos)) ? whiteColor : semiGrayColor, textSize * 0.9, true);
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
								ImGuiUtils::drawText(sTextPos, setting->settingName + ":", whiteColor, textSize * 0.9, true);
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
								ImGuiUtils::drawText(Vec2<float>(sRectPos.z - ImGuiUtils::getTextWidth(enumName, textSize * 0.9) - textPadding * 4.f, sTextPos.y), enumName, whiteColor, textSize * 0.9, true);
								yOffset += textHeight + textPadding * 2.f;
								break;
							}
							case (SettingType::COLOR): {
								ImGuiUtils::drawText(sTextPos, setting->settingName + ":", whiteColor, textSize * 0.9, true);
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
									ImGuiUtils::drawText(sTextPos, setting->settingName + ":", whiteColor, textSize * 0.9, true);
									ImGuiUtils::drawText(Vec2<float>(sRectPos.z - ImGuiUtils::getTextWidth(std::to_string(setting->valuePtr->_int), textSize * 0.9) - textPadding * 4.f, sTextPos.y), std::to_string(setting->valuePtr->_int), whiteColor, textSize, true);

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
									ImGuiUtils::drawText(sTextPos, setting->settingName + ":", whiteColor, textSize * 0.9, true);
									char str[20];
									sprintf_s(str, 10, "%.2f", setting->valuePtr->_float);
									ImGuiUtils::drawText(Vec2<float>(sRectPos.z - ImGuiUtils::getTextWidth(std::string(str), textSize * 0.9) - textPadding * 4.f, sTextPos.y), (std::string(str)), whiteColor, textSize * 0.9, true);

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
			// (yOffset < window->pos.y + io.DisplaySize.y * 0.65f) ImGuiUtils::drawRectangle(Vec4<float>(cRectPos.x, cRectPos.y, cRectPos.z, yOffset), UIColor(36, 36, 36, 255), 2.f, roundValue, ImDrawFlags_RoundCornersTop);
			//else ImGuiUtils::drawRectangle(Vec4<float>(cRectPos.x, cRectPos.y, cRectPos.z, window->pos.y + io.DisplaySize.y * 0.65f), UIColor(36, 36, 36, 255), 2.f, roundValue, ImDrawFlags_RoundCornersTop);
		}
	}

	if (this->tooltips && tooltipString != std::string("NULL")) {
		Vec2<float> textPos(mousePos.x + 5.f, mousePos.y + 20.f);
		Vec4<float> rectPos(textPos.x - 5.f, textPos.y, textPos.x + ImGuiUtils::getTextWidth(tooltipString, 0.9f) + 5.f, textPos.y + ImGuiUtils::getTextHeight(0.9f));
		ImGuiUtils::fillRectangle(rectPos, UIColor(21, 21, 21, 200), roundValue);
		ImGuiUtils::drawRectangle(rectPos, UIColor(36, 36, 36, 255), (Mode == 0) ? 1.f : 2.f, roundValue);
		ImGuiUtils::drawText(textPos, tooltipString, whiteColor, 0.9f);
	}

	Vec4<float> searchRegion = Vec4<float>(io.DisplaySize.x / 2.f - 275.f,
		io.DisplaySize.y / 1.25f,
		io.DisplaySize.x / 2.f + 275.f,
		io.DisplaySize.y);
	static const float searchWidth = 400.f;
	static const float searchHeight = 40.f;
	static const std::string SearchStr = "Search";
	Vec4<float> searchRectPos = Vec4<float>(io.DisplaySize.x / 2.f - searchWidth / 2.f,
		io.DisplaySize.y - 50.f,
		io.DisplaySize.x / 2.f + searchWidth / 2.f,
		io.DisplaySize.y - 50.f + searchHeight);
	Vec4<float> typeRectPos = Vec4<float>(searchRectPos.x + 5.f,
		searchRectPos.y + 5.f,
		searchRectPos.z - ImGuiUtils::getTextWidth(SearchStr, textSize) - 10.f,
		searchRectPos.w - 5.f);
	if (typeRectPos.contains(mousePos) && isLeftClickDown) {
		if (isChoosingKeyBindPtr != nullptr) *isChoosingKeyBindPtr = false;
		isSearching = true;
		isLeftClickDown = false;
	}
	roundValue = 0.0f;
	Vec2<float> typeTextPos = Vec2<float>(typeRectPos.x + 5.f, typeRectPos.y + textPadding * 2.f);
	ImGuiUtils::fillRectangle(searchRectPos, UIColor(60, 60, 60, 255), roundValue);
	ImGuiUtils::fillRectangle(typeRectPos, UIColor(50, 50, 50, 255), roundValue);
	ImGuiUtils::drawText(Vec2<float>(typeRectPos.z + 5.f, typeTextPos.y), SearchStr, UIColor(255, 255, 255, 255), textSize);
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
		ImGuiUtils::drawText(typeTextPos, searchingModule, UIColor(255, 255, 255, 255), textSize);
		drawlist->PopClipRect();
	}
	static float barXAdd = 0.f;
	float mid = (typeRectPos.y + typeRectPos.w) / 2.f;
	Vec4<float> barRect = Vec4<float>(typeTextPos.x + barXAdd,
		mid - (textSize * 6.f),
		typeTextPos.x + barXAdd + 2.f,
		mid + (textSize * 6.f));
	UIColor barColor(255, 255, 255, 255);
	if (!isSearching && searchingModule.empty()) ImGuiUtils::drawText(typeTextPos, "Type here", UIColor(200, 200, 200, 255), textSize, false);
	lastKeyPress = -1;
}