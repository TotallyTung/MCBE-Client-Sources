#include "ArrayListNuke.h"
using namespace std;
ArrayListNuke::ArrayListNuke() : IModule(0, Category::VISUAL, "Displays enabled modules") {
	registerEnumSetting("Mode", &mode, 0);
	mode.addEntry("Outline", 0);
	mode.addEntry("Full", 1);
	mode.addEntry("Split", 2);
	mode.addEntry("Bar", 3);
	mode.addEntry("Right", 4);
	mode.addEntry("None", 5);
	registerEnumSetting("Animation", &animation, 1);
	animation.addEntry("Normal", 0);
	registerBoolSetting("Bottom", &bottom, bottom);
	registerBoolSetting("Modes", &modes, modes);
	registerBoolSetting("Show keybind", &showkb, showkb);
	registerIntSetting("Opacity", &opacity, opacity, 0, 255);
	registerFloatSetting("Scale", &scale, scale, 0.5f, 1.5f);
	registerFloatSetting("Spacing", &spacing, spacing, 0.f, 1.f);
}

const char* ArrayListNuke::getModuleName() {
	return ("ArrayListNuke1");
}

#pragma region IModuleContainer
struct IModuleContainer {
	shared_ptr<IModule> backingModule;
	bool shouldRender = true;
	int keybind;
	string moduleName;
	float textWidth;
	bool enabled;
	Vec2* pos;

	IModuleContainer(shared_ptr<IModule> mod) {
		const char* name = "Module";
		this->keybind = mod->getKeybind();
		switch (moduleMgr->getModule<ArrayListNuke>()->modes) {
		case 0: name = mod->getRawModuleName(); moduleName = name; break;
		case 1: name = mod->getModuleName(); moduleName = name; break;
		}
		if (moduleMgr->getModule<ArrayListNuke>()->showkb) {
			if (keybind != 0x0) {
				char text[50];
				sprintf_s(text, 50, "%s%s", name, std::string(std::string(GRAY) + " [" + std::string(WHITE) + std::string(Utils::getKeybindName(keybind)) + std::string(GRAY) + "]").c_str());
				moduleName = text;
			}
		}

		enabled = mod->isEnabled();
		backingModule = mod;
		pos = mod->getPos();

		if (!enabled && *pos == Vec2(0.f, 0.f)) shouldRender = false;
		textWidth = DrawUtils::getTextWidth(&moduleName, 1.f) + 5.f;
	}

	bool operator<(const IModuleContainer& other) const {
		if (textWidth == other.textWidth) return moduleName < other.moduleName;
		return textWidth > other.textWidth;
	}
};
#pragma endregion

