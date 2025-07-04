#include "ClickGui.h"

#include <Windows.h>
#include "../../Utils/Json.hpp"
#include "../../Utils/Logger.h"
using namespace std;
bool isLeftClickDown = false;
bool isRightClickDown = false;
bool shouldToggleLeftClick = false;  // If true, toggle the focused module
bool shouldToggleRightClick = false;
bool resetStartPos = true;
bool initialised = false;
int scrollingDirection = 0;

struct SavedWindowSettings {
	vec2_t pos = { -1, -1 };
	bool isExtended = true;
	const char* name = "";
};

map<unsigned int, shared_ptr<ClickWindow>> windowMap;
map<unsigned int, SavedWindowSettings> savedWindowSettings;

bool isDragging = false;
unsigned int draggedWindow = -1;
vec2_t dragStart = vec2_t();

unsigned int focusedElement = -1;
bool isFocused = false;
float opacity = 0.6f;
//static constexpr float textPadding = 2.0f;
static constexpr float textPadding = 1.0f;
static constexpr float textSize = 1.0f;
static constexpr float textHeight = textSize * 9.5f;
static constexpr float categoryMargin = 0.5f;
static constexpr float paddingRight = 13.5f;
static constexpr float crossSize = textHeight / 2.f;
static constexpr float crossWidth = 0.2f;
SettingEntry* keybindMenuCurrent = nullptr;  // What setting is currently capturing the user's input?
int newKeybind = 0;
bool isCapturingKey = false;
bool shouldStopCapturing = false;
bool isConfirmingKey = false;
//static float rcolors[4];  // Rainbow color array RGBA

float currentYOffset = 0;
float currentXOffset = 0;

int timesRendered = 0;

void ClickGui::getModuleListByCategory(Category category, vector<shared_ptr<IModule>>* modList) {
	auto lock = moduleMgr->lockModuleList();
	vector<shared_ptr<IModule>>* moduleList = moduleMgr->getModuleList();

	for (auto& it : *moduleList) {
		if (it->getCategory() == category) modList->push_back(it);
	}
}

shared_ptr<ClickWindow> ClickGui::getWindow(const char* name) {
	unsigned int id = Utils::getCrcHash(name);

	auto search = windowMap.find(id);
	if (search != windowMap.end()) {  // Window exists already
		return search->second;
	}
	else {  // Create window
		// TODO: restore settings for position etc
		shared_ptr<ClickWindow> newWindow = make_shared<ClickWindow>();
		newWindow->name = name;

		auto savedSearch = savedWindowSettings.find(id);
		if (savedSearch != savedWindowSettings.end()) {  // Use values from config
			newWindow->isExtended = savedSearch->second.isExtended;
			if (savedSearch->second.pos.x > 0) newWindow->pos = savedSearch->second.pos;
		}

		windowMap.insert(make_pair(id, newWindow));
		return newWindow;
	}
}

shared_ptr<ClickModule> ClickGui::getClickModule(shared_ptr<ClickWindow> window, const char* name) {
	unsigned int id = Utils::getCrcHash(name);

	auto search = window->moduleMap.find(id);
	if (search != window->moduleMap.end()) {  // Window exists already
		return search->second;
	}
	else {  // Create window
		// TODO: restore settings for position etc
		shared_ptr<ClickModule> newModule = make_shared<ClickModule>();

		window->moduleMap.insert(make_pair(id, newModule));
		return newModule;
	}
}

void ClickGui::renderLabel(const char* text) { }

void ClickGui::renderTooltip(string* text) {
	auto interfaceColor = ColorUtil::interfaceColor(1);
	vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	float lFPS = DrawUtils::getTextWidth(text, 1) + 6.5;
	float posY = windowSize.y - 24;
	vec4_t rectPos = vec4_t(2 + 2, posY, lFPS + 3, posY + 17);
	vec2_t textPos = vec2_t(rectPos.x + textPadding + 2, rectPos.y + 4);
	DrawUtils::fillRectangleA(rectPos, MC_Color(0, 0, 0, 130));
	DrawUtils::drawRectangle(rectPos, MC_Color(interfaceColor), 1.f, 0.7f);
	DrawUtils::drawText(textPos, text, MC_Color(255, 255, 255), 1.f, 1.f, true);
}

