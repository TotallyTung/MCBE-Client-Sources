#include "../../../Utils/ClientColors.h"
#include "../../../Utils/ColorUtil.h"
#include "BindList.h"

Bindlist::Bindlist() : IModule(0, Category::VISUAL, "Display your enabled modules on the HUD!") {
	registerFloatSetting("Opacity", &alpha, alpha, 0.f, 1.f);
	registerFloatSetting("Color Speed", &cycleSpeed, cycleSpeed, 1.f, 5.f);
	registerFloatSetting("Saturation", &saturation, saturation, 0.f, 1.f);
}

Bindlist::~Bindlist() {
}

const char* Bindlist::getModuleName() {
	return ("BindList");
}

struct IModuleContainer {
	// Struct used to Sort IModules in a std::set
	std::shared_ptr<IModule> backingModule;
	std::string moduleName;
	int keybind;
	float textWidth;
	Vec2* pos;
	bool shouldRender = true;

	IModuleContainer(std::shared_ptr<IModule> mod) {
		static auto arrayMod = moduleMgr->getModule<Bindlist>();
		static auto hudMod = moduleMgr->getModule<HudModule>();
		const char* moduleNameChr = mod->getModuleName();
		this->keybind = mod->getKeybind();
		this->backingModule = mod;

		if (keybind == 0x0)
			moduleName = moduleNameChr;
		else {
			char text[50];
			sprintf_s(text, 50, "%s%s", moduleNameChr, std::string(" [" + std::string(Utils::getKeybindName(keybind)) + "]").c_str());
			moduleName = text;
		}
		this->textWidth = DrawUtils::getTextWidth(&moduleName, 1.f);
	}

	bool operator<(const IModuleContainer& other) const {
		if (this->textWidth == other.textWidth)
			return moduleName < other.moduleName;
		return this->textWidth > other.textWidth;
	}
};

void Bindlist::onPostRender(MinecraftUIRenderContext* renderCtx) {
	static auto hudMod = moduleMgr->getModule<HudModule>();

	Vec2 windowSize = Game.getClientInstance()->getGuiData()->windowSize;
	Vec2 windowSizeReal = Game.getClientInstance()->getGuiData()->windowSizeReal;
	Vec2 mousePos = *Game.getClientInstance()->getMousePos();

	// Convert mousePos to visual Pos
	{
		mousePos = mousePos.div(windowSizeReal);
		mousePos = mousePos.mul(windowSize);
	}

	// Mouse click detector
	static bool wasLeftMouseDown = GameData::isLeftClickDown();  // Last isDown value
	bool leftMouseDown = GameData::isLeftClickDown();            // current isDown value

	bool executeClick = leftMouseDown && leftMouseDown != wasLeftMouseDown;  // isDown == true AND (current state IS NOT last state)
	wasLeftMouseDown = leftMouseDown;                                        // Set last isDown value

	std::set<IModuleContainer> modContainerList;
	// Fill modContainerList with Modules
	{
		std::vector<std::shared_ptr<IModule>>* moduleList = moduleMgr->getModuleList();
		for (auto it : *moduleList) {
			if (it.get()->getKeybind() != 0x0)
				modContainerList.emplace(IModuleContainer(it));
		}
	}

	// Parameters
	float textSize = 1,f;
	float textPadding = 1.0f * textSize;
	float textHeight = 10.0f * textSize;
	int seperation = 50;  // Adjust this to change the seperation of the colors
	int index = 0;
	float yOffset = windowSize.y;
	yOffset -= textHeight;

	for (auto it = modContainerList.begin(); it != modContainerList.end(); ++it) {
		auto& container = *it;
		auto textStr = container.moduleName;
		auto textWidth = container.textWidth;

		auto arrayColor = ColorUtil::getRainbowColor(cycleSpeed, saturation, 1, index * (seperation * 2));
		auto xOffsetOri = windowSize.x - textWidth - (textPadding * 2);
		auto xOffset = xOffsetOri;

		auto textPos = Vec2(xOffset + textPadding, yOffset + textPadding);
		auto rectPos = Vec4(xOffset - 2, yOffset, windowSize.x, yOffset + textPadding * 2 + textHeight);
		auto sideRect = Vec4(xOffset - 2, yOffset, xOffset - 1, yOffset + textPadding * 2 + textHeight);

		if (alpha > 0) {
			DrawUtils::fillRectangle(rectPos, MC_Color(255,255,255), alpha);
			DrawUtils::fillRectangle(sideRect, MC_Color(arrayColor.r, arrayColor.g, arrayColor.b), 1.f);
		}
		if (!GameData::canUseMoveKeys() && rectPos.contains(&mousePos) && clickToggle) {
			auto selectedRect = rectPos;
			selectedRect.x = sideRect.z;
			if (leftMouseDown) {
				DrawUtils::fillRectangle(selectedRect, MC_Color(0.8f, 0.8f, 0.8f), 0.8f);
				if (executeClick) container.backingModule->toggle();
			} else {
				DrawUtils::fillRectangle(selectedRect, MC_Color(0.8f, 0.8f, 0.8f, 0.8f), 0.3f);
			}
		}
		DrawUtils::drawText(textPos, &textStr, MC_Color(arrayColor.r, arrayColor.g, arrayColor.b), textSize);

		yOffset -= ((10.0f * textSize) + (textPadding * 2)) * ((windowSize.x - xOffset) / (windowSize.x - xOffsetOri));
		++index;
	}
	modContainerList.clear();
}