void ArrayListNuke::onPostRender(MinecraftUIRenderContext* renderCtx) {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;

	auto interfaceMod = moduleMgr->getModule<Colors>();
	auto clickGUI = moduleMgr->getModule<ClickGuiMod>();

	Vec2 mousePos = *Game.getClientInstance()->getMousePos(); {
		Vec2 windowSizeReal = Game.getClientInstance()->getGuiData()->windowSizeReal;
		Vec2 windowSize = Game.getClientInstance()->getGuiData()->windowSize;
		mousePos = mousePos.div(windowSizeReal);
		mousePos = mousePos.mul(windowSize);
	}

	if (Game.canUseMoveKeys()) {
		//Vec2 windowSizeReal = Game.getClientInstance()->getGuiData()->windowSizeReal;
		//Vec2 racoonaids = Game.getClientInstance()->getGuiData()->windowSize;
		Vec2 windowSize = Game.getClientInstance()->getGuiData()->windowSize;
		static constexpr bool rightSide = true;
		set<IModuleContainer> moduleList;

		float arh = 2;
		//windowSize.x = positionX;
		//windowSize.y = positionY;
		if (bottom) {
			positionX = windowSize.x;
			positionY = windowSize.y - arh;
		}
		else
		{
			positionX = windowSize.x;
			positionY = 0;
		}
		float textSize = scale;
		float yOffset = positionY;
		float textPadding = spacing * textSize;
		float textHeight = 10.0f * textSize;

		if (bottom) invert = true;
		else invert = false;

		auto lock = moduleMgr->lockModuleList();
		vector<shared_ptr<IModule>>* getModuleList = moduleMgr->getModuleList();
		for (auto moduleInd : *getModuleList) {
			if (!moduleInd->isVisible()) continue;
			moduleList.emplace(IModuleContainer(moduleInd));
		}

		float lastModuleLength = 0.f;
		Vec4 underline;
		int index = 0;

		Vec4 selectableSurface;

		for (std::set<IModuleContainer>::iterator mod = moduleList.begin(); mod != moduleList.end(); ++mod) {
			if (!mod->shouldRender) continue;
			index++; int curIndex = -index * interfaceMod->spacing;

			if (invert) selectableSurface.y -= 10.2f;
			else selectableSurface.w += 10.2f;

			float textWidth;
			std::string textStr = mod->moduleName;
			textWidth = DrawUtils::getTextWidth(&textStr, textSize) + 2.f;
			if (mode.getSelectedValue() == 1 || mode.getSelectedValue() == 3 || mode.getSelectedValue() == 4) textWidth = DrawUtils::getTextWidth(&textStr, textSize) + 4.f;
			if (index == 1) selectableSurface = Vec4(windowSize.x - textWidth, windowSize.y, windowSize.x, windowSize.y + 10);

#pragma region Animations
			// Animations
			float xOffsetOri = windowSize.x - textWidth - (textPadding * 2);
			float xOffset = windowSize.x - mod->pos->x;

			switch (animation.getSelectedValue()) {
			case 0: case 1: mod->pos->x += ((textPadding * 2) + textWidth - mod->pos->x) * 0.25f; break;
			case 2: mod->pos->x += INFINITY; break;
			}

			if (xOffset < xOffsetOri) xOffset = xOffsetOri;
			if (!mod->enabled) {
				switch (animation.getSelectedValue()) {
				case 0: xOffset += mod->pos->y; mod->pos->y += 4.f; break;
				case 1: xOffset += mod->pos->y; mod->pos->y += ((textPadding * 2) + textWidth + 2.f - mod->pos->y) * 0.05f; break;
				case 2:xOffset += mod->pos->y; mod->pos->y += INFINITY; break;
				}

				if (!invert) yOffset -= mod->pos->y / (textHeight + (textPadding * 2) * 0.1f);
				else yOffset += mod->pos->y / (textHeight + (textPadding * 2) * 0.1f);
			}
			if (!mod->enabled && xOffset > windowSize.x) { mod->pos->x = 0.f; mod->pos->y = 0.f; }
#pragma endregion

#pragma region Math
			Vec4 rectPos = Vec4(xOffset - 3, yOffset, windowSize.x + (textPadding * 2), yOffset + textPadding * 2 + textHeight);
			Vec4 bar = Vec4(rectPos.z - 1.f, rectPos.y, rectPos.z, rectPos.w);
			if (mode.getSelectedValue() == 4) bar = Vec4(rectPos.z - 1.f, rectPos.y - 2.f, rectPos.z, rectPos.w);
			if (mode.getSelectedValue() == 4 && invert) bar = Vec4(rectPos.z - 1.f, rectPos.y, rectPos.z, rectPos.w + 2.f);
			Vec4 topLine = Vec4(rectPos.x - 1.f, rectPos.y - 1.f, rectPos.z, rectPos.y);
			Vec4 topLine2 = Vec4(rectPos.x, rectPos.y - 2, rectPos.z, rectPos.y);
			if (invert) topLine2 = Vec4(rectPos.x, rectPos.y + (textPadding * 2) + textHeight, rectPos.z, rectPos.y + (textPadding * 2) + textHeight + 2);
			Vec4 topLine3 = Vec4(rectPos.x, rectPos.y - 3.f, rectPos.z, rectPos.y - 2);
			if (invert) topLine3 = Vec4(rectPos.x, rectPos.y + (textPadding * 2) + textHeight + 2, rectPos.z, rectPos.y + (textPadding * 2) + textHeight + 3);
			if (invert) topLine = Vec4(rectPos.x, rectPos.y + (textPadding * 2) + textHeight - 1, rectPos.z + 1.f, rectPos.y + (textPadding * 2) + textHeight);
			Vec4 leftRect = Vec4(xOffset - 4, yOffset, xOffset - 3, yOffset + textPadding * 2 + textHeight);
			Vec2 textPos = Vec2(rectPos.x + 4.f, rectPos.y);
			if (mode.getSelectedValue() == 1) textPos = Vec2(rectPos.x + 4.f, rectPos.y + 1.f);
			if (invert) textPos = Vec2(rectPos.x + 3.5f, rectPos.y + 2.f);
			underline = Vec4(windowSize.x - (lastModuleLength + 4.f + (textPadding * 2.f)), leftRect.y, leftRect.x, leftRect.y + 1.f);
			if (invert) underline = Vec4(windowSize.x - (lastModuleLength + 4.f + (textPadding * 2.f)), leftRect.y + (textPadding * 2) + textHeight - 1, leftRect.x, leftRect.y + (textPadding * 2) + textHeight);

			if (selectableSurface.contains(&mousePos)) focused = true;
			else focused = false;
#pragma endregion

			// Drawing
			auto interfaceColor = MC_Color(255, 255, 255);
			if (opacity > 0) DrawUtils::fillRectangleA(rectPos, MC_Color(0, 0, 0, opacity));
			switch (mode.getSelectedValue()) {
			case 0:
				DrawUtils::fillRectangleA(leftRect, MC_Color(interfaceColor));
				if (mod->enabled)DrawUtils::fillRectangleA(underline, MC_Color(interfaceColor));
				DrawUtils::drawText(textPos, &textStr, MC_Color(interfaceColor), textSize, 1.f, true);
				break;
			case 1:
				DrawUtils::fillRectangleA(leftRect, MC_Color(interfaceColor));
				if (mod->enabled) DrawUtils::fillRectangleA(underline, MC_Color(interfaceColor));
				if (index == 1) DrawUtils::fillRectangleA(topLine, MC_Color(interfaceColor));
				DrawUtils::fillRectangleA(bar, MC_Color(interfaceColor));
				DrawUtils::drawText(textPos, &textStr, MC_Color(interfaceColor), textSize, 1.f, true);
				break;
			case 2:
				DrawUtils::fillRectangleA(leftRect, MC_Color(interfaceColor));
				DrawUtils::drawText(textPos, &textStr, MC_Color(interfaceColor), textSize, 1.f, true);
				break;
			case 3:
				DrawUtils::fillRectangleA(bar, MC_Color(interfaceColor));
				DrawUtils::drawText(textPos, &textStr, MC_Color(interfaceColor), textSize, 1.f, true);
				break;
			case 4:
				if (index == 1) DrawUtils::fillRectangleA(topLine2, MC_Color(0, 0, 0, opacity));
				if (index == 1) DrawUtils::fillRectangleA(topLine3, MC_Color(interfaceColor));
				DrawUtils::fillRectangleA(bar, MC_Color(interfaceColor));
				DrawUtils::drawText(textPos, &textStr, MC_Color(interfaceColor), textSize, 1.f, true);
				break;
			case 5: DrawUtils::drawText(textPos, &textStr, MC_Color(interfaceColor), textSize, 1.f, true); break;
			}

			if (invert) yOffset -= textHeight + (textPadding * 2);
			else yOffset += textHeight + (textPadding * 2);

			lastModuleLength = textWidth;
			underline = Vec4(leftRect.x, leftRect.w, windowSize.x, leftRect.w + 1.f);
			if (invert) underline = Vec4(leftRect.z, leftRect.y, windowSize.x, leftRect.y + 1.f);
		}

		index++; int curIndex = -index * interfaceMod->spacing;
		auto interfaceColor = MC_Color(255, 255, 255);
		if (mode.getSelectedValue() == 0 || mode.getSelectedValue() == 1) DrawUtils::fillRectangleA(underline, MC_Color(interfaceColor));
		moduleList.clear();
	}
}