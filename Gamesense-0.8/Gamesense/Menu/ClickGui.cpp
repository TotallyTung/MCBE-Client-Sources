#include "ClickGui.h"

#include <Windows.h>

#include "../../Utils/Json.hpp"
#include "../../Utils/Logger.h"
#include "../../Utils/DrawUtils.h"
#include "../../Utils/ImGuiUtils.h"
#include "../Module/Modules/ClickGuiMod.h"

// #define min(a, b) (((a) < (b)) ? (a) : (b))

bool isLeftClickDown = false;
bool isRightClickDown = false;
bool shouldToggleLeftClick = false; // If true, toggle the focused module
bool shouldToggleRightClick = false;
bool resetStartPos = true;
bool initialised = false;
int scrollingDirection = 0;
using namespace std;
SettingEntry *keybindMenuCurrent = nullptr; // What setting is currently capturing the user's input?
int newKeybind = 0;
bool isCapturingKey = false;
bool shouldStopCapturing = false;

bool isConfirmingKey = false; // Show a cancel and save button

struct SavedWindowSettings
{
	vec2_t pos = {-1, -1};
	bool isExtended = true;
	const char *name = "";
};

std::map<unsigned int, std::shared_ptr<ClickWindow>> windowMap;
std::map<unsigned int, SavedWindowSettings> savedWindowSettings;

bool isDragging = false;
unsigned int draggedWindow = -1;
vec2_t dragStart = vec2_t();

unsigned int focusedElement = -1;
bool isFocused = false;

static constexpr float textPadding = 1.0f;
static constexpr float textSize = 0.98f;
static constexpr float textHeight = textSize * 9.0f;
static constexpr float categoryMargin = 0.5f;
static constexpr float paddingRight = 13.5f;
static constexpr float crossSize = textHeight / 2.f;
static constexpr float crossWidth = 0.5f;

float currentYOffset = 0;
float currentXOffset = 0;

int timesRendered = 0;

void ClickGui::getModuleListByCategory(Category category, vector<shared_ptr<IModule>> *modList)
{
	auto lock = moduleMgr->lockModuleList();
	vector<shared_ptr<IModule>> *moduleList = moduleMgr->getModuleList();

	for (auto &it : *moduleList)
	{
		if (it->getCategory() == category)
			modList->push_back(it);
	}
}

shared_ptr<ClickWindow> ClickGui::getWindow(const char *name)
{
	unsigned int id = Utils::getCrcHash(name);

	auto search = windowMap.find(id);
	if (search != windowMap.end())
	{ // Window exists already
		return search->second;
	}
	else
	{ // Create window
		// TODO: restore settings for position etc
		shared_ptr<ClickWindow> newWindow = make_shared<ClickWindow>();
		newWindow->name = name;

		auto savedSearch = savedWindowSettings.find(id);
		if (savedSearch != savedWindowSettings.end())
		{ // Use values from config
			newWindow->isExtended = savedSearch->second.isExtended;
			if (savedSearch->second.pos.x > 0)
				newWindow->pos = savedSearch->second.pos;
		}

		windowMap.insert(make_pair(id, newWindow));
		return newWindow;
	}
}

shared_ptr<ClickModule> ClickGui::getClickModule(shared_ptr<ClickWindow> window, const char *name)
{
	unsigned int id = Utils::getCrcHash(name);

	auto search = window->moduleMap.find(id);
	if (search != window->moduleMap.end())
	{ // Window exists already
		return search->second;
	}
	else
	{ // Create window
		// TODO: restore settings for position etc
		shared_ptr<ClickModule> newModule = make_shared<ClickModule>();

		window->moduleMap.insert(make_pair(id, newModule));
		return newModule;
	}
}

void ClickGui::renderLabel(const char *text)
{
}

