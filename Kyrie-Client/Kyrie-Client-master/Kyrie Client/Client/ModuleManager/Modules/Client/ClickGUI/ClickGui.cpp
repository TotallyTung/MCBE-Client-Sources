#include "ClickGui.h"
#include "../../../../Client.h"
#include "../../../../../ImFX/imfx.h"

void getModuleListByCategory(const Category& c, std::vector<std::shared_ptr<Module>>& modList) {
	for (const std::shared_ptr<Module>& mod : client->moduleMgr->getModuleList()) {
		if (mod->getCategory() == c) {
			modList.push_back(mod);
		}
	}
};

ClickGui::ClickGui() : Module(VK_INSERT, Category::CLIENT, "ClickGui.") {
	registerBoolSetting("Gradient BG", &gradientBG, gradientBG);
	registerBoolSetting("Particles", &showParticles, showParticles);
	registerFloatSetting("Blur Strength", &blurStrength, blurStrength, 0.f, 10.f);
}

ClickGui::~ClickGui() {
	this->particles.clear();
	this->windowList.clear();
}

const char* ClickGui::getModuleName() {
	return "ClickGui";
}

void ClickGui::onEnable() {
	mc.getClientInstance()->releasebMouse();
}

void ClickGui::onDisable() {
	if (mc.getLocalPlayer() != nullptr) mc.getClientInstance()->grabMouse();
}

void ClickGui::init() {
	if (client->isInitialized() && client->moduleMgr->isInitialized()) {
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
		this->initialized = true;
	}
}

void ClickGui::onKeyUpdate(unsigned char key, bool isDown) {
	if (isDown) {
		lastKeyPressed = (int)key;
	}
	if (isDown && (key == VK_ESCAPE || key == this->getKeybind())) {
		if (isChoosingKeybind == nullptr) {
			this->setEnabled(false);
			return;
		}
		else if (!(*isChoosingKeybind)) {
			this->setEnabled(false);
			return;
		}
	}
}

