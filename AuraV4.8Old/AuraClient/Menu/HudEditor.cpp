#include "HudEditor.h"
#include <Windows.h>

//bool findBlocks(C_ItemStack* itemStack);
static bool isLeftClickDown1 = false;
static bool isRightClickDown1 = false;
static bool shouldToggleLeftClick1 = false;  // If true, toggle the focused module
static bool shouldToggleRightClick1 = false;
static bool arrayListIsDragging = false;
static bool targetHUDIsDragging = false;
static bool armHudIsDragging = false;
static float defaultX = 0;
static float defaultY = 0;

//unsigned int focusedElement = -1;
//unsigned int draggedWindow = -1;
static vec2_t dragStartHudEditor = vec2_t();
//bool isFocused = false;

static bool hasInitializedHE = false;
static int timesRenderedHE = 0;

using namespace std;

void HudEditor::render() {
	auto player = g_Data.getLocalPlayer();
	bool rightClickDown = g_Data.isRightClickDown;
	bool leftClickDown = g_Data.isLeftClickDown();
	auto interfaceMod = moduleMgr->getModule<Interface>();
	static auto arrayList = moduleMgr->getModule<ArrayList>();

	static auto targetHUD = moduleMgr->getModule<TargetHUD>();
	static auto watermark = moduleMgr->getModule<Watermark>();
	static auto scaffold = moduleMgr->getModule<Scaffold>();
	static auto clickGUI = moduleMgr->getModule<ClickGUIMod>();

	vec3_t* currPos = g_Data.getLocalPlayer()->getPos();
	vec2_t windowSizeReal = g_Data.getClientInstance()->getGuiData()->windowSizeReal;
	vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	vec2_t mousePos = *g_Data.getClientInstance()->getMousePos();
	// Convert mousePos to visual Pos
	{
		vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
		vec2_t windowSizeReal = g_Data.getClientInstance()->getGuiData()->windowSizeReal;

		mousePos = mousePos.div(windowSizeReal);
		mousePos = mousePos.mul(windowSize);
	}
	static const float rectHeight2 = (5, 5) * DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();

	auto hudColor = ColorUtil::rainbowColor(8, 1.F, 1.F, 1);
	if (interfaceMod->color.getSelectedValue() == 0) hudColor = ColorUtil::rainbowColor(5, interfaceMod->saturation, 1, 1);
	if (interfaceMod->color.getSelectedValue() == 1) hudColor = ColorUtil::astolfoRainbow(5, 1000); //Astolfo

	string position = "Position: " + to_string((int)floorf(currPos->x)) + " " + to_string((int)floorf(currPos->y)) + " " + to_string((int)floorf(currPos->z));
	string speedText = "Speed: " + to_string((int)player->getBlocksPerSecond()) + string(".") + to_string((int)(player->getBlocksPerSecond() * 10) - ((int)player->getBlocksPerSecond() * 10));
	string fpsText = "FPS: " + to_string(g_Data.getFPS());
	string totalCountAids = "64";

	string strLength = "V2.11 - Developer Build   ";
	float lFPS = DrawUtils::getTextWidth(&fpsText, 1) + 6.5;
	float lPos = DrawUtils::getTextWidth(&position, 1) + 6.5;
	float lSpeed = DrawUtils::getTextWidth(&speedText, 1) + 6.5;
	float infoTextLength = DrawUtils::getTextWidth(&strLength) + DrawUtils::getTextWidth(&string(""), 0.7) + 2;
	static const float height = (1 + 0.7 * 1) * DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
	constexpr float borderPadding = 1;
	float margin = 2.f;


	vec4_t testRect = vec4_t(targetHUD->positionX, targetHUD->positionY, targetHUD->targetLen + targetHUD->positionX, targetHUD->positionY + rectHeight2);
	vec4_t rectPos2 = vec4_t(arrayList->positionX - 50, arrayList->positionY, arrayList->positionX, arrayList->positionY + 10);
	if (arrayList->invert) rectPos2 = vec4_t(arrayList->positionX - 50, arrayList->positionY - 10, arrayList->positionX, arrayList->positionY);

	vec4_t clickGUIRect = vec4_t(windowSize.x / 2 - 35, windowSize.y / 2 - 8, windowSize.x / 2 + 35, windowSize.y / 2 + 8);
	if (clickGUIRect.contains(&mousePos)) {
		DrawUtils::fillRoundRectangle(clickGUIRect, MC_Color(255, 2555, 255, 100), true);
		if (clickGUIRect.contains(&mousePos) && leftClickDown) {
			clickGUI->hasOpenedGUI = true;
			shouldToggleLeftClick1 = false;
		}
	}
	auto interfaceColor = ColorUtil::interfaceColor(1);
	//DrawUtils::drawText(vec2_t(clickGUIRect.x + 17, clickGUIRect.y + 4), &string("Modules"), MC_Color(255, 255, 255), 1.F, 1.F, true);
	//DrawUtils::drawCenteredString(vec2_t(windowSize.x / 2, windowSize.y / 2 - 20), &string("Packet Client"), 1.5f, MC_Color(255, 255, 255), true);
	DrawUtils::drawText(vec2_t(clickGUIRect.x + 17, clickGUIRect.y + 4), &string("Modules"), MC_Color(255, 255, 255), 1.F, 1.F, true);
	DrawUtils::drawCenteredString(vec2_t(windowSize.x / 2, windowSize.y / 2 - 20), &string("Aura - V4.8 Beta"), 1.5f, MC_Color(interfaceColor), true);
	DrawUtils::fillRoundRectangle(clickGUIRect, MC_Color(0, 0, 0, 100), false);
	DrawUtils::drawRoundRectangle2(clickGUIRect, MC_Color(interfaceColor));
	
	const bool targetHUDIsFocused = testRect.contains(&mousePos);
	
	// ?? 
	/*C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	for (int s = 0; s < 9; s++) {
		C_ItemStack* stack = inv->getItemStack(s);
		if (stack->item != nullptr && stack->getItem()->isBlock() && findBlocks(stack)) {
			if (stack->isValid()) DrawUtils::drawItem(stack, vec2_t(blockPos.x - 4, blockPos.y - 7), 1, 1, false);
		}
	}*/ 

	if (targetHUD->isEnabled()) DrawUtils::drawRectangle(testRect, MC_Color(255, 255, 255), 1.f);
	if (targetHUD->isEnabled() && targetHUDIsFocused) DrawUtils::fillRoundRectangle(testRect, MC_Color(255, 255, 255, 255), false);

	//keybd_event(0x41, 0, KEYEVENTF_EXTENDEDKEY, 0);
	vec4_t t = vec4_t(20, 20, 200, 30);
	vec2_t stringPos = vec2_t(t.x, t.y);
	const bool tesdr = t.contains(&mousePos);
	string aids;
	//if (GetAsyncKeyState('F')) {
		//aids = aids += "a";
	//}

		//keybd_event(0x41, 0, KEYEVENTF_EXTENDEDKEY, 0);
	
	
	//DrawUtils::drawText(stringPos, &aids, MC_Color(255, 255, 255), 1.f, 1.f, true);
	//DrawUtils::fillRectangleA(t, MC_Color(0, 0, 0, 100));

	if (arrayListIsDragging) {
		if (leftClickDown) {
			vec2_t diff = vec2_t(mousePos).sub(dragStartHudEditor);
			arrayList->positionY = arrayList->positionY + diff.y;
			arrayList->positionX = arrayList->positionX + diff.x;
			dragStartHudEditor = mousePos;
			float cctest = DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() * 1.f;
			// anti idiot
			{
				if (arrayList->positionX > windowSize.x)
					arrayList->positionX = windowSize.x;

				if (arrayList->positionY + cctest > windowSize.y)
					arrayList->positionY = windowSize.y - cctest;

				arrayList->positionX = (float)fmax(0, arrayList->positionX);
				arrayList->positionY = (float)fmax(0, arrayList->positionY);
			}
		}
		else {
			arrayListIsDragging = false;
		}
	}
	else if (arrayList->focused && leftClickDown) {
		arrayListIsDragging = true;
		dragStartHudEditor = mousePos;
	}

	if (targetHUDIsDragging) {
		if (leftClickDown) {
			vec2_t diff = vec2_t(mousePos).sub(dragStartHudEditor);
			targetHUD->positionY = targetHUD->positionY + diff.y;
			targetHUD->positionX = targetHUD->positionX + diff.x;
			dragStartHudEditor = mousePos;
			// anti idiot
			{
				if (targetHUD->positionX + 120 > windowSize.x)
					targetHUD->positionX = windowSize.x - 120;

				if (targetHUD->positionY + 50 > windowSize.y)
					targetHUD->positionY = windowSize.y - 50;

				targetHUD->positionX = (float)fmax(0, targetHUD->positionX);
				targetHUD->positionY = (float)fmax(0, targetHUD->positionY);
			}
		}
		else {
			targetHUDIsDragging = false;
		}
	}
	else if (targetHUDIsFocused && leftClickDown) {
		dragStartHudEditor = mousePos;
		targetHUDIsDragging = true;
	}

	shouldToggleLeftClick1 = false;
	shouldToggleRightClick1 = false;

	DrawUtils::flush();
}