void ClickGui::renderTooltip(string *text)
{
	auto interfaceColor = ColorUtil::interfaceColor(1);
	static auto clickGuiMod = moduleMgr->getModule<ClickGUIMod>();
	bool shouldShadow = true;
	if (clickGuiMod->cFont)
		shouldShadow = false;
	else if (!clickGuiMod->cFont)
		shouldShadow = true;
	vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	vec2_t windowSizeReal = g_Data.getClientInstance()->getGuiData()->windowSizeReal;
	float lFPS = DrawUtils::getTextWidth(text, 1) + 6.5;
	float posY = windowSize.y - 24;
	float posY2 = windowSize.y - 38;
	vec4_t rectPos = vec4_t(2 + 2, posY, lFPS + 11, posY + 17);
	vec4_t rectPos2 = vec4_t(2 + 2, posY2, lFPS + 11, posY2 + 15);
	vec2_t textPos = vec2_t(rectPos.x + textPadding + 2, rectPos.y + 4);
	vec2_t textPos2 = vec2_t(rectPos2.x + textPadding + 2, rectPos.y - 11);
	vec2_t mousePos = *g_Data.getClientInstance()->getMousePos();
	mousePos = mousePos.div(windowSizeReal);
	mousePos = mousePos.mul(windowSize);
	vec2_t currentTooltipPos = vec2_t(5.f, windowSize.y - 15.f);
	float textWidth = DrawUtils::getTextWidth(text, textSize);
	vec2_t textPosD = vec2_t(
		mousePos.x + 2.f + 2.f,
		mousePos.y + 2.f);
	vec4_t rectPosD = vec4_t(
		mousePos.x,
		mousePos.y,
		mousePos.x + (2.f * 2) + textWidth + 2.f,
		mousePos.y + textHeight + 2.f);

	if (clickGuiMod->desc.getSelectedValue() == 1)
	{
		DrawUtils::fillRectangleA(rectPos, MC_Color(0, 0, 0, 130));
		DrawUtils::drawBottomLine(vec4_t(rectPos), ColorUtil::interfaceColor(1), 3);
		DrawUtils::drawText(textPos, text, MC_Color(255, 255, 255), 1.f, 1.f, shouldShadow);
	}

	if (clickGuiMod->desc.getSelectedValue() == 2)
	{
		DrawUtils::fillRectangleA(rectPos, MC_Color(0, 0, 0, 130));
		DrawUtils::drawRectangle(rectPos, MC_Color(interfaceColor), 1.f, 1.f);
		DrawUtils::drawText(textPos, text, MC_Color(255, 255, 255), 1.f, 1.f, shouldShadow);
	}
	if (clickGuiMod->desc.getSelectedValue() == 3)
	{
		DrawUtils::fillRectangleA(rectPos, MC_Color(0, 0, 0, 130));
		DrawUtils::drawText(textPos, text, MC_Color(255, 255, 255), 1.f, 1.f, shouldShadow);
	}
	if (clickGuiMod->desc.getSelectedValue() == 4)
	{
		string descr = "Description";
		DrawUtils::drawText(textPos2, &descr, MC_Color(255, 255, 255), 1.f, 1.f, shouldShadow);
		DrawUtils::fillRectangleA(rectPos2, MC_Color(interfaceColor));
		DrawUtils::fillRectangleA(rectPos, MC_Color(0, 0, 0, 130));
		DrawUtils::drawRectangle(rectPos2, MC_Color(interfaceColor), 1.f, 1.f);
		DrawUtils::drawRectangle(rectPos, MC_Color(interfaceColor), 1.f, 1.f);
		DrawUtils::drawText(textPos, text, MC_Color(255, 255, 255), 1.f, 1.f, shouldShadow);
	}
	if (clickGuiMod->desc.getSelectedValue() == 5)
	{
		DrawUtils::fillRectangle(rectPosD, MC_Color(0, 0, 0), 0.4f);
		DrawUtils::drawText(textPosD, text, MC_Color(255, 255, 255), textSize);
	}
}

//*
std::vector<Category> categoryOrder;
std::vector<Category> nextOrder;

void ClickGui::setScale(float newscale)
{
}