#pragma region Packet
void ClickGui::renderPacketCategory(Category category, MC_Color categoryColor) {
	//static constexpr float textHeight = textSize * 10.f;
	static constexpr float textHeight = textSize * 10.f;
	const char* categoryName = ClickGui::catToName(category);
	static auto clickGUI = moduleMgr->getModule<ClickGUIMod>();

	const std::shared_ptr<ClickWindow> ourWindow = getWindow(categoryName);

	// Reset Windows to pre-set positions to avoid confusion
	if (resetStartPos && ourWindow->pos.x <= 0) {
		float yot = g_Data.getGuiData()->windowSize.x;
		ourWindow->pos.y = 4;
		switch (category) {
		case Category::COMBAT:
			ourWindow->pos.x = yot / 7.5f;
			break;
		case Category::VISUAL:
			ourWindow->pos.x = yot / 4.1f;
			break;
		case Category::MOVEMENT:
			ourWindow->pos.x = yot / 5.6f * 2.f;
			break;
		case Category::PLAYER:
			ourWindow->pos.x = yot / 4.25f * 2.f;
			break;
		case Category::EXPLOIT:
			ourWindow->pos.x = yot / 3.4f * 2;
			break;
		case Category::OTHER:
			ourWindow->pos.x = yot / 2.9f * 2.05f;
			break;
		case Category::UNUSED:
			ourWindow->pos.x = yot / 1.6f * 2.2f;
			break;
		case Category::CUSTOM:
			ourWindow->pos.x = yot / 5.6f * 2.f;
			break;

		}
	}
	if (clickGUI->openAnim < 27) ourWindow->pos.y = clickGUI->openAnim;

	const float xOffset = ourWindow->pos.x;
	const float yOffset = ourWindow->pos.y;
	vec2_t* windowSize = &ourWindow->size;
	currentXOffset = xOffset;
	currentYOffset = yOffset;

	// Get All Modules in our category
	std::vector<std::shared_ptr<IModule>> ModuleList;
	getModuleListByCategory(category, &ModuleList);

	// Get max width of all text
	{
		for (auto& it : ModuleList) {
			auto intf = moduleMgr->getModule<Interface>();
			//std::string label = "-------------";
			//std::string label = "--------------";
			std::string label = "--------------";
			windowSize->x = fmax(windowSize->x, DrawUtils::getTextWidth(&string(label), textSize, Fonts::SMOOTH));
		}
	}

	vector<shared_ptr<IModule>> moduleList;
	/*{
		vector<int> toIgniore;
		int moduleCount = (int)ModuleList.size();
		for (int i = 0; i < moduleCount; i++) {
			float bestWidth = 1.f;
			int bestIndex = 1;

			for (int j = 0; j < ModuleList.size(); j++) {
				bool stop = false;
				for (int bruhwth = 0; bruhwth < toIgniore.size(); bruhwth++)
					if (j == toIgniore[bruhwth]) {
						stop = true;
						break;
					}
				if (stop)
					continue;

				string t = ModuleList[j]->getRawModuleName();
				float textWidthRn = DrawUtils::getTextWidth(&t, textSize, Fonts::SMOOTH);
				if (textWidthRn > bestWidth) {
					bestWidth = textWidthRn;
					bestIndex = j;
				}
			}
			moduleList.push_back(ModuleList[bestIndex]);
			toIgniore.push_back(bestIndex);
		}
	}
	std::sort(moduleList.begin(), moduleList.end(), [](auto lhs, auto rhs) {
		auto current = lhs;
		auto other = rhs;
		return std::string{ *current->getRawModuleName() } < std::string{ *other->getRawModuleName() };
	});
	*/
	getModuleListByCategory(category, &moduleList);
	const float xEnd = currentXOffset + windowSize->x + paddingRight;

	vec2_t mousePos = *g_Data.getClientInstance()->getMousePos();
	{
		vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
		vec2_t windowSizeReal = g_Data.getClientInstance()->getGuiData()->windowSizeReal;
		mousePos = mousePos.div(windowSizeReal);
		mousePos = mousePos.mul(windowSize);
	}

	float categoryHeaderYOffset = currentYOffset;

	if (ourWindow->isInAnimation) {
		if (ourWindow->isExtended) {
			clickGUI->animation *= 0.85f;
			if (clickGUI->animation < 0.001f) {
				ourWindow->yOffset = 0;
				ourWindow->isInAnimation = false;
			}

		}
		else {
			clickGUI->animation = 1 - ((1 - clickGUI->animation) * 0.85f);
			if (1 - clickGUI->animation < 0.001f) ourWindow->isInAnimation = false;
		}
	}

	currentYOffset += textHeight + (textPadding * 2) + 2.f;
	if (ourWindow->isExtended) {
		if (ourWindow->isInAnimation) {
			currentYOffset -= clickGUI->animation * moduleList.size() * (textHeight + (textPadding * 2));
		}

		bool overflowing = false;
		const float cutoffHeight = roundf(g_Data.getGuiData()->heightGame * 0.75f) + 0.5f /*fix flickering related to rounding errors*/;
		int moduleIndex = 0;
		for (auto& mod : moduleList) {
			moduleIndex++;
			if (moduleIndex < ourWindow->yOffset) continue;
			float probableYOffset = (moduleIndex - ourWindow->yOffset) * (textHeight + (textPadding * 2));

			if (ourWindow->isInAnimation) { // Estimate, we don't know about module settings yet
				if (probableYOffset > cutoffHeight) {
					overflowing = true;
					break;
				}
			}
			else if ((currentYOffset - ourWindow->pos.y) > cutoffHeight || currentYOffset > g_Data.getGuiData()->heightGame - 5) {
				overflowing = true;
				break;
			}

			std::string ncdb = "LOL";
			std::string textStr = mod->getRawModuleName();
			if (mod->getKeybind() != 0x0) {
				char textgf[50];
				//printf_s(textgf, 50, "%s", std::string(std::string(GRAY) + " [" + std::string(WHITE) + std::string(Utils::getKeybindName(mod->getKeybind())) + std::string(GRAY) + "]").c_str());
				sprintf_s(textgf, 50, "%s", ("[" + std::string(Utils::getKeybindName(mod->getKeybind())) + "]").c_str());
				//moduleName = text;
				ncdb = textgf;
			}

			vec2_t textPos = vec2_t(currentXOffset + textPadding + 45, currentYOffset + textPadding + 5);
			vec4_t rectPos = vec4_t(
				currentXOffset, currentYOffset - 1.05f, xEnd,
				currentYOffset + textHeight + (textPadding * 2) - 1.3);
			bool allowRender = currentYOffset >= categoryHeaderYOffset;

			// Background
			if (allowRender) {
				auto interfaceColor = ColorUtil::interfaceColor(1);
				if (!ourWindow->isInAnimation && !isDragging && rectPos.contains(&mousePos)) {
					if (mod->isEnabled()) DrawUtils::fillRectangle(rectPos, ColorUtil::interfaceColor(1), opacity);
					else DrawUtils::fillRectangle(rectPos, MC_Color(0, 0, 0), opacity);
					if (shouldToggleLeftClick && !ourWindow->isInAnimation) {
						mod->toggle();
						shouldToggleLeftClick = false;
					}
				}
				else {
					//DrawUtils::fillRectangle(vec4_t(rectPos.x, rectPos.y + 1, rectPos.z, rectPos.w+0.1), MC_Color(171, 171, 171) ,0);
					if (mod->isEnabled()) DrawUtils::fillRectangle(rectPos, ColorUtil::interfaceColor(1), opacity);
					else DrawUtils::fillRectangle(rectPos, MC_Color(0, 0, 0), opacity);
				}
			}

			// Text

			float ncdb4 = DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() * 1.f;
			vec2_t textPosFix = vec2_t(currentXOffset + textPadding + 2.f, (rectPos.y + rectPos.w) / 2 - ncdb4 / 2 + 1.5f);
			if (allowRender) {
				DrawUtils::drawText(textPosFix, &textStr, mod->isEnabled() ? MC_Color(255, 255, 255) : MC_Color(175, 175, 175), 1.f, 1.f, true);
			}

			// Settings
			{
				std::vector<SettingEntry*>* settings = mod->getSettings();
				if (settings->size() > 2 && allowRender) {
					std::shared_ptr<ClickModule> clickMod = getClickModule(ourWindow, mod->getRawModuleName());
					if (rectPos.contains(&mousePos) && shouldToggleRightClick && !ourWindow->isInAnimation) {
						shouldToggleRightClick = false;
						clickMod->isExtended = !clickMod->isExtended;
					}

					currentYOffset += textHeight + (textPadding * 2);
					if (clickMod->isExtended) {
						float startYOffset = currentYOffset - 2;
						for (auto setting : *settings) {
							if (strcmp(setting->name, "enabled") == 0 || !setting->visible) continue;

							vec2_t textPos = vec2_t(currentXOffset + textPadding + 6, currentYOffset + textPadding + 1);
							vec4_t rectPos = vec4_t(currentXOffset + 1, currentYOffset, xEnd - 1, 0);/*
							if ((currentYOffset - ourWindow->pos.y) > cutoffHeight) {
								overflowing = true;
								break;
							}*/

							string len = "saturation  ";
							switch (setting->valueType) {
							case ValueType::BOOL_T: {
								currentYOffset += 0.5f;
								vec4_t rectPos = vec4_t(currentXOffset, currentYOffset - 2.f, xEnd, currentYOffset + textHeight + (textPadding * 2) + 0.5f - 2.f);
								DrawUtils::fillRectangle(vec4_t(rectPos.x, rectPos.y - 0.5f, rectPos.z, rectPos.y), MC_Color(0, 0, 0), opacity);
								if (!setting->value->_bool) {
									DrawUtils::fillRectangle((rectPos), MC_Color(0, 0, 0), opacity);
								}
								else {
									DrawUtils::fillRectangle((vec4_t(rectPos.x, rectPos.y, rectPos.x + 3.5f, rectPos.w)), MC_Color(0, 0, 0), opacity);
									DrawUtils::fillRectangle((vec4_t(rectPos.x + 3.5f, rectPos.y, rectPos.z - 1.5f, rectPos.w)), MC_Color(ColorUtil::interfaceColor(1)), opacity);
									DrawUtils::fillRectangle((vec4_t(rectPos.z - 1.5f, rectPos.y, rectPos.z, rectPos.w)), MC_Color(0, 0, 0), opacity);
								}

								float TextHeight = DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() * 1.f;
								vec2_t textPos = vec2_t(currentXOffset + textPadding + 4.f, (rectPos.y + rectPos.w) / 2 - ncdb4 / 2 + 1.5f);
								char name[0x21];
								sprintf_s(name, 0x21, "%s", setting->name);
								if (name[0] != 0) name[0] = toupper(name[0]);
								std::string elTexto = name;

								// Logic
								bool isFocused = rectPos.contains(&mousePos);
								if (isFocused) {
									DrawUtils::fillRectangle((vec4_t(rectPos.x + 3.5f, rectPos.y, rectPos.z - 1.5f, rectPos.w)), MC_Color(255, 255, 255), opacity);
								}
								DrawUtils::drawText(textPos, &elTexto, isFocused || setting->value->_bool ? MC_Color(255, 255, 255) : MC_Color(155, 155, 155), textSize, 1.f, true);
								{
									if (isFocused && shouldToggleLeftClick && !ourWindow->isInAnimation) {
										shouldToggleLeftClick = false;
										setting->value->_bool = !setting->value->_bool;

									}
								}

								currentYOffset += textHeight + (textPadding * 2) + 0.5f;
								break;
							}
							case ValueType::ENUM_T: {
								currentYOffset += 0.5f;
								vec4_t rectPos = vec4_t(currentXOffset, currentYOffset - 2.f, xEnd, currentYOffset + textHeight + (textPadding * 2) + 0.5f - 2.f);
								DrawUtils::fillRectangle(vec4_t(rectPos.x, rectPos.y - 0.5f, rectPos.z, rectPos.y), MC_Color(0, 0, 0), opacity);
								DrawUtils::fillRectangle(rectPos, MC_Color(0, 0, 0), opacity);

								char name[0x22];
								sprintf_s(name, +"%s:", setting->name);
								if (name[0] != 0) name[0] = toupper(name[0]);

								EnumEntry& i = ((SettingEnum*)setting->extraData)->GetSelectedEntry();

								char name2[0x21];
								sprintf_s(name2, 0x21, " %s", i.GetName().c_str());
								if (name2[0] != 0) name2[0] = toupper(name2[0]);
								std::string elTexto2 = std::string(GRAY) + name2;

								std::string elTexto = name;  // + string(GRAY) + elTexto2;

								float TextHeight = DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() * 1.f;
								vec2_t textPos = vec2_t(currentXOffset + textPadding + 4.f, (rectPos.y + rectPos.w) / 2 - ncdb4 / 2 + 1.5f);
								DrawUtils::drawText(textPos, &elTexto, MC_Color(255, 255, 255), textSize, 1.f, true);
								DrawUtils::drawText(vec2_t(rectPos.z - 1.5f - DrawUtils::getTextWidth(&elTexto2, 1.f), textPos.y), &elTexto2, MC_Color(255, 255, 255), textSize, 1.f, true);

								// logic
								if (rectPos.contains(&mousePos)) {
									DrawUtils::fillRectangle((vec4_t(rectPos.x + 3.5f, rectPos.y, rectPos.z - 1.5f, rectPos.w)), MC_Color(255, 255, 255), opacity);
								}
								if (!ourWindow->isInAnimation && rectPos.contains(&mousePos)) {
									if (shouldToggleLeftClick) {
										shouldToggleLeftClick = false;
										((SettingEnum*)setting->extraData)->SelectNextValue(false);

									}
									else if (shouldToggleRightClick) {
										shouldToggleRightClick = false;
										((SettingEnum*)setting->extraData)->SelectNextValue(true);

									}
								}
								currentYOffset += textHeight + (textPadding * 2) + 0.5f;
								break;
							}
							case ValueType::FLOAT_T: {
								currentYOffset += 0.5f;
								vec4_t rectPos = vec4_t(currentXOffset, currentYOffset - 2.f, xEnd, currentYOffset + textHeight + (textPadding * 2) + 0.5f - 2.f);
								DrawUtils::fillRectangle(vec4_t(rectPos.x, rectPos.y - 0.5f, rectPos.z, rectPos.y), MC_Color(0, 0, 0), opacity);
								// DrawUtils::fillRectangle(rectPos, MC_Color(0, 0, 0), opacity);
								float TextHeight = DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() * 1.f;
								vec2_t textPos = vec2_t(currentXOffset + textPadding + 4.f, (rectPos.y + rectPos.w) / 2 - ncdb4 / 2 + 1.5f);
								char str[10];
								sprintf_s(str, 10, "%.2f", setting->value->_float);
								std::string text = str;
								char name[0x22];
								sprintf_s(name, "%s: ", setting->name);
								if (name[0] != 0) name[0] = toupper(name[0]);
								std::string elTexto = name;  // + text;
								DrawUtils::drawText(textPos, &elTexto, MC_Color(255, 255, 255), textSize, 1.f, true);
								DrawUtils::drawText(vec2_t(rectPos.z - 1.5f - DrawUtils::getTextWidth(&text, 1.f), textPos.y), &text, MC_Color(255, 255, 255), textSize, 1.f, true);

								DrawUtils::fillRectangle(vec4_t(rectPos.x, rectPos.y, rectPos.x + 3.5f, rectPos.w), MC_Color(0, 0, 0), opacity);  // Code dung
								DrawUtils::fillRectangle(vec4_t(rectPos.z - 1.5f, rectPos.y, rectPos.z, rectPos.w), MC_Color(0, 0, 0), opacity);  // Code dung

								vec4_t rect = vec4_t(rectPos.x + 3.5f, rectPos.y, rectPos.z - 1.5f, rectPos.w);

								const float minValue = setting->minValue->_float;
								const float maxValue = setting->maxValue->_float - minValue;

								float value = (float)fmax(0, setting->value->_float - minValue);
								if (value > maxValue) value = maxValue;
								value /= maxValue;

								const float endXlol = rectPos.z - 1.5f - rectPos.x - 3.5f;
								value *= endXlol;

								const bool areWeFocused = rectPos.contains(&mousePos);

								{
									rect.z = rect.x + value;
									DrawUtils::fillRectangle(rect, MC_Color(ColorUtil::interfaceColor(1)), opacity);
									DrawUtils::fillRectangle(vec4_t(rect.z, rectPos.y, rectPos.z - 1.5f, rectPos.w), MC_Color(0, 0, 0), opacity);
								}

								// Drag Logic
								{
									if (setting->isDragging) {
										if (isLeftClickDown && !isRightClickDown) {
											value = mousePos.x - rect.x;
											DrawUtils::fillRectangle(rect, MC_Color(0, 0, 0), opacity);
										}
										else
											setting->isDragging = false;
									}
									else if (areWeFocused && shouldToggleLeftClick && !ourWindow->isInAnimation) {
										shouldToggleLeftClick = false;
										setting->isDragging = true;
									}
									if (areWeFocused && !isLeftClickDown && !ourWindow->isInAnimation) {
										DrawUtils::fillRectangle(rect, MC_Color(255, 255, 255), opacity);
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
								// DrawUtils::fillRectangle(vec4_t(rectPos.z - 2.f, rectPos.y, rectPos.z, rectPos.w), MC_Color(0, 0, 0), opacity);
								// DrawUtils::fillRectangle(vec4_t(rectPos.x + 3.5f, rectPos.y, rectPos.z - 2.f, rectPos.w), MC_Color(ColorUtil::interfaceColor(1)), opacity);

								currentYOffset += textHeight + (textPadding * 2) + 0.5f;
								break;
							}
							case ValueType::INT_T: {
								currentYOffset += 0.5f;
								vec4_t rectPos = vec4_t(currentXOffset, currentYOffset - 2.f, xEnd, currentYOffset + textHeight + (textPadding * 2) + 0.5f - 2.f);
								DrawUtils::fillRectangle(vec4_t(rectPos.x, rectPos.y - 0.5f, rectPos.z, rectPos.y), MC_Color(0, 0, 0), opacity);
								// DrawUtils::fillRectangle(rectPos, MC_Color(0, 0, 0), opacity);
								float TextHeight = DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() * 1.f;
								vec2_t textPos = vec2_t(currentXOffset + textPadding + 4.f, (rectPos.y + rectPos.w) / 2 - ncdb4 / 2 + 1.5f);

								char name[0x22];
								sprintf_s(name, "%s: ", setting->name);
								if (name[0] != 0) name[0] = toupper(name[0]);
								char str[10];
								sprintf_s(str, 10, "%i", setting->value->_int);
								std::string text = str;
								std::string elTexto = name;  // +text;

								DrawUtils::drawText(textPos, &elTexto, MC_Color(255, 255, 255), textSize, 1.f, true);
								DrawUtils::drawText(vec2_t(rectPos.z - 1.5f - DrawUtils::getTextWidth(&text, 1.f), textPos.y), &text, MC_Color(255, 255, 255), textSize, 1.f, true);

								DrawUtils::fillRectangle(vec4_t(rectPos.x, rectPos.y, rectPos.x + 3.5f, rectPos.w), MC_Color(0, 0, 0), opacity);  // Code dung
								DrawUtils::fillRectangle(vec4_t(rectPos.z - 1.5f, rectPos.y, rectPos.z, rectPos.w), MC_Color(0, 0, 0), opacity);  // Code dung

								currentYOffset += textHeight + (textPadding * 2) + 0.5f;

								const bool areWeFocused = rectPos.contains(&mousePos);
								vec4_t rect = vec4_t(rectPos.x + 3.5f, rectPos.y, rectPos.z - 1.5f, rectPos.w);

								const float minValue = (float)setting->minValue->_int;
								const float maxValue = (float)setting->maxValue->_int - minValue;
								float value = (float)fmax(0, setting->value->_int - minValue);  // Value is now always > 0 && < maxValue
								if (value > maxValue) value = maxValue;
								value /= maxValue;  // Value is now in range 0 - 1
								const float endXlol = rectPos.z - 1.5f - rectPos.x - 3.5f;
								value *= endXlol;  // Value is now pixel diff between start of bar and end of progress

								{
									rect.z = rect.x + value;
									DrawUtils::fillRectangle(rect, MC_Color(ColorUtil::interfaceColor(1)), opacity);
									DrawUtils::fillRectangle(vec4_t(rect.z, rectPos.y, rectPos.z - 1.5f, rectPos.w), MC_Color(0, 0, 0), opacity);
								}

								// Drag Logic
								{
									if (setting->isDragging) {
										if (isLeftClickDown && !isRightClickDown) {
											value = mousePos.x - rect.x;
											DrawUtils::fillRectangle(rect, MC_Color(0, 0, 0), opacity);
										}
										else
											setting->isDragging = false;
									}
									else if (areWeFocused && shouldToggleLeftClick && !ourWindow->isInAnimation) {
										shouldToggleLeftClick = false;
										setting->isDragging = true;

									}
									if (areWeFocused && !isLeftClickDown && !ourWindow->isInAnimation) {
										DrawUtils::fillRectangle(rect, MC_Color(255, 255, 255), opacity);
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
								break;
							} break;
							case ValueType::KEYBIND_T: {
								vec4_t rectPos = vec4_t(currentXOffset, currentYOffset - 1.7f, xEnd, currentYOffset + textHeight + (textPadding * 2) + 0.5f - 2.5f);
								vec2_t textPos = vec2_t(currentXOffset + textPadding + 4.f, (rectPos.y + rectPos.w) / 2 - ncdb4 / 2 + 1.f);
								DrawUtils::fillRectangleA(rectPos, MC_Color(0, 0, 0, opacity * 255));
								char name[0x21];
								sprintf_s(name, 0x21, "%s:", setting->name);
								if (name[0] != 0)
									name[0] = toupper(name[0]);

								std::string text = name;


								if (!isCapturingKey || (keybindMenuCurrent != setting && isCapturingKey)) {
									DrawUtils::drawText(textPos, &text, MC_Color(255, 255, 255), textSize, true);

									const char* key;

									if (setting->value->_int > 0 && setting->value->_int < 190)
										key = KeyNames[setting->value->_int];
									else if (setting->value->_int == 0x0)
										key = "NONE";
									else
										key = "???";

									if (keybindMenuCurrent == setting && isCapturingKey) {
										key = "...";
									}
									else if (keybindMenuCurrent == setting && isConfirmingKey) {
										if (newKeybind > 0 && newKeybind < 190)
											key = KeyNames[newKeybind];
										else if (newKeybind == 0x0)
											key = "N/A";
										else
											key = "???";
									}

									std::string keyb = std::string(GRAY) + key;
									float keybSz = textHeight * 0.8f;

									float length = 10.f;  // because we add 5 to text padding + keybind name
									length += DrawUtils::getTextWidth(&text, textSize);
									length += DrawUtils::getTextWidth(&keyb, textSize);

									// windowSize.x = fmax(windowSize.x, length + offset);
									vec2_t textPos2(rectPos.z, textPos.y);
									textPos2.x -= DrawUtils::getTextWidth(&keyb, textSize);
									textPos2.x -= textPadding;
									if (setting->value->_int == 0x0)
										DrawUtils::drawText(textPos2, &keyb, MC_Color(255, 255, 255), textSize, true);
									else
										DrawUtils::drawText(textPos2, &keyb, MC_Color(ColorUtil::interfaceColor(1)), textSize, true);

								}
								else {
									// windowSize.x = fmax(windowSize.x, DrawUtils::getTextWidth(&text, textSize));
									std::string text = "Press new bind...";
									DrawUtils::drawText(textPos, &text, MC_Color(255, 255, 255), textSize, true);
								}

								// DrawUtils::fillRectangle(rectPos, FILLCOLOR, OPACITY);

								if ((currentYOffset - ourWindow->pos.y) > (g_Data.getGuiData()->heightGame * 0.75)) {
									overflowing = true;
									break;
								}

								// Logic
								{
									bool isFocused = rectPos.contains(&mousePos);

									if (isFocused && shouldToggleLeftClick && !(isCapturingKey && keybindMenuCurrent != setting /*don't let the user click other stuff while changing a keybind*/)) {


										keybindMenuCurrent = setting;
										isCapturingKey = true;
									}

									if (isFocused && shouldToggleRightClick && !(isCapturingKey && keybindMenuCurrent != setting)) {


										setting->value->_int = 0x0;  // Clear

										isCapturingKey = false;
									}

									if (shouldStopCapturing && keybindMenuCurrent == setting) {  // The user has selected a key


										shouldStopCapturing = false;
										isCapturingKey = false;
										setting->value->_int = newKeybind;
									}
								}
								currentYOffset += textHeight + (textPadding * 2);
							} break;
							}
						}
						float endYOffset = currentYOffset;
						startYOffset += textPadding;
						endYOffset -= textPadding;
						MC_Color interfaceColor = ColorUtil::interfaceColor(0);
						DrawUtils::fillRectangle(vec4_t(currentXOffset + 1, startYOffset, currentXOffset + 2.5f, endYOffset - 0.7f), interfaceColor, 0.8f);
					}
				}
				else currentYOffset += textHeight + (textPadding * 2) + 2.f;
			}
		}

		vec4_t winRectPos = vec4_t(xOffset, yOffset, xEnd, currentYOffset);

		if (winRectPos.contains(&mousePos)) {
			if (scrollingDirection > 0 && overflowing) ourWindow->yOffset += scrollingDirection;
			else if (scrollingDirection < 0) ourWindow->yOffset += scrollingDirection;
			scrollingDirection = 0;
			if (ourWindow->yOffset < 0) ourWindow->yOffset = 0;
		}
	}
	DrawUtils::flush();
	// Draw Category Header
	{
		vec2_t textPos = vec2_t(currentXOffset + textPadding + 1, categoryHeaderYOffset + textPadding);
		vec4_t rectPos = vec4_t(
			currentXOffset - categoryMargin, categoryHeaderYOffset - categoryMargin,
			currentXOffset + windowSize->x + paddingRight + categoryMargin,
			categoryHeaderYOffset + textHeight + (textPadding * 2));
		vec4_t rectPos2 = vec4_t(
			currentXOffset - categoryMargin, categoryHeaderYOffset + categoryMargin + 9.5,
			currentXOffset + windowSize->x + paddingRight + categoryMargin,
			categoryHeaderYOffset + textHeight + (textPadding * 2));
		vec4_t rectTest = vec4_t(rectPos.x, rectPos.y + 1, rectPos.z, rectPos.w);
		vec4_t rectTest2 = vec4_t(rectPos.x + 1.f, rectPos.y + 2, rectPos.z - 1.f, rectPos.w);

		for (auto& mod : moduleList) {
			rectTest.w = currentYOffset;
			rectTest2.w = currentYOffset - 2;
		}

		// Extend Logic
		{
			if (rectPos.contains(&mousePos) && shouldToggleRightClick && !isDragging) {
				shouldToggleRightClick = false;
				ourWindow->isExtended = !ourWindow->isExtended;
				if (ourWindow->isExtended && ourWindow->animation == 0) ourWindow->animation = 0.2f;
				else if (!ourWindow->isExtended && ourWindow->animation == 1) ourWindow->animation = 0;
				ourWindow->isInAnimation = true;

				for (auto& mod : moduleList) {
					std::shared_ptr<ClickModule> clickMod = getClickModule(ourWindow, mod->getRawModuleName());
					clickMod->isExtended = false;
				}
			}
		}

		// Dragging Logic
		{
			if (isDragging && Utils::getCrcHash(categoryName) == draggedWindow) {
				if (isLeftClickDown) {
					vec2_t diff = vec2_t(mousePos).sub(dragStart);
					ourWindow->pos = ourWindow->pos.add(diff);
					dragStart = mousePos;
				}
				else {
					isDragging = false;
				}
			}
			else if (rectPos.contains(&mousePos) && shouldToggleLeftClick) {
				isDragging = true;
				draggedWindow = Utils::getCrcHash(categoryName);
				shouldToggleLeftClick = false;
				dragStart = mousePos;
			}
		}

		//Draw a bit more then just the HudEditor button
		/*{
			std::vector<SettingEntry*>* settings = clickGUI->getSettings();
			string textStr = "Packet";
			float textStrLen = DrawUtils::getTextWidth(&string("------------")) - 2.f;
			float textStrLen2 = DrawUtils::getTextWidth(&string("--------------"));
			float stringLen = DrawUtils::getTextWidth(&textStr) + 2;
			vec2_t windowSize2 = g_Data.getClientInstance()->getGuiData()->windowSize;
			float mid = windowSize2.x / 2 - 20;

			vec4_t rect = vec4_t(mid, 0, mid + textStrLen, 18);
			vec4_t settingsRect = vec4_t(rect.x + stringLen + 3, rect.y + 2, rect.x + stringLen + 17, rect.y + 16);
			vec4_t hudEditor = vec4_t(rect.x + stringLen + 19, rect.y + 2, rect.x + stringLen + 33, rect.y + 16);

			if (hudEditor.contains(&mousePos) && shouldToggleLeftClick) clickGUI->setEnabled(false);

			DrawUtils::fillRectangleA(rect, MC_Color(37, 39, 43, 255));
			DrawUtils::fillRectangleA(settingsRect, MC_Color(9, 12, 16, 255));
			DrawUtils::fillRectangleA(hudEditor, MC_Color(15, 20, 26, 255));
			DrawUtils::drawText(vec2_t(rect.x + 3, rect.y + 4), &textStr, MC_Color(255, 255, 255), 1.f, 1.f, true);

			float ourOffset = 17;
			static bool extended = false;

			if (settingsRect.contains(&mousePos) && shouldToggleRightClick) {
				shouldToggleRightClick = false;
				extended = !extended;
			}

			vec4_t idkRect = vec4_t(settingsRect.x, ourOffset, settingsRect.x + textStrLen2, ourOffset + 16);
			for (int t = 0; t < 4; t++)	idkRect.w += ourOffset;

			if (extended) {
				DrawUtils::fillRectangleA(idkRect, MC_Color(45, 45, 45, 255));
				string stringAids;
				string stringAids2;
				if (clickGUI->theme.getSelectedValue() == 0) stringAids = "Theme: Packet";
				if (clickGUI->theme.getSelectedValue() == 1) stringAids = "Theme: Vape";
				if (clickGUI->theme.getSelectedValue() == 2) stringAids = "Theme: Astolfo";

				if (clickGUI->color.getSelectedValue() == 0) stringAids2 = "Color: Rainbow";
				if (clickGUI->color.getSelectedValue() == 1) stringAids2 = "Color: Astolfo";
				if (clickGUI->color.getSelectedValue() == 2) stringAids2 = "Color: Wave";
				if (clickGUI->color.getSelectedValue() == 3) stringAids2 = "Color: RainbowWave";

				vec4_t selectableSurface = vec4_t(settingsRect.x, ourOffset + 2.f, settingsRect.x + textStrLen2, ourOffset + 17.f);
				vec4_t selectableSurface2 = vec4_t(settingsRect.x, ourOffset + 17.f, settingsRect.x + textStrLen2, ourOffset + 37.f);
				DrawUtils::drawText(vec2_t(selectableSurface.x + 2, selectableSurface.y + 3), &stringAids, MC_Color(255, 255, 255), 1.f, 1.f, true);
				DrawUtils::drawText(vec2_t(selectableSurface2.x + 2, selectableSurface2.y + 3), &stringAids2, MC_Color(255, 255, 255), 1.f, 1.f, true);

				if (selectableSurface.contains(&mousePos) && shouldToggleLeftClick) {
					clickGUI->theme.SelectNextValue(false);
					shouldToggleLeftClick = false;
				}
				if (selectableSurface.contains(&mousePos) && shouldToggleRightClick) {
					clickGUI->theme.SelectNextValue(true);
					shouldToggleLeftClick = false;
				}
				if (selectableSurface2.contains(&mousePos) && shouldToggleLeftClick) {
					clickGUI->color.SelectNextValue(false);
					shouldToggleLeftClick = false;
				}
				if (selectableSurface2.contains(&mousePos) && shouldToggleRightClick) {
					clickGUI->color.SelectNextValue(true);
					shouldToggleLeftClick = false;
				}
			}
		}*/

		// Draw category header
		{
			// Draw Text
			auto interfaceColor = ColorUtil::interfaceColor(1);
			string textStr = categoryName;
			DrawUtils::drawText(textPos, &textStr, MC_Color(255, 255, 255), 1.f, 1.f, true);
			//DrawUtils::drawText(textPos, &textStr, MC_Color(255,255,255), textSize,true);
			DrawUtils::fillRectangle(vec4_t(rectPos.x - 0, rectPos.y - 2, rectPos.z + 0.1, rectPos.w), MC_Color(interfaceColor), opacity);
			/*
			if (clickGUI->ModuleOutline) {
				DrawUtils::drawRectangle(vec4_t(rectPos.x - 0.25, rectPos.y - 2, rectPos.z + 0.25, rectPos.w), MC_Color(0.f, 0.f, 0.f), 0.5f);
			}
			*/ // Outline Category

			//DrawUtils::fillRectangleA(rectPos2, MC_Color(interfaceColor));
			//DrawUtils::drawRectangle(rectTest, MC_Color(interfaceColor), 1.f, 1.f);
			//DrawUtils::fillRectangleA(vec4_t(rectTest), MC_Color(0, 0, 0, 180));

			//DrawUtils::drawRoundRectangle(rectTest2, MC_Color(100, 100, 100), true);
		}
	}

	// anti idiot
	{
		vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
		if (ourWindow->pos.x + ourWindow->size.x > windowSize.x) ourWindow->pos.x = windowSize.x - ourWindow->size.x;
		if (ourWindow->pos.y + ourWindow->size.y > windowSize.y) ourWindow->pos.y = windowSize.y - ourWindow->size.y;
		ourWindow->pos.x = (float)fmax(0, ourWindow->pos.x);
		ourWindow->pos.y = (float)fmax(0, ourWindow->pos.y);
	}

	moduleList.clear();
	DrawUtils::flush();
}
#pragma endregion
void ClickGui::render() {
	static auto clickGUI = moduleMgr->getModule<ClickGUIMod>();
	auto interfaceModDM = ColorUtil::interfaceColor(1);
	if (!moduleMgr->isInitialized()) return;

	if (timesRendered < 1) timesRendered++;

	// Fill Background
	{
		DrawUtils::fillRectangle(vec4_t(0, 0,
			g_Data.getClientInstance()->getGuiData()->widthGame,
			g_Data.getClientInstance()->getGuiData()->heightGame),
			MC_Color(0, 0, 0), 0.3);
		//DrawUtils::drawRectangle(vec4_t(0, 0, g_Data.getClientInstance()->getGuiData()->widthGame, g_Data.getClientInstance()->getGuiData()->heightGame), MC_Color(interfaceDM), 1.f, 2.f);
	}

	// Render all categorys

	renderPacketCategory(Category::COMBAT, MC_Color(255, 255, 255));
	renderPacketCategory(Category::VISUAL, MC_Color(255, 255, 255));
	renderPacketCategory(Category::MOVEMENT, MC_Color(255, 255, 255));
	renderPacketCategory(Category::PLAYER, MC_Color(255, 255, 255));
	renderPacketCategory(Category::EXPLOIT, MC_Color(255, 255, 255));
	renderPacketCategory(Category::OTHER, MC_Color(255, 255, 255));


	shouldToggleLeftClick = false;
	shouldToggleRightClick = false;
	resetStartPos = false;
	DrawUtils::flush();
}

void ClickGui::init() { initialised = true; }

void ClickGui::onMouseClickUpdate(int key, bool isDown) {
	static auto clickGUI = moduleMgr->getModule<ClickGUIMod>();
	if (clickGUI->isEnabled() && g_Data.isInGame()) {
		switch (key) {
		case 1:  // Left Click
			isLeftClickDown = isDown;
			if (isDown)
				shouldToggleLeftClick = true;
			break;
		case 2:  // Right Click
			isRightClickDown = isDown;
			if (isDown)
				shouldToggleRightClick = true;
			break;
		}
	}
}

void ClickGui::onWheelScroll(bool direction) {
	if (!direction) scrollingDirection++;
	else scrollingDirection--;
}

void ClickGui::onKeyUpdate(int key, bool isDown) {
	if (!initialised)
		return;
	static auto clickGuiMod = moduleMgr->getModule<ClickGUIMod>();

	if (!isDown)
		return;

	if (!clickGuiMod->isEnabled()) {
		timesRendered = 0;
		return;
	}
	if (isCapturingKey && !shouldStopCapturing) {
		if (VK_DELETE == key || VK_ESCAPE == key)
			newKeybind = 0x0;
		else
			newKeybind = key;
		shouldStopCapturing = true;
	}
	if (timesRendered < 10)
		return;
	timesRendered = 0;

	switch (key) {
	case VK_ESCAPE:
		clickGuiMod->setEnabled(false);
		return;
	default:
		if (key == clickGuiMod->getKeybind())
			clickGuiMod->setEnabled(false);
	}
}

#pragma region Config Stuff
using json = nlohmann::json;
void ClickGui::onLoadConfig(void* confVoid) {
	savedWindowSettings.clear();
	windowMap.clear();
	json* conf = reinterpret_cast<json*>(confVoid);
	if (conf->contains("ClickGuiMenu")) {
		auto obj = conf->at("ClickGuiMenu");
		if (obj.is_null())
			return;
		for (int i = 0; i <= (int)Category::CUSTOM; i++) {
			auto catName = ClickGui::catToName((Category)i);
			if (obj.contains(catName)) {
				auto value = obj.at(catName);
				if (value.is_null())
					continue;
				try {
					SavedWindowSettings windowSettings = {};
					windowSettings.name = catName;
					if (value.contains("pos")) {
						auto posVal = value.at("pos");
						if (!posVal.is_null() && posVal.contains("x") && posVal["x"].is_number_float() && posVal.contains("y") && posVal["y"].is_number_float()) {
							try {
								windowSettings.pos = { posVal["x"].get<float>(), posVal["y"].get<float>() };
							}
							catch (exception e) {
							}
						}
					}
					if (value.contains("isExtended")) {
						auto isExtVal = value.at("isExtended");
						if (!isExtVal.is_null() && isExtVal.is_boolean()) {
							try {
								windowSettings.isExtended = isExtVal.get<bool>();
							}
							catch (exception e) {
							}
						}
					}
					savedWindowSettings[Utils::getCrcHash(catName)] = windowSettings;
				}
				catch (exception e) {
					logF("Config Load Error (ClickGuiMenu): %s", e.what());
				}
			}
		}
	}
}
void ClickGui::onSaveConfig(void* confVoid) {
	json* conf = reinterpret_cast<json*>(confVoid);
	// First update our map
	for (const auto& wind : windowMap) {
		savedWindowSettings[wind.first] = { wind.second->pos, wind.second->isExtended, wind.second->name };
	}

	// Save to json
	if (conf->contains("ClickGuiMenu"))
		conf->erase("ClickGuiMenu");

	json obj = {};

	for (const auto& wind : savedWindowSettings) {
		json subObj = {};
		subObj["pos"]["x"] = wind.second.pos.x;
		subObj["pos"]["y"] = wind.second.pos.y;
		subObj["isExtended"] = wind.second.isExtended;
		obj[wind.second.name] = subObj;
	}

	conf->emplace("ClickGuiMenu", obj);
}
#pragma endregion