void ClickGui::Render(ImDrawList* d) {
	if (!this->initialized) {
		init();
	}
	
	static float textHeight = RenderUtils::getTextHeight("A", 1.f);
	static float categoryPadding = 1.f;
	static float textPadding = 1.f;
	static float windowWidth = 200.f;
	static float opacity = 0.f;
	static ClickGui* clickguiModule = client->moduleMgr->getModule<ClickGui>();
	static Colors* ColorsModule = client->moduleMgr->getModule<Colors>();

	if (clickguiModule->isEnabled()) {
		mc.getClientInstance()->releasebMouse();
	}
	opacity = lerpSync(opacity, clickguiModule->isEnabled() ? 1.f : 0.f, 0.15f);

	Color mainColor(ColorsModule->getColor());
	Color SelectedColor(255, 255, 255, (int)(25 * opacity));
	Color ClickedColor(0, 0, 0, (int)(25 * opacity));
	Color CategoryColor(mainColor.r, mainColor.g, mainColor.b, (int)(150 * opacity)); // old 175
	Color ModBColor(0, 0, 0, (int)(50 * opacity));
	Color MdisableColor(0, 0, 0, (int)(75 * opacity));
	Color MenableColor(mainColor.r, mainColor.g, mainColor.b, (int)(125 * opacity)); // old 150

	Vec2<float> mousePos = *mc.getClientInstance()->getMousePos();
	if (opacity > 0.01f) {
		bool canClick = clickguiModule->isEnabled();
		{ // Background
			if (clickguiModule->blurStrength > 0.f) {
				ImFX::Begin(d);
				ImFX::AddBlur(blurStrength * opacity, ImVec4(0.f, 0.f, ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y));
				ImFX::End();
			}
			if (clickguiModule->gradientBG)
				d->AddRectFilledMultiColor(ImVec2(0.f, 0.f), ImGui::GetIO().DisplaySize, ImColor(0, 0, 0, (int)(75 * opacity)), ImColor(0, 0, 0, (int)(75 * opacity)), ImColor(mainColor.r, mainColor.g, mainColor.b, (int)(75 * opacity)), ImColor(mainColor.r, mainColor.g, mainColor.b, (int)(75 * opacity)));
			else RenderUtils::drawRectFilled(d, Vec2<float>(0.f, 0.f), Vec2<float>(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), Color(0, 0, 0, (int)(75 * opacity)));
		}
		{ // Particles
			float start = RandomFloat(0.f, ImGui::GetIO().DisplaySize.x);
			if (clickguiModule->showParticles) {
				if (particles.size() < 125) {
					int count = 1 * (int)(ceil(60.f / ImGui::GetIO().Framerate));
					for (int i = 0; i < count; i++)
						particles.emplace_back(new clickGuiParticles(Vec2<float>(start, ImGui::GetIO().DisplaySize.y + 10.f), Vec2<float>(start, ImGui::GetIO().DisplaySize.y / RandomFloat(1.5f, 2.5f)), RandomFloat(3.f, 10.f), RandomFloat(1.f, 7.f)));
				}
			}
			for (int i = 0; i < particles.size(); i++) {
				std::shared_ptr<clickGuiParticles> p = particles[i];
				d->AddCircleFilled(ImVec2(p->startPos.x, p->startPos.y), p->size, ImColor(mainColor.r, mainColor.g, mainColor.b, (int)(150 * opacity * p->duration))); // old 175
				d->AddShadowCircle(ImVec2(p->startPos.x, p->startPos.y), p->size, ImColor(mainColor.r, mainColor.g, mainColor.b, (int)(255 * opacity * p->duration)), 50.f, ImVec2(0.f, 0.f));
				p->startPos.y -= p->speed * (60.f / ImGui::GetIO().Framerate);
				if (p->startPos.y <= p->endPos.y || !clickguiModule->showParticles) {
					p->duration = lerpSync(p->duration, 0.f, 0.1f);
				}
				if (p->duration <= 0.1f) {
					particles.erase(particles.begin() + i);
					i--;
				}
			}
		}

		for (std::shared_ptr<ClickWindow>& window : windowList) {
			{	
				window->posReal.y += ImGui::GetIO().MouseWheel * 50.f;
				window->pos.y = lerpSync(window->pos.y, window->posReal.y, 0.1f);
				// Logic
				if (window->isDragging) {
					window->pos.x += mousePos.x - startDragPos.x;
					window->pos.y += mousePos.y - startDragPos.y;
					window->posReal = window->pos;
					startDragPos = mousePos;
					if (!mc.isHoldingLeftClick) window->isDragging = false;
				}
				// Shadow for window
				{
					float yEnd = window->pos.y + textHeight + categoryPadding;
					for (const std::shared_ptr<Module>& mod : window->modList) {
						yEnd += (textHeight + textPadding * 2.f) * window->extendedDuration;
						for (const SettingEntry* setting : mod->getSettings()) {
							if (*setting->shouldShow) yEnd += (textHeight + textPadding * 1.f) * mod->extendedDuration * window->extendedDuration;
						}
					}

					if (window->extended) yEnd += textPadding * 2.f;
					
					//d->AddShadowRect(ImVec2(window->pos.x - categoryPadding, window->pos.y - categoryPadding), ImVec2(window->pos.x + windowWidth + categoryPadding, yEnd), ImColor(0, 0, 0, (int)(50 * opacity)), 75.f, ImVec2(0.f, 0.f));
					//d->AddShadowRect(ImVec2(window->pos.x - categoryPadding, window->pos.y - categoryPadding), ImVec2(window->pos.x + windowWidth + categoryPadding, yEnd - 2.f), ImColor(0, 0, 0, (int)(175 * opacity)), 75.f, ImVec2(0.f, 0.f), ImDrawFlags_ShadowCutOutShapeBackground);
					//d->AddShadowRect(ImVec2(window->pos.x - categoryPadding, window->pos.y - categoryPadding), ImVec2(window->pos.x + windowWidth + categoryPadding, yEnd - 2.f), ImColor(0, 0, 0, (int)(125 * opacity)), 40.f, ImVec2(0.f, 0.f));

					ImFX::Begin(d);
					ImFX::AddBlur(2.5f * opacity, ImVec4(window->pos.x - categoryPadding, window->pos.y - categoryPadding, window->pos.x + windowWidth + categoryPadding, yEnd - 2.f));
					ImFX::End();
				}
				//  Category Render
				Vec4<float> categoryRectPos = Vec4<float>(window->pos.x - categoryPadding, window->pos.y - categoryPadding, window->pos.x + windowWidth + categoryPadding, window->pos.y + textHeight + categoryPadding);
				bool isFocus = categoryRectPos.contains(mousePos);
				RenderUtils::drawRectFilled(d, categoryRectPos, CategoryColor, 0.f);
				if (isFocus) {
					RenderUtils::drawRectFilled(d, categoryRectPos, SelectedColor, 0.f);

					if (mc.isLeftClickDown) {
						if (!window->isDragging) {
							window->isDragging = true;
							startDragPos = mousePos;
						}
						mc.isLeftClickDown = false;
					}
					else if (mc.isRightClickDown) {
						window->extended = !window->extended;
						mc.isRightClickDown = false;
					}

				}
				RenderUtils::drawText(d, Vec2<float>(window->pos.x + categoryPadding + textPadding, window->pos.y), window->windowName, Color(255, 255, 255, (int)(255 * opacity)), 1.f, true);

				window->extendedDuration = lerpSync(window->extendedDuration, window->extended ? 1.f : 0.f, 0.2f);

				{ // Render Modules
					float xOffset = window->pos.x - categoryPadding;
					float xEnd = window->pos.x + windowWidth + categoryPadding;
					float yOffset = window->pos.y + textHeight + categoryPadding;
					float yEnd = window->pos.y + textHeight + categoryPadding;
					for (const std::shared_ptr<Module>& mod : window->modList) {
						yEnd += (textHeight + textPadding * 2.f) * window->extendedDuration;
						for (const SettingEntry* setting : mod->getSettings()) {
							if (*setting->shouldShow) yEnd += (textHeight + textPadding * 1.f) * mod->extendedDuration * window->extendedDuration;
						}
					}
					if (window->extended) yEnd += textPadding * 2.f;

					d->PushClipRect(ImVec2(xOffset, yOffset), ImVec2(xEnd, yEnd));
					RenderUtils::drawRectFilled(d, Vec4<float>(xOffset, yOffset, xEnd, yEnd), ModBColor);
					
					for (std::shared_ptr<Module>& mod : window->modList) {
						yOffset += textPadding * 2.f;
						Vec4<float> modRect = Vec4<float>(xOffset + textPadding * 2.f, yOffset, xEnd - textPadding * 2.f, yOffset + textHeight);

						mod->enabledDuration = lerpSync(mod->enabledDuration, mod->isEnabled() ? 1.f : 0.f, 0.2f);

						RenderUtils::drawRectFilled(d, modRect, ColorUtils::lerp(MdisableColor, MenableColor, mod->enabledDuration));

						bool isFocus = modRect.contains(mousePos) && window->extended;
						if (isFocus) {
							mod->selectedDuration = lerpSync(mod->selectedDuration, 1.f, 0.2f);
							if (canClick) {
								if (mc.isLeftClickDown) {
									mod->toggle();
									mc.isLeftClickDown = false;
								}
								else if (mc.isRightClickDown) {
									mod->extended = !mod->extended;
									mc.isRightClickDown = false;
								}
							}
						}
						else {
							mod->selectedDuration = lerpSync(mod->selectedDuration, 0.f, 0.2f);
						}
						RenderUtils::drawRectFilled(d, modRect, Color(SelectedColor.r, SelectedColor.g, SelectedColor.b, (int)(SelectedColor.a * mod->selectedDuration)));
						RenderUtils::drawText(d, Vec2<float>(modRect.x + textPadding * 2.f + 5.f * mod->selectedDuration, modRect.y), mod->getModuleName(), ColorUtils::lerp(Color(175, 175, 175, (int)(255 * opacity)), Color(255, 255, 255, (int)(255 * opacity)), mod->enabledDuration), 1.f, true);
						float mid = (modRect.y + modRect.w) / 2.f;
						/*
						d->AddCircleFilled(ImVec2(modRect.z - 6.f, mid - 6.f * (1.f - mod->extendedDuration)), 1.5f, ImColor(255, 255, 255, (int)(255 * opacity)));
						d->AddCircleFilled(ImVec2(modRect.z - 6.f, mid), 1.5f, ImColor(255, 255, 255, (int)(255 * opacity)));
						d->AddCircleFilled(ImVec2(modRect.z - 6.f, mid + 6.f * (1.f - mod->extendedDuration)), 1.5f, ImColor(255, 255, 255, (int)(255 * opacity)));
						*/
						/*
						d->AddCircleFilled(ImVec2(modRect.z - 6.f, mid + 6.f - 12.f * (1.f - mod->extendedDuration)), 1.5f, ImColor(255, 255, 255, (int)(255 * opacity)));
						d->AddCircleFilled(ImVec2(modRect.z - 6.f, mid + 6.f - 6.f * (1.f - mod->extendedDuration)), 1.5f, ImColor(255, 255, 255, (int)(255 * opacity)));
						d->AddCircleFilled(ImVec2(modRect.z - 6.f, mid + 6.f), 1.5f, ImColor(255, 255, 255, (int)(255 * opacity)));
						*/
						mod->extendedDuration = lerpSync(mod->extendedDuration, mod->extended ? 1.f : 0.f, 0.2f);
						
						yOffset += textHeight * mod->extendedDuration;
						//yOffset += textPadding * 2.f * mod->extendedDuration;

						//RenderUtils::drawRect(d, Vec4<float>(modRect.x, yOffset, modRect.z, yOffset + (textPadding * 2.f + textHeight) * mod->getSettings().size() * mod->extendedDuration), MdisableColor);
						{
							float test = 0.f;
							for (const SettingEntry* setting : mod->getSettings()) {
								if (*setting->shouldShow) test += (textHeight + textPadding * 1.f) * mod->extendedDuration * window->extendedDuration;
							}
							if (modRect.w < yOffset + test) RenderUtils::drawRectFilled(d, Vec4<float>(modRect.x, modRect.w, modRect.z, yOffset + test), MdisableColor);
						}
						float yOffsetOld = yOffset;
						for (int i = 0; i < mod->getSettings().size(); i++) {
							SettingEntry* setting = mod->getSettings()[i];
							if (!(*setting->shouldShow)) {
								switch (setting->valueType) {
								case ValueType::BOOL_T: {
									setting->enabledDuration = 0.f;
									break;
								}
								case ValueType::INT_T: {
									setting->PosZ = modRect.x + textPadding * 2.f;
									break;
								}
								case ValueType::FLOAT_T: {
									setting->PosZ = modRect.x + textPadding * 2.f;
									break;
								}
								}
								if (i + 1 < mod->getSettings().size()) {
									if (*mod->getSettings()[i + 1]->shouldShow) yOffset += textHeight * mod->extendedDuration;
								}
								continue;
							}
							float Sopacity = opacity * mod->extendedDuration;
							Color SdisableColor(0, 0, 0, (int)(0 * Sopacity));
							Color SenableColor((int)(mainColor.r / 1.f), (int)(mainColor.g / 1.f), (int)(mainColor.b / 1.f), (int)(125 * Sopacity)); // old 150 and  / 1.15f
							yOffset += textPadding * 1.f * mod->extendedDuration;

							Vec4<float> settingPos = Vec4<float>(modRect.x + textPadding * 3.f, yOffset, modRect.z - textPadding * 3.f, yOffset + textHeight);
							const bool isFocus = settingPos.contains(mousePos) && mod->extendedDuration >= 0.8f && window->extended;
							switch (setting->valueType) {
							case ValueType::KEYBINT_T:
							{
								if (!mod->extended) setting->isChoosingKeybind = false;
								if (isFocus) {
									setting->selectedDuration = lerpSync(setting->selectedDuration, 1.f, 0.2f);
									if (canClick) {
										if (mc.isLeftClickDown) {
											if (isChoosingKeybind != nullptr && isChoosingKeybind != &(setting->isChoosingKeybind)) *isChoosingKeybind = false;
											setting->isChoosingKeybind = !setting->isChoosingKeybind;
											isChoosingKeybind = &(setting->isChoosingKeybind);
											setting->timeCounter = 0.f;
											mc.isLeftClickDown = false;
										}
										else if (setting->isChoosingKeybind && mc.isRightClickDown) {
											setting->value->_int = 0x0;
											setting->isChoosingKeybind = false;
											mc.isRightClickDown = false;
										}
									}
								}
								else {
									setting->selectedDuration = lerpSync(setting->selectedDuration, 0.f, 0.2f);
								}
								RenderUtils::drawRectFilled(d, settingPos, Color(SelectedColor.r, SelectedColor.g, SelectedColor.b, (int)(SelectedColor.a * setting->selectedDuration * mod->extendedDuration)));

								RenderUtils::drawText(d, Vec2<float>(settingPos.x + textPadding * 2.f, settingPos.y), std::string(std::string(setting->name) + ":").c_str(), Color(255, 255, 255, (int)(255 * Sopacity)), 1.f, true);
								if (setting->isChoosingKeybind) {
									//RenderUtils::drawText(d, Vec2<float>(settingPos.z - RenderUtils::getTextWidth("...", 1.f) - textPadding * 2.f, settingPos.y), "...", Color(255, 255, 255, (int)(255 * Sopacity)), 1.f, true);
									float test1, test2, test3 = 0.f;
									setting->timeCounter += ImGui::GetIO().DeltaTime;
									if (setting->timeCounter > 1.f && setting->timeCounter < 1.5f) test1 = 1.f - abs(setting->timeCounter - 1.25f) * 4.f;
									if (setting->timeCounter > 1.25f && setting->timeCounter < 1.75f) test2 = 1.f - abs(setting->timeCounter - 1.5f) * 4.f;
									if (setting->timeCounter > 1.5f && setting->timeCounter < 2.f) test3 = 1.f - abs(setting->timeCounter - 1.75f) * 4.f;
									d->AddCircleFilled(ImVec2(settingPos.z - 4.f - 18.f, settingPos.w - 6.f - 6.f * test1), 2.f, ImColor(255, 255, 255, 255));
									d->AddCircleFilled(ImVec2(settingPos.z - 4.f - 10.f, settingPos.w - 6.f - 6.f * test2), 2.f, ImColor(255, 255, 255, 255));
									d->AddCircleFilled(ImVec2(settingPos.z - 4.f - 2.f, settingPos.w - 6.f - 6.f * test3), 2.f, ImColor(255, 255, 255, 255));
									if (setting->timeCounter >= 2.5f) setting->timeCounter = 0.f;
									if (lastKeyPressed != 0xFFFFFF) {
										setting->value->_int = lastKeyPressed;
										setting->isChoosingKeybind = false;
									}
								}
								else {
									if (mod->getKeybind() == 0x0) RenderUtils::drawText(d, Vec2<float>(settingPos.z - RenderUtils::getTextWidth("None", 1.f) - textPadding * 2.f, settingPos.y), "None", Color(255, 255, 255, (int)(255 * Sopacity)), 1.f, true);
									else if (KeyNames[setting->value->_int] != nullptr) RenderUtils::drawText(d, Vec2<float>(settingPos.z - RenderUtils::getTextWidth(KeyNames[setting->value->_int], 1.f) - textPadding * 2.f, settingPos.y), KeyNames[setting->value->_int], Color(255, 255, 255, (int)(255 * Sopacity)), 1.f, true);
									else RenderUtils::drawText(d, Vec2<float>(settingPos.z - RenderUtils::getTextWidth("Unknow", 1.f) - textPadding * 2.f, settingPos.y), "Unknow", Color(255, 255, 255, (int)(255 * Sopacity)), 1.f, true);
								}

								break;

							}
							case ValueType::BOOL_T: {
								RenderUtils::drawRectFilled(d, settingPos, ColorUtils::lerp(SdisableColor, SenableColor, setting->enabledDuration));
								//RenderUtils::drawText(d, Vec2<float>(settingPos.x + textPadding * 2.f + 5.f * setting->selectedDuration, settingPos.y), setting->name, ColorUtils::lerp(Color(175, 175, 175, (int)(255 * Sopacity)), Color(255, 255, 255, (int)(255 * Sopacity)), setting->enabledDuration), 1.f, true);
								RenderUtils::drawText(d, Vec2<float>(settingPos.x + textPadding * 2.f, settingPos.y), setting->name, ColorUtils::lerp(Color(175, 175, 175, (int)(255 * Sopacity)), Color(255, 255, 255, (int)(255 * Sopacity)), setting->enabledDuration), 1.f, true);
								setting->enabledDuration = lerpSync(setting->enabledDuration, setting->value->_bool ? 1.f : 0.f, 0.2f);
								RenderUtils::drawRectFilled(d, settingPos, Color(SelectedColor.r, SelectedColor.g, SelectedColor.b, (int)(SelectedColor.a * setting->selectedDuration * mod->extendedDuration)));

								if (isFocus) {
									setting->selectedDuration = lerpSync(setting->selectedDuration, 1.f, 0.2f);
									if (canClick) {
										if (mc.isLeftClickDown) {
											setting->value->_bool = !setting->value->_bool;
											mc.isLeftClickDown = false;
										}
									}
								}
								else {
									setting->selectedDuration = lerpSync(setting->selectedDuration, 0.f, 0.2f);
								}
								break;
							}
							case ValueType::INT_T: {

								const float minValue = (float)setting->minValue->_int;
								const float maxValue = (float)setting->maxValue->_int - minValue;
								float value = (float)fmax(0, setting->value->_int - minValue);  // Value is now always > 0 && < maxValue
								if (value > maxValue)
									value = maxValue;
								value /= maxValue;  // Value is now in range 0 - 1
								const float endXlol = settingPos.z - settingPos.x;
								value *= endXlol;
								Vec4<float> sliderRect = settingPos;
								// Draw Progress
								{
									if (setting->PosZ < sliderRect.x) setting->PosZ = sliderRect.x;
									if (setting->PosZ > sliderRect.z) setting->PosZ = sliderRect.z;
									sliderRect.z = sliderRect.x + value;
									setting->PosZ += (sliderRect.z - setting->PosZ) * 0.2f;
									//RenderUtils::fillRectangle(setting->pos, Ccolor, EnablebackgroundOpacity);
									RenderUtils::drawRectFilled(d, Vec4<float>(sliderRect.x, sliderRect.y, setting->PosZ, sliderRect.w), SenableColor);
								}
								if (isFocus) {
									setting->selectedDuration = lerpSync(setting->selectedDuration, 1.f, 0.2f);
									if (canClick) {
										if (mc.isLeftClickDown) {
											setting->isDragging = true;
											mc.isLeftClickDown = false;
										}
									}
								}
								else {
									if (!setting->isDragging) setting->selectedDuration = lerpSync(setting->selectedDuration, 0.f, 0.2f);
								}
								RenderUtils::drawRectFilled(d, Vec4<float>(sliderRect.x, sliderRect.y, setting->PosZ, sliderRect.w), Color(SelectedColor.r, SelectedColor.g, SelectedColor.b, (int)(SelectedColor.a * setting->selectedDuration * mod->extendedDuration)));
								if (setting->isDragging) {
									if (!mc.isHoldingLeftClick) setting->isDragging = false;
									else {
										value = mousePos.x - sliderRect.x;
									}
								}
								// Save Value
								{
									value /= endXlol;  // Now in range 0 - 1
									value *= maxValue;
									value += minValue;

									setting->value->_int = (int)roundf(value);
									setting->makeSureTheValueIsAGoodBoiAndTheUserHasntScrewedWithIt();
								}
								RenderUtils::drawText(d, Vec2<float>(settingPos.x + textPadding * 2.f, settingPos.y), std::string(std::string(setting->name) + ":").c_str(), Color(255, 255, 255, (int)(255 * Sopacity)), 1.f, true);
								RenderUtils::drawText(d, Vec2<float>(settingPos.z - RenderUtils::getTextWidth(std::to_string(setting->value->_int).c_str(), 1.f) - textPadding * 2.f, settingPos.y), std::to_string(setting->value->_int).c_str(), Color(255, 255, 255, (int)(255 * Sopacity)), 1.f, true);
								break;
							}
							case ValueType::FLOAT_T: {
								const float minValue = setting->minValue->_float;
								const float maxValue = setting->maxValue->_float - minValue;
								float value = (float)fmax(0, setting->value->_float - minValue);  // Value is now always > 0 && < maxValue
								if (value > maxValue)
									value = maxValue;
								value /= maxValue;  // Value is now in range 0 - 1
								const float endXlol = settingPos.z - settingPos.x;
								value *= endXlol;
								Vec4<float> sliderRect = settingPos;
								// Draw Progress
								{
									if (setting->PosZ < sliderRect.x) setting->PosZ = sliderRect.x;
									if (setting->PosZ > sliderRect.z) setting->PosZ = sliderRect.z;
									sliderRect.z = sliderRect.x + value;
									setting->PosZ += (sliderRect.z - setting->PosZ) * 0.2f;
									//RenderUtils::fillRectangle(setting->pos, Ccolor, EnablebackgroundOpacity);
									RenderUtils::drawRectFilled(d, Vec4<float>(sliderRect.x, sliderRect.y, setting->PosZ, sliderRect.w), SenableColor);
								}
								if (isFocus) {
									setting->selectedDuration = lerpSync(setting->selectedDuration, 1.f, 0.2f);
									if (canClick) {
										if (mc.isLeftClickDown) {
											setting->isDragging = true;
											mc.isLeftClickDown = false;
										}
									}
								}
								else {
									if (!setting->isDragging) setting->selectedDuration = lerpSync(setting->selectedDuration, 0.f, 0.2f);
								}
								RenderUtils::drawRectFilled(d, Vec4<float>(sliderRect.x, sliderRect.y, setting->PosZ, sliderRect.w), Color(SelectedColor.r, SelectedColor.g, SelectedColor.b, (int)(SelectedColor.a * setting->selectedDuration * mod->extendedDuration)));
								if (setting->isDragging) {
									if (!mc.isHoldingLeftClick) setting->isDragging = false;
									else {
										value = mousePos.x - sliderRect.x;
									}
								}
								// Save Value
								{
									value /= endXlol;  // Now in range 0 - 1
									value *= maxValue;
									value += minValue;

									setting->value->_float = value;
									setting->makeSureTheValueIsAGoodBoiAndTheUserHasntScrewedWithIt();
								}
								RenderUtils::drawText(d, Vec2<float>(settingPos.x + textPadding * 2.f, settingPos.y), std::string(std::string(setting->name) + ":").c_str(), Color(255, 255, 255, (int)(255 * Sopacity)), 1.f, true);
								char str[10];
								sprintf_s(str, 10, "%.2f", setting->value->_float);
								RenderUtils::drawText(d, Vec2<float>(settingPos.z - RenderUtils::getTextWidth(str, 1.f) - textPadding * 2.f, settingPos.y), str, Color(255, 255, 255, (int)(255 * Sopacity)), 1.f, true);
								break;
							}
							case ValueType::ENUM_T: {
								RenderUtils::drawText(d, Vec2<float>(settingPos.x + textPadding * 2.f, settingPos.y), std::string(std::string(setting->name) + ":").c_str(), Color(255, 255, 255, (int)(255 * Sopacity)), 1.f, true);
								EnumEntry& i = ((SettingEnum*)setting->extraData)->GetSelectedEntry();
								RenderUtils::drawText(d, Vec2<float>(settingPos.z - RenderUtils::getTextWidth(i.GetName(), 1.f) - textPadding * 2.f, settingPos.y), i.GetName(), Color(255, 255, 255, (int)(255 * Sopacity)), 1.f, true);
								if (isFocus) {
									setting->selectedDuration = lerpSync(setting->selectedDuration, 1.f, 0.2f);
									if (canClick) {
										if (mc.isLeftClickDown) {
											((SettingEnum*)setting->extraData)->SelectNextValue(false);
											mc.isLeftClickDown = false;
										}
										else if (mc.isRightClickDown) {
											((SettingEnum*)setting->extraData)->SelectNextValue(true);
											mc.isRightClickDown = false;
										}
									}
								}
								else {
									setting->selectedDuration = lerpSync(setting->selectedDuration, 0.f, 0.2f);
								}
								RenderUtils::drawRectFilled(d, settingPos, Color(SelectedColor.r, SelectedColor.g, SelectedColor.b, (int)(SelectedColor.a* setting->selectedDuration* mod->extendedDuration)));
								break;
							}
							}
							//if (i != mod->getSettings().size() - 1) yOffset += textHeight * mod->extendedDuration;
							if (i + 1 < mod->getSettings().size()) {
								if (*mod->getSettings()[i + 1]->shouldShow) yOffset += textHeight * mod->extendedDuration;
							}
						}

						yOffset += textHeight;
					}
					d->PopClipRect();
				}
				
			}
		}
	}
	mc.isLeftClickDown = false;
	mc.isRightClickDown = false;
	lastKeyPressed = 0xFFFFFF;
}