void ClickGui::renderCategory(Category category)
{
	static constexpr float textHeight = textSize * 9.f;
	const char* categoryName = ClickGui::catToName(category);
	static auto clickGUI = moduleMgr->getModule<ClickGUIMod>();
	const std::shared_ptr<ClickWindow> ourWindow = getWindow(categoryName);
	vec2_t windowSizeccc = g_Data.getClientInstance()->getGuiData()->windowSize;

	// Reset Windows to pre-set positions to avoid confusion
	if (resetStartPos && ourWindow->pos.x <= 0) {
		float yot = g_Data.getGuiData()->windowSize.x;
		ourWindow->pos.y = 4;
		switch (category) {
		case Category::COMBAT:
			ourWindow->pos.x = 7.5f;
			ourWindow->pos.y = 7.5f;
			break;
		case Category::VISUAL:
			ourWindow->pos.x = 120.f;
			ourWindow->pos.y = 7.5f;
			break;
		case Category::MOVEMENT:
			ourWindow->pos.x = 232.5f;
			ourWindow->pos.y = 7.5f;
			break;
		case Category::PLAYER:
			ourWindow->pos.x = 345.f;
			ourWindow->pos.y = 7.5f;
			break;
		case Category::EXPLOIT:
			ourWindow->pos.x = 457.5f;
			ourWindow->pos.y = 7.5f;
			break;
		case Category::OTHER:
			ourWindow->pos.x = 570.f;
			ourWindow->pos.y = 7.5f;
			break;
		case Category::UNUSED:
			ourWindow->pos.x = yot / 1.6f * 2.2f;
			ourWindow->pos.y = 7.5f;
			break;
		case Category::CUSTOM:
			ourWindow->pos.x = yot / 5.6f * 2.f;
			ourWindow->pos.y = 7.5f;
			break;

		}
	}
	if (clickGUI->animations) {
		if (clickGUI->openAnim < 27) ourWindow->pos.y = clickGUI->openAnim;
	}

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
			std::string label = "---------------"; //MC font
			windowSize->x = fmax(windowSize->x, DrawUtils::getTextWidth(&label, textSize, Fonts::SMOOTH));
		}
	}

	const std::shared_ptr<ClickWindow> ourWindowCOMBAT = getWindow(ClickGui::catToName(Category::COMBAT));
	const std::shared_ptr<ClickWindow> ourWindowVISUAL = getWindow(ClickGui::catToName(Category::VISUAL));
	const std::shared_ptr<ClickWindow> ourWindowMOVEMENT = getWindow(ClickGui::catToName(Category::MOVEMENT));
	const std::shared_ptr<ClickWindow> ourWindowPLAYER = getWindow(ClickGui::catToName(Category::PLAYER));
	const std::shared_ptr<ClickWindow> ourWindowEXPLOIT = getWindow(ClickGui::catToName(Category::EXPLOIT));
	const std::shared_ptr<ClickWindow> ourWindowOTHER = getWindow(ClickGui::catToName(Category::OTHER));
	auto scrollSpeed = clickGUI->scrollSpeed * 5;
	if (scrollingDirection > 0) {
		ourWindowCOMBAT->pos.y += scrollingDirection + scrollSpeed;
		ourWindowVISUAL->pos.y += scrollingDirection + scrollSpeed;
		ourWindowMOVEMENT->pos.y += scrollingDirection + scrollSpeed;
		ourWindowPLAYER->pos.y += scrollingDirection + scrollSpeed;
		ourWindowEXPLOIT->pos.y += scrollingDirection + scrollSpeed;
		ourWindowOTHER->pos.y += scrollingDirection + scrollSpeed;
	}
	else if (scrollingDirection < 0) {
		ourWindowCOMBAT->pos.y -= scrollingDirection + scrollSpeed;
		ourWindowVISUAL->pos.y -= scrollingDirection + scrollSpeed;
		ourWindowMOVEMENT->pos.y -= scrollingDirection + scrollSpeed;
		ourWindowPLAYER->pos.y -= scrollingDirection + scrollSpeed;
		ourWindowEXPLOIT->pos.y -= scrollingDirection + scrollSpeed;
		ourWindowOTHER->pos.y -= scrollingDirection + scrollSpeed;
	}
	scrollingDirection = 0;

	vector<shared_ptr<IModule>> moduleList;
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

	currentYOffset += textHeight + (textPadding * 2) + 0.5f;
	if (ourWindow->isExtended) {
		if (ourWindow->isInAnimation) {
			currentYOffset -= clickGUI->animation * moduleList.size() * (textHeight + (textPadding * 2));
		}

		bool overflowing = false;
		const float cutoffHeight = 9999.f;
		int moduleIndex = 0;
		for (auto& mod : moduleList) {
			moduleIndex++;
			if (moduleIndex < ourWindow->yOffset) continue;
			float probableYOffset = (moduleIndex - ourWindow->yOffset) * (textHeight + (textPadding * 2));

			if (ourWindow->isInAnimation) {
				if (probableYOffset > cutoffHeight) {
					overflowing = true;
					break;
				}
			}
			else if ((currentYOffset - ourWindow->pos.y) > cutoffHeight || currentYOffset > g_Data.getGuiData()->heightGame - 5) {
				overflowing = true;
				break;
			}

			std::string textStr = mod->getRawModuleName();

			vec2_t textPos = vec2_t(currentXOffset + textPadding + 45, currentYOffset + textPadding + 5);
			vec4_t rectPos = vec4_t(
				currentXOffset, currentYOffset, xEnd,
				currentYOffset + textHeight + (textPadding * 2));

			bool allowRender = currentYOffset >= categoryHeaderYOffset;

			// Background
			if (allowRender) {
				if (!ourWindow->isInAnimation && !isDragging && rectPos.contains(&mousePos)) {
					DrawUtils::fillRectangle(vec4_t(rectPos.x, rectPos.y, rectPos.z, rectPos.w), mod->isEnabled() ? MC_Color(ColorUtil::interfaceColor(1)) : MC_Color(0, 0, 0), (float)(clickGUI->opacityReal / 255.f));
					DrawUtils::fillRectangle(vec4_t(rectPos.x, rectPos.y, rectPos.z, rectPos.w), MC_Color(255, 255, 255), (float)(clickGUI->opacityReal / 1912.5f));

					DrawUtils::fillRectangle(vec4_t(rectPos.x, rectPos.y - 0.5f, rectPos.z, rectPos.y), MC_Color(0, 0, 0), (float)(clickGUI->opacityReal / 450.f));
					static auto clickGuiMod = moduleMgr->getModule<ClickGUIMod>();
					if (shouldToggleLeftClick && !ourWindow->isInAnimation) {
						if (clickGUI->sounds) {
							auto player = g_Data.getLocalPlayer();
							PointingStruct* level = g_Data.getLocalPlayer()->pointingStruct;
							level->playSound("random.click", *player->getPos(), 1, 1);
						}
						mod->toggle();
						shouldToggleLeftClick = false;
					}
				}
				else {
					DrawUtils::fillRectangle(vec4_t(rectPos.x, rectPos.y, rectPos.z, rectPos.w), mod->isEnabled() ? MC_Color(ColorUtil::interfaceColor(1)) : MC_Color(0, 0, 0), (float)(clickGUI->opacityReal / 255.f));
					DrawUtils::fillRectangle(vec4_t(rectPos.x, rectPos.y - 0.5f, rectPos.z, rectPos.y), MC_Color(0, 0, 0), (float)(clickGUI->opacityReal / 450.f));
				}
			}
			// Text
			float ncdb4 = DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() * 1.f;
			vec2_t textPosFix = vec2_t(currentXOffset + textPadding + 2.f, (rectPos.y + rectPos.w) / 2 - ncdb4 / 2 + 1.5f);
			if (allowRender) {
				DrawUtils::drawText(textPosFix, &textStr, mod->isEnabled() ? MC_Color(255, 255, 255) : MC_Color(155, 155, 155), 1.f, (float)(clickGUI->opacityText / 255.f), true);
				std::string tooltip = mod->getTooltip();
				static auto clickGuiMod = moduleMgr->getModule<ClickGUIMod>();
				if (clickGuiMod->desc.getSelectedValue() != 0 && !tooltip.empty() && rectPos.contains(&mousePos)) renderTooltip(&tooltip);
			}
			// Settings
			{
				std::vector<SettingEntry*>* settings = mod->getSettings();
				if (settings->size() > 2 && allowRender) {
					std::shared_ptr<ClickModule> clickMod = getClickModule(ourWindow, mod->getRawModuleName());
					if (rectPos.contains(&mousePos) && shouldToggleRightClick && !ourWindow->isInAnimation) {
						if (clickGUI->sounds) {
							auto player = g_Data.getLocalPlayer();
							PointingStruct* level = g_Data.getLocalPlayer()->pointingStruct;
							level->playSound("random.click", *player->getPos(), 1, 1);
						}
						shouldToggleRightClick = false;
						clickMod->isExtended = !clickMod->isExtended;
					}

					currentYOffset += textHeight + (textPadding * 2);

					if (clickMod->isExtended) {
						DrawUtils::fillRectangle(vec4_t(rectPos.x, rectPos.w, rectPos.z, rectPos.w + 0.5f), MC_Color(0, 0, 0), (float)(clickGUI->opacityReal / 255.f));
						float startYOffset = currentYOffset - 0.5f;
						for (auto setting : *settings) {
							if (strcmp(setting->name, "enabled") == 0 || strcmp(setting->name, "keybind") == 0) continue;

							if ((currentYOffset - ourWindow->pos.y) > cutoffHeight) {
								overflowing = true;
								break;
							}

							switch (setting->valueType) {
							case ValueType::BOOL_T: {
								currentYOffset += 0.5f;
								vec4_t rectPos = vec4_t(currentXOffset, currentYOffset + 0.5f, xEnd, currentYOffset + textHeight + (textPadding * 2) + 0.5f + 0.5f);
								DrawUtils::fillRectangle(vec4_t(rectPos.x, rectPos.y - 0.5f, rectPos.z, rectPos.y), MC_Color(0, 0, 0), (float)(clickGUI->opacityReal / 255.f));
								if (!setting->value->_bool) {
									DrawUtils::fillRectangle((rectPos), MC_Color(0, 0, 0), (float)(clickGUI->opacityReal / 255.f));
								}
								else {
									DrawUtils::fillRectangle((vec4_t(rectPos.x, rectPos.y, rectPos.x + 3.5f, rectPos.w)), MC_Color(0, 0, 0), (float)(clickGUI->opacityReal / 255.f));
									DrawUtils::fillRectangle((vec4_t(rectPos.x + 3.5f, rectPos.y, rectPos.z - 1.5f, rectPos.w)), MC_Color(ColorUtil::interfaceColor(1)), (float)(clickGUI->opacityReal / 255.f));
									DrawUtils::fillRectangle((vec4_t(rectPos.z - 1.5f, rectPos.y, rectPos.z, rectPos.w)), MC_Color(0, 0, 0), (float)(clickGUI->opacityReal / 255.f));
								}

								float TextHeight = DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() * 1.f;
								vec2_t textPos = vec2_t(currentXOffset + textPadding + 4.f, (rectPos.y + rectPos.w) / 2 - ncdb4 / 2 + 1.5f);
								char name[0x21]; sprintf_s(name, 0x21, "%s", setting->name); if (name[0] != 0) name[0] = toupper(name[0]);
								string elTexto = name;

								// Logic
								bool isFocused = rectPos.contains(&mousePos);
								if (isFocused) {
									DrawUtils::fillRectangle((vec4_t(rectPos.x + 3.5f, rectPos.y, rectPos.z - 1.5f, rectPos.w)), MC_Color(255, 255, 255), (float)(clickGUI->opacityReal / 1912.5f));
								}
								DrawUtils::drawText(textPos, &elTexto, isFocused || setting->value->_bool ? MC_Color(255, 255, 255) : MC_Color(155, 155, 155), 1.f, (float)(clickGUI->opacityText / 255.f), true);
								{
									if (isFocused && shouldToggleLeftClick && !ourWindow->isInAnimation) {
										shouldToggleLeftClick = false;
										setting->value->_bool = !setting->value->_bool;
										if (clickGUI->sounds) {
											auto player = g_Data.getLocalPlayer();
											PointingStruct* level = g_Data.getLocalPlayer()->pointingStruct;
											level->playSound("random.click", *player->getPos(), 1, 1);
										}
									}
								}
								currentYOffset += textHeight + (textPadding * 2) + 0.5f;
								std::string tooltip2 = setting->tooltip2;
								if (!tooltip2.empty() && isFocused && clickGUI->desc.getSelectedValue() != 0)
								{
									renderTooltip(&tooltip2);
								}
								break;
							}
							case ValueType::ENUM_T: {
								currentYOffset += 0.5f;
								vec4_t rectPos = vec4_t(currentXOffset, currentYOffset + 0.5f, xEnd, currentYOffset + textHeight + (textPadding * 2) + 0.5f + 0.5f);
								DrawUtils::fillRectangle(vec4_t(rectPos.x, rectPos.y - 0.5f, rectPos.z, rectPos.y), MC_Color(0, 0, 0), (float)(clickGUI->opacityReal / 255.f));
								DrawUtils::fillRectangle(rectPos, MC_Color(0, 0, 0), (float)(clickGUI->opacityReal / 255.f));

								char name[0x22]; sprintf_s(name, +"%s:", setting->name); if (name[0] != 0) name[0] = toupper(name[0]);

								EnumEntry& i = ((SettingEnum*)setting->extraData)->GetSelectedEntry();

								char name2[0x21]; sprintf_s(name2, 0x21, " %s", i.GetName().c_str()); if (name2[0] != 0) name2[0] = toupper(name2[0]);
								string elTexto2 = string(GRAY) + name2;

								string elTexto = name;// + string(GRAY) + elTexto2;

								float TextHeight = DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() * 1.f;
								vec2_t textPos = vec2_t(currentXOffset + textPadding + 4.f, (rectPos.y + rectPos.w) / 2 - ncdb4 / 2 + 1.5f);
								DrawUtils::drawText(textPos, &elTexto, MC_Color(255, 255, 255), 1.f, (float)(clickGUI->opacityText / 255.f), true);
								DrawUtils::drawText(vec2_t(rectPos.z - 1.5f - DrawUtils::getTextWidth(&elTexto2, 1.f), textPos.y), &elTexto2, MC_Color(255, 255, 255), 1.f, (float)(clickGUI->opacityText / 255.f), true);

								// logic
								if (rectPos.contains(&mousePos)) {
									DrawUtils::fillRectangle((vec4_t(rectPos.x + 3.5f, rectPos.y, rectPos.z - 1.5f, rectPos.w)), MC_Color(255, 255, 255), (float)(clickGUI->opacityReal / 1912.5f));
								}
								if (!ourWindow->isInAnimation && rectPos.contains(&mousePos)) {
									if (shouldToggleLeftClick) {
										shouldToggleLeftClick = false;
										((SettingEnum*)setting->extraData)->SelectNextValue(false);
										if (clickGUI->sounds) {
											auto player = g_Data.getLocalPlayer();
											PointingStruct* level = g_Data.getLocalPlayer()->pointingStruct;
											level->playSound("random.click", *player->getPos(), 1, 1);
										}
									}
									else if (shouldToggleRightClick) {
										shouldToggleRightClick = false;
										((SettingEnum*)setting->extraData)->SelectNextValue(true);
										if (clickGUI->sounds) {
											auto player = g_Data.getLocalPlayer();
											PointingStruct* level = g_Data.getLocalPlayer()->pointingStruct;
											level->playSound("random.click", *player->getPos(), 1, 1);
										}
									}
								}
								std::string tooltip2 = setting->tooltip2;
								currentYOffset += textHeight + (textPadding * 2) + 0.5f;
								if (!tooltip2.empty() && rectPos.contains(&mousePos) && clickGUI->desc.getSelectedValue() != 0)
								{
									renderTooltip(&tooltip2);
								}
								break;
							}
							case ValueType::FLOAT_T: {
								currentYOffset += 0.5f;
								vec4_t rectPos = vec4_t(currentXOffset, currentYOffset + 0.5f, xEnd, currentYOffset + textHeight + (textPadding * 2) + 0.5f + 0.5f);
								DrawUtils::fillRectangle(vec4_t(rectPos.x, rectPos.y - 0.5f, rectPos.z, rectPos.y), MC_Color(0, 0, 0), (float)(clickGUI->opacityReal / 255.f));
								float TextHeight = DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() * 1.f;
								vec2_t textPos = vec2_t(currentXOffset + textPadding + 4.f, (rectPos.y + rectPos.w) / 2 - ncdb4 / 2 + 1.5f);
								char str[10]; sprintf_s(str, 10, "%.1f", setting->value->_float);
								string text = str;
								char name[0x22]; sprintf_s(name, "%s: ", setting->name); if (name[0] != 0) name[0] = toupper(name[0]);
								string elTexto = name;// + text;
								DrawUtils::drawText(textPos, &elTexto, MC_Color(255, 255, 255), 1.f, (float)(clickGUI->opacityText / 255.f), true);
								DrawUtils::drawText(vec2_t(rectPos.z - 1.5f - DrawUtils::getTextWidth(&text, 1.f), textPos.y), &text, MC_Color(255, 255, 255), 1.f, (float)(clickGUI->opacityText / 255.f), true);

								DrawUtils::fillRectangle(vec4_t(rectPos.x, rectPos.y, rectPos.x + 3.5f, rectPos.w), MC_Color(0, 0, 0), (float)(clickGUI->opacityReal / 255.f)); // Code dung
								DrawUtils::fillRectangle(vec4_t(rectPos.z - 1.5f, rectPos.y, rectPos.z, rectPos.w), MC_Color(0, 0, 0), (float)(clickGUI->opacityReal / 255.f)); // Code dung

								vec4_t rect = vec4_t(rectPos.x + 3.5f, rectPos.y, rectPos.z - 1.5f, rectPos.w);

								const float minValue = setting->minValue->_float;
								const float maxValue = setting->maxValue->_float - minValue;

								float value = (float)fmax(0, setting->value->_float - minValue);
								if (value > maxValue) value = maxValue; value /= maxValue;

								const float endXlol = rectPos.z - 1.5f - rectPos.x - 3.5f;
								value *= endXlol;

								const bool areWeFocused = rectPos.contains(&mousePos);
								{
									rect.z = rect.x + value;
									DrawUtils::fillRectangle(rect, MC_Color(ColorUtil::interfaceColor(1)), (float)(clickGUI->opacityReal / 255.f));
									DrawUtils::fillRectangle(vec4_t(rect.z, rectPos.y, rectPos.z - 1.5f, rectPos.w), MC_Color(0, 0, 0), (float)(clickGUI->opacityReal / 255.f));
								}
								// Drag Logic
								{
									if (setting->isDragging) {
										if (isLeftClickDown && !isRightClickDown) {
											value = mousePos.x - rect.x;
											DrawUtils::fillRectangle(rect, MC_Color(0, 0, 0), (float)(clickGUI->opacityReal / 1912.5f));
										}
										else
											setting->isDragging = false;
									}
									else if (areWeFocused && shouldToggleLeftClick && !ourWindow->isInAnimation) {
										shouldToggleLeftClick = false;
										setting->isDragging = true;
										if (clickGUI->sounds) {
											auto player = g_Data.getLocalPlayer();
											PointingStruct* level = g_Data.getLocalPlayer()->pointingStruct;
											level->playSound("random.click", *player->getPos(), 1, 1);
										}
									}
									if (areWeFocused && !isLeftClickDown && !ourWindow->isInAnimation) {
										DrawUtils::fillRectangle(rect, MC_Color(255, 255, 255), (float)(clickGUI->opacityReal / 1912.5f));
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
								std::string tooltip2 = setting->tooltip2;

								currentYOffset += textHeight + (textPadding * 2) + 0.5f;
								if (!tooltip2.empty() && rectPos.contains(&mousePos) && clickGUI->desc.getSelectedValue() != 0)
								{
									renderTooltip(&tooltip2);
								}
								break;
							}
							case ValueType::INT_T: {
								currentYOffset += 0.5f;
								vec4_t rectPos = vec4_t(currentXOffset, currentYOffset + 0.5f, xEnd, currentYOffset + textHeight + (textPadding * 2) + 0.5f + 0.5f);
								DrawUtils::fillRectangle(vec4_t(rectPos.x, rectPos.y - 0.5f, rectPos.z, rectPos.y), MC_Color(0, 0, 0), (float)(clickGUI->opacityReal / 255.f));
								float TextHeight = DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() * 1.f;
								vec2_t textPos = vec2_t(currentXOffset + textPadding + 4.f, (rectPos.y + rectPos.w) / 2 - ncdb4 / 2 + 1.5f);

								char name[0x22]; sprintf_s(name, "%s: ", setting->name); if (name[0] != 0) name[0] = toupper(name[0]);
								char str[10]; sprintf_s(str, 10, "%i", setting->value->_int);
								string text = str;
								string elTexto = name;// +text;

								DrawUtils::drawText(textPos, &elTexto, MC_Color(255, 255, 255), 1.f, (float)(clickGUI->opacityText / 255.f), true);
								DrawUtils::drawText(vec2_t(rectPos.z - 1.5f - DrawUtils::getTextWidth(&text, 1.f), textPos.y), &text, MC_Color(255, 255, 255), 1.f, (float)(clickGUI->opacityText / 255.f), true);

								DrawUtils::fillRectangle(vec4_t(rectPos.x, rectPos.y, rectPos.x + 3.5f, rectPos.w), MC_Color(0, 0, 0), (float)(clickGUI->opacityReal / 255.f)); // Code dung
								DrawUtils::fillRectangle(vec4_t(rectPos.z - 1.5f, rectPos.y, rectPos.z, rectPos.w), MC_Color(0, 0, 0), (float)(clickGUI->opacityReal / 255.f)); // Code dung

								currentYOffset += textHeight + (textPadding * 2) + 0.5f;

								const bool areWeFocused = rectPos.contains(&mousePos);
								vec4_t rect = vec4_t(rectPos.x + 3.5f, rectPos.y, rectPos.z - 1.5f, rectPos.w);
								std::string tooltip2 = setting->tooltip2;
								const float minValue = (float)setting->minValue->_int;
								const float maxValue = (float)setting->maxValue->_int - minValue;
								float value = (float)fmax(0, setting->value->_int - minValue);  // Value is now always > 0 && < maxValue
								if (value > maxValue) value = maxValue;
								value /= maxValue;  // Value is now in range 0 - 1
								const float endXlol = rectPos.z - 1.5f - rectPos.x - 3.5f;
								value *= endXlol;  // Value is now pixel diff between start of bar and end of progress

								{
									rect.z = rect.x + value;
									DrawUtils::fillRectangle(rect, MC_Color(ColorUtil::interfaceColor(1)), (float)(clickGUI->opacityReal / 255.f));
									DrawUtils::fillRectangle(vec4_t(rect.z, rectPos.y, rectPos.z - 1.5f, rectPos.w), MC_Color(0, 0, 0), (float)(clickGUI->opacityReal / 255.f));
								}

								// Drag Logic
								{
									if (setting->isDragging) {
										if (isLeftClickDown && !isRightClickDown) {
											value = mousePos.x - rect.x;
											DrawUtils::fillRectangle(rect, MC_Color(0, 0, 0), (float)(clickGUI->opacityReal / 1912.5f));
										}
										else
											setting->isDragging = false;
									}
									else if (areWeFocused && shouldToggleLeftClick && !ourWindow->isInAnimation) {
										shouldToggleLeftClick = false;
										setting->isDragging = true;
										if (clickGUI->sounds) {
											auto player = g_Data.getLocalPlayer();
											PointingStruct* level = g_Data.getLocalPlayer()->pointingStruct;
											level->playSound("random.click", *player->getPos(), 1, 1);
										}
									}
									if (areWeFocused && !isLeftClickDown && !ourWindow->isInAnimation) {
										DrawUtils::fillRectangle(rect, MC_Color(255, 255, 255), (float)(clickGUI->opacityReal / 1912.5f));
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
								if (!tooltip2.empty() && rectPos.contains(&mousePos) && clickGUI->desc.getSelectedValue() != 0)
								{
									renderTooltip(&tooltip2);
								}

								break;

							}
							case ValueType::KEYBIND_T:
							{
								currentYOffset += 0.5f;
								vec4_t rectPos = vec4_t(currentXOffset, currentYOffset + 0.5f, xEnd, currentYOffset + textHeight + (textPadding * 2) + 0.5f + 0.5f);
								vec2_t textPos = vec2_t(currentXOffset + textPadding + 4.f, (rectPos.y + rectPos.w) / 2 - ncdb4 / 2 + 1.5f);
								float TextHeight = DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() * 1.f;
								currentYOffset += textHeight + (textPadding * 2) + 0.5f;
								if (!isCapturingKey || (keybindMenuCurrent != setting && isCapturingKey))
								{
									char name[0x21];
									sprintf_s(name, 0x21, "%s:", setting->name);
									if (name[0] != 0)
										name[0] = toupper(name[0]);

									std::string text = name;

									DrawUtils::drawText(textPos, &text, MC_Color(255, 255, 255), textSize, (float)(clickGUI->opacityText / 255.f), true);

									const char* key;

									if (setting->value->_int > 0 && setting->value->_int < 190)
										key = KeyNames[setting->value->_int];
									else if (setting->value->_int == 0x0)
										key = "NONE";
									else
										key = "UNKNOWN";

									if (keybindMenuCurrent == setting && isCapturingKey)
									{
										key = "...";
									}
									else if (keybindMenuCurrent == setting && isConfirmingKey)
									{
										if (newKeybind > 0 && newKeybind < 190)
											key = KeyNames[newKeybind];
										else if (newKeybind == 0x0)
											key = "N/A";
										else
											key = "UNKNOWN";
									}

									std::string keyb = key;
									float keybSz = textHeight * 0.8f;

									float length = 10.f; // because we add 5 to text padding + keybind name
									length += DrawUtils::getTextWidth(&text, textSize);
									length += DrawUtils::getTextWidth(&keyb, textSize);

									DrawUtils::drawText(textPos, &text, MC_Color(255, 255, 255), textSize, (float)(clickGUI->opacityText / 255.f), true);

									vec2_t textPos2(rectPos.z - 5.f, textPos.y);
									textPos2.x -= DrawUtils::getTextWidth(&keyb, textSize);

									DrawUtils::drawText(textPos2, &keyb, MC_Color(180, 180, 180), textSize), 1.f, true;
								}
								else
								{
									std::string text = "Key:     ";
									std::string text2 = "Recording...";
									std::string textFull = string(WHITE) + text + string(GRAY) + text2;
									DrawUtils::drawText(textPos, &textFull, MC_Color(255, 255, 255), textSize, (float)(clickGUI->opacityText / 255.f), true);
								}

								DrawUtils::fillRectangle(rectPos, MC_Color(0, 0, 0), (float)(clickGUI->opacityReal / 255.f));
								// Logic
								{
									bool isFocused = rectPos.contains(&mousePos);

									if (isFocused && shouldToggleLeftClick && !(isCapturingKey && keybindMenuCurrent != setting /*don't let the user click other stuff while changing a keybind*/))
									{
										keybindMenuCurrent = setting;
										isCapturingKey = true;
									}

									if (isFocused && shouldToggleRightClick && !(isCapturingKey && keybindMenuCurrent != setting))
									{
										setting->value->_int = 0x0; // Clear

										isCapturingKey = false;
									}

									if (shouldStopCapturing && keybindMenuCurrent == setting)
									{ // The user has selected a key
										shouldStopCapturing = false;
										isCapturingKey = false;
										setting->value->_int = newKeybind;
									}
								}
							}
							break;
							}
						}

						float endYOffset = currentYOffset + 1.5f;

						if (endYOffset - startYOffset > textHeight + 1 || overflowing) {
							startYOffset += textPadding;
							endYOffset -= textPadding;
							auto itfcl = ColorUtil::interfaceColor(1);
							DrawUtils::setColor(itfcl.r, itfcl.g, itfcl.b, (float)(clickGUI->opacityReal / 255.f));
							DrawUtils::drawLine(vec2_t(currentXOffset + 1.5, startYOffset), vec2_t(currentXOffset + 1.5, endYOffset), 1.f);
						}
						currentYOffset += 0.5f;

					}
					currentYOffset += 0.5f;
				}
				else currentYOffset += textHeight + (textPadding * 2);
			}
		}

		vec4_t winRectPos = vec4_t(xOffset, yOffset, xEnd, currentYOffset);
	}
	DrawUtils::flush();
	// Draw Category Header
	{
		vec2_t textPos = vec2_t(currentXOffset + textPadding, categoryHeaderYOffset + textPadding);
		vec4_t rectPos = vec4_t(
			currentXOffset - categoryMargin, categoryHeaderYOffset - categoryMargin,
			currentXOffset + windowSize->x + paddingRight + categoryMargin,
			categoryHeaderYOffset + textHeight + (textPadding * 2));
		vec4_t rectTest = vec4_t(rectPos.x, rectPos.y + 1, rectPos.z, rectPos.w);
		vec4_t rectTest2 = vec4_t(rectPos.x + 1.f, rectPos.y + 2, rectPos.z - 1.f, rectPos.w);

		for (auto& mod : moduleList) {
			rectTest.w = currentYOffset - 1;
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

		// Draw category header
		{
			string textStr = categoryName;
			DrawUtils::drawText(vec2_t(textPos.x + 0.5f, textPos.y), &textStr, MC_Color(255, 255, 255), textSize, (float)(clickGUI->opacityText / 255.f), true);
			DrawUtils::fillRectangle(vec4_t(rectPos.x, rectPos.y - 1.5f, rectPos.z, rectPos.w), MC_Color(ColorUtil::interfaceColor(1)), (float)(clickGUI->opacityReal / 255.f));
		}
	}

	// anti idiot
	{
		vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	}

	moduleList.clear();
	DrawUtils::flush();
}

void ClickGui::render()
{
	if (!moduleMgr->isInitialized())
		return;

	if (categoryOrder.empty())
	{
		categoryOrder.push_back(Category::COMBAT);
		categoryOrder.push_back(Category::VISUAL);
		categoryOrder.push_back(Category::MOVEMENT);
		categoryOrder.push_back(Category::PLAYER);
		categoryOrder.push_back(Category::EXPLOIT);
		categoryOrder.push_back(Category::OTHER);
	}

	if (!nextOrder.empty())
		categoryOrder = nextOrder;

	nextOrder = categoryOrder;

	if (timesRendered < 10)
		timesRendered++;

	// Fill Background
	auto clickGuiMod = moduleMgr->getModule<ClickGUIMod>();
	auto tint = clickGuiMod->tint;
	auto tintopacity = clickGuiMod->tintopacity / 255;
	auto red = clickGuiMod->red;
	auto green = clickGuiMod->green;
	auto blue = clickGuiMod->blue;
	auto interfacrColor = ColorUtil::interfaceColor(1);
	switch (tint.getSelectedValue())
	{
	case 0: // None
		DrawUtils::fillRectangle(vec4_t(0, 0, g_Data.getClientInstance()->getGuiData()->widthGame, g_Data.getClientInstance()->getGuiData()->heightGame),
								 MC_Color(0, 0, 0), 0.f);
		break;
	case 1: // Custom
		DrawUtils::fillRectangle(vec4_t(0, 0, g_Data.getClientInstance()->getGuiData()->widthGame, g_Data.getClientInstance()->getGuiData()->heightGame),
								 MC_Color(red, green, blue), tintopacity);
		break;
	case 2: // Interface
		DrawUtils::fillRectangle(vec4_t(0, 0, g_Data.getClientInstance()->getGuiData()->widthGame, g_Data.getClientInstance()->getGuiData()->heightGame),
								 MC_Color(interfacrColor), tintopacity);
		break;
	}

	// Render all categorys
	for (auto window : categoryOrder)
	{
		renderCategory(window);
	}

	shouldToggleLeftClick = false;
	shouldToggleRightClick = false;
	resetStartPos = false;

	DrawUtils::flush();
}

void ClickGui::imguirender(ImDrawList* d) {
}

void ClickGui::init() { initialised = true; }

void ClickGui::onMouseClickUpdate(int key, bool isDown)
{
	if (!moduleMgr->getModule<ClickGUIMod>()->isEnabled())
		return;

	switch (key)
	{
	case 1: // Left Click
		isLeftClickDown = isDown;
		if (isDown)
			shouldToggleLeftClick = true;
		break;
	case 2: // Right Click
		isRightClickDown = isDown;
		if (isDown)
			shouldToggleRightClick = true;
		break;
	}
}

void ClickGui::onWheelScroll(bool direction)
{
	if (!direction)
		scrollingDirection++;
	else
		scrollingDirection--;
}

void ClickGui::onKeyUpdate(int key, bool isDown)
{
	if (!initialised)
		return;
	static auto clickGuiMod = moduleMgr->getModule<ClickGUIMod>();

	if (!isDown)
		return;

	if (!clickGuiMod->isEnabled())
	{
		timesRendered = 0;
		return;
	}

	if (isCapturingKey && !shouldStopCapturing)
	{
		newKeybind = key;
		shouldStopCapturing = true;
	}

	if (timesRendered < 10)
		return;
	timesRendered = 0;

	switch (key)
	{
	case VK_ESCAPE:
		clickGuiMod->setEnabled(false);
		isCapturingKey = false;
		isConfirmingKey = false;
		keybindMenuCurrent = nullptr;
		g_Data.getClientInstance()->grabMouse();
		return;
	default:
		if (key == clickGuiMod->getKeybind())
		{
			clickGuiMod->setEnabled(false);
			isCapturingKey = false;
			isConfirmingKey = false;
			keybindMenuCurrent = nullptr;
			g_Data.getClientInstance()->grabMouse();
		}
	}
}

using json = nlohmann::json;
void ClickGui::onLoadConfig(void *confVoid)
{
	savedWindowSettings.clear();
	windowMap.clear();
	json *conf = reinterpret_cast<json *>(confVoid);
	if (conf->contains("ClickGuiMenu"))
	{
		auto obj = conf->at("ClickGuiMenu");
		if (obj.is_null())
			return;
		for (int i = 0; i <= (int)Category::CUSTOM /*last category*/; i++)
		{
			auto catName = ClickGui::catToName((Category)i);
			if (obj.contains(catName))
			{
				auto value = obj.at(catName);
				if (value.is_null())
					continue;
				try
				{
					SavedWindowSettings windowSettings = {};
					windowSettings.name = catName;
					if (value.contains("pos"))
					{
						auto posVal = value.at("pos");
						if (!posVal.is_null() && posVal.contains("x") && posVal["x"].is_number_float() && posVal.contains("y") && posVal["y"].is_number_float())
						{
							try
							{
								windowSettings.pos = {posVal["x"].get<float>(), posVal["y"].get<float>()};
							}
							catch (std::exception e)
							{
							}
						}
					}
					if (value.contains("isExtended"))
					{
						auto isExtVal = value.at("isExtended");
						if (!isExtVal.is_null() && isExtVal.is_boolean())
						{
							try
							{
								windowSettings.isExtended = isExtVal.get<bool>();
							}
							catch (std::exception e)
							{
							}
						}
					}
					savedWindowSettings[Utils::getCrcHash(catName)] = windowSettings;
				}
				catch (std::exception e)
				{
					logF("Config Load Error (ClickGuiMenu): %s", e.what());
				}
			}
		}
	}
}
void ClickGui::onSaveConfig(void *confVoid)
{
	json *conf = reinterpret_cast<json *>(confVoid);
	// First update our map
	for (const auto &wind : windowMap)
	{
		savedWindowSettings[wind.first] = {wind.second->pos, wind.second->isExtended, wind.second->name};
	}

	// Save to json
	if (conf->contains("ClickGuiMenu"))
		conf->erase("ClickGuiMenu");

	json obj = {};

	for (const auto &wind : savedWindowSettings)
	{
		json subObj = {};
		subObj["pos"]["x"] = wind.second.pos.x;
		subObj["pos"]["y"] = wind.second.pos.y;
		subObj["isExtended"] = wind.second.isExtended;
		obj[wind.second.name] = subObj;
	}

	conf->emplace("ClickGuiMenu", obj);
}