void HudEditor::init() { hasInitializedHE = true; }

void HudEditor::onKeyUpdate(int key, bool isDown) {
	if (!hasInitializedHE)
		return;
	static auto clickGUI = moduleMgr->getModule<ClickGUIMod>();

	if (!isDown)
		return;

	if (!clickGUI->isEnabled()) {
		timesRenderedHE = 0;
		return;
	}

	if (timesRenderedHE < 10)
		return;
	timesRenderedHE = 0;
}

void HudEditor::onMouseClickUpdate(int key, bool isDown) {
	static auto clickGUI = moduleMgr->getModule<ClickGUIMod>();
	if (clickGUI->isEnabled() && g_Data.isInGame()) {
		switch (key) {
		case 1:  // Left Click
			isLeftClickDown1 = isDown;
			if (isDown)
				shouldToggleLeftClick1 = true;
			break;
		case 2:  // Right Click
			isRightClickDown1 = isDown;
			if (isDown)
				shouldToggleRightClick1 = true;
			break;
		}
	}
}

using json = nlohmann::json;
void HudEditor::onLoadSettings(void* confVoid) {
	auto interfaceMod = moduleMgr->getModule<Interface>();
	auto arrayList = moduleMgr->getModule<ArrayList>();
	auto targetHUD = moduleMgr->getModule<TargetHUD>();
	auto watermark = moduleMgr->getModule<Watermark>();
	auto scaffold = moduleMgr->getModule<Scaffold>();

	json* conf = reinterpret_cast<json*>(confVoid);
	if (conf->contains("HudEditorMenu")) {
		auto obj = conf->at("HudEditorMenu");
		if (obj.is_null())
			return;
		auto Position = "pos";
		const char* categories[12] = { "Speed","FPS","Position","Watermark","TargetHud","BlockCount","Release","ArrayList","Config","ArmorHud","SessionInfo","PlayerList" };

		//TargetHud
		if (obj.contains(categories[4])) {
			auto SpeedVal = obj.at(categories[4]);
			if (!SpeedVal.is_null() && SpeedVal.contains(Position)) {
				auto posVal = SpeedVal.at(Position);
				if (!posVal.is_null() && posVal.contains("x") && posVal["x"].is_number_float() && posVal.contains("y") && posVal["y"].is_number_float()) {
					targetHUD->positionX = { posVal["x"].get<float>() };
					targetHUD->positionY = { posVal["y"].get<float>() };
				}
			}
		}
		if (obj.contains(categories[7])) {
			auto SpeedVal = obj.at(categories[7]);
			if (!SpeedVal.is_null() && SpeedVal.contains(Position)) {
				auto posVal = SpeedVal.at(Position);
				if (!posVal.is_null() && posVal.contains("x") && posVal["x"].is_number_float() && posVal.contains("y") && posVal["y"].is_number_float()) {
					arrayList->positionX = { posVal["x"].get<float>() };
					arrayList->positionY = { posVal["y"].get<float>() };
				}
			}
		}
	}
}

void HudEditor::onSaveSettings(void* confVoid) {
	json* conf = reinterpret_cast<json*>(confVoid);
	auto interfaceMod = moduleMgr->getModule<Interface>();
	auto arrayList = moduleMgr->getModule<ArrayList>();
	auto targetHUD = moduleMgr->getModule<TargetHUD>();
	auto watermark = moduleMgr->getModule<Watermark>();
	auto scaffold = moduleMgr->getModule<Scaffold>();
	// Save to json
	if (conf->contains("HudEditorMenu"))
		conf->erase("HudEditorMenu");

	json obj = {};
	// AidsList
	json arrayListObj = {};
	arrayListObj["pos"]["x"] = arrayList->positionX;
	arrayListObj["pos"]["y"] = arrayList->positionY;
	obj["ArrayList"] = arrayListObj;
	//TargetHud
	json TargetHudObj = {};
	TargetHudObj["pos"]["x"] = targetHUD->positionX;
	TargetHudObj["pos"]["y"] = targetHUD->positionY;
	obj["TargetHud"] = TargetHudObj;

	conf->emplace("HudEditorMenu", obj);
}