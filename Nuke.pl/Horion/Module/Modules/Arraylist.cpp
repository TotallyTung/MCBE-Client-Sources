#include "Arraylist.h"
#include "../../Utils/ImGuiUtils.h"

float textSize = 1.f;
Arraylist::Arraylist() : IModule(0, Category::CLIENT, "Display your enabled modules on the HUD!") {
	registerBoolSetting("Show Keybinds", &showKey, showKey);
	registerBoolSetting("Bottom", &bottom, bottom);
	registerBoolSetting("Outline", &outline, outline);
	registerBoolSetting("Modes", &showMode, showMode);
	registerIntSetting("Alpha", &alpha, alpha, 0, 255);
	registerFloatSetting("Scale", &textSize, textSize, 0.1f, 3.f);
	registerFloatSetting("Blur Strength", &blurStrength, blurStrength, 0.f, 20.f);
}

Arraylist::~Arraylist() {
}

const char* Arraylist::getModuleName() {
	return ("Arraylist");
}

struct IModuleContainer {
	// Struct used to Sort IModules in a std::set
	std::shared_ptr<IModule> backingModule;
	std::string moduleName;
	std::string modName;
	std::string keybind;
	bool enabled;
	bool visible;
	float textWidth;
	Vec2* pos;

	IModuleContainer(std::shared_ptr<IModule> mod) {
		static Arraylist* arrayListMod = moduleMgr->getModule<Arraylist>();
		this->backingModule = mod;
		this->moduleName = std::string(mod->getModuleName());
		this->keybind = mod->getKeybind();
		if (mod->getModName() != nullptr && arrayListMod->showMode) {
			this->modName = std::string(mod->getModName());
			std::string fullModuleName = this->moduleName + std::string(" ") + this->modName;
			this->textWidth = ImGuiUtils::getTextWidth(fullModuleName.c_str(), 1.f);
		}
		else {
			this->textWidth = ImGuiUtils::getTextWidth(this->moduleName.c_str(), 1.f);
		}
		this->enabled = mod->isEnabled();
		this->visible = mod->isVisible();
		this->pos = mod->getModulePos();
	}

	bool operator<(const IModuleContainer& other) const {
		if (this->textWidth == other.textWidth)
			return moduleName < other.moduleName;
		return this->textWidth > other.textWidth;
	}
};

void Arraylist::onImGuiRender(ImDrawList* d) {
	if (Game.getLocalPlayer() == nullptr) return;
	if (!Game.canUseMoveKeys()) return;

	ImGuiIO& io = ImGui::GetIO();
	Vec2 windowSize = Game.getGuiData()->windowSizeReal;

	static std::set<IModuleContainer> modContainerList;
	// Fill modContainerList with Modules
	{
		std::vector<std::shared_ptr<IModule>>* moduleList = moduleMgr->getModuleList();
		for (std::shared_ptr<IModule> it : *moduleList) {
			modContainerList.emplace(IModuleContainer(it));
		}
	}
	static const float textPadding = 1.0f * textSize;
	float textHeight = ImGuiUtils::getTextHeight(textSize) - textPadding;
	static const float lineWidth = 1.f;
	static const UIColor grayColor = UIColor(175, 175, 175, 255);
	static const bool textShadow = true;
	static const int seperation = 25;

	static Colors* colorsMod = moduleMgr->getModule<Colors>();
	UIColor mainColor = colorsMod->getColor();
	float yOffset = this->bottom ? (windowSize.y - textHeight) : 0.f;
	int i = 0;

	// Update ModulePos
	for (auto it = modContainerList.begin(); it != modContainerList.end(); ++it) {
		float textWidth = it->textWidth;
		float xOffsetOri = windowSize.x - textWidth - textPadding * 2.f;
		it->pos->x = smoothLerp((it->enabled && it->visible) ? windowSize.x - xOffsetOri : -2.f, it->pos->x, io.DeltaTime * 15.f);
		if (it->pos->x > windowSize.x - xOffsetOri) it->pos->x = windowSize.x - xOffsetOri;
	}

	if (blurStrength > 0.f) { // Blur
		ImFX::Begin(d);
		for (auto it = modContainerList.begin(); it != modContainerList.end(); ++it) {
			//mainColor = ColorUtil::getRainbowColor(3.f, 1.f, 1.f, (bottom ? -i : i) * (seperation * 2.f));
			float textWidth = it->textWidth;
			float xOffsetOri = windowSize.x - textWidth - textPadding * 2.f;
			float xOffset = windowSize.x - it->pos->x;

			float duration = ((windowSize.x - xOffset) / (windowSize.x - xOffsetOri));
			if (duration < 0.f) duration = 0.f;

			Vec4 rectPos(xOffsetOri - 2.f, yOffset + (textHeight - textPadding * 2.f) * (bottom ? (1.f - duration) : 0.f), windowSize.x, yOffset + (textHeight - textPadding * 2.f) * (bottom ? 1.f : duration));
			if (i == 0 && bottom) rectPos.w += textPadding * 2.f;
			if (duration > 0.01f) {
				//ImGuiUtils::drawShadowRect(rectPos, mainColor, 50.f * duration, false);
				ImFX::AddBlur(blurStrength, ImVec4(rectPos.x, rectPos.y, rectPos.z, rectPos.w));
				i++;
				yOffset += (textHeight - textPadding * 2.f) * duration * (bottom ? -1.f : 1.f);
			}
		}
		ImFX::End();
	}

	yOffset = this->bottom ? (windowSize.y - textHeight) : 0.f;
	i = 0;

	Vec4 lastRect(0.f, 0.f, 0.f, 0.f);
	for (auto it = modContainerList.begin(); it != modContainerList.end(); ++it) {
		//mainColor = ColorUtil::getRainbowColor(3.f, 1.f, 1.f, (bottom ? -i : i) * (seperation * 2.f));
		std::string moduleName = it->moduleName;
		std::string modName = it->modName;
		float textWidth = it->textWidth;
		float xOffsetOri = windowSize.x - textWidth - textPadding * 2.f;
		float xOffset = windowSize.x - it->pos->x;

		//it->pos->x = smoothLerp((it->enabled && it->visible) ? windowSize.x - xOffsetOri : -2.f, it->pos->x, io.DeltaTime * 15.f);
		float duration = ((windowSize.x - xOffset) / (windowSize.x - xOffsetOri));
		if (duration < 0.f) duration = 0.f;

		Vec2 textPos(xOffset, yOffset - textPadding);
		Vec4 rectPos(xOffsetOri - 2.f, yOffset + (textHeight - textPadding * 2.f) * (bottom ? (1.f - duration) : 0.f), windowSize.x, yOffset + (textHeight - textPadding * 2.f) * (bottom ? 1.f : duration));
		if (i == 0 && bottom) rectPos.w += textPadding * 2.f;
		if (duration > 0.01f) {
			if (alpha > 0) ImGuiUtils::drawRectFilled(rectPos, UIColor(0, 0, 0, alpha));
			if (outline) {
				if (lastRect.x != 0.f) {
					if (!bottom) ImGuiUtils::drawRectFilled(Vec4(lastRect.x - lineWidth, rectPos.y, rectPos.x, rectPos.y + lineWidth), mainColor);
					else ImGuiUtils::drawRectFilled(Vec4(lastRect.x - lineWidth, rectPos.w, rectPos.x, rectPos.w - lineWidth), mainColor);
				}
				ImGuiUtils::drawRectFilled(Vec4(rectPos.x - lineWidth, rectPos.y, rectPos.x, rectPos.w), mainColor);
			}
			ImGuiUtils::drawText(textPos, moduleName.c_str(), UIColor(mainColor.r, mainColor.g, mainColor.b, (float)(255 * duration)), textSize, textShadow);
			if (it->backingModule->getModName() != nullptr && showMode) {
				std::string spacing = moduleName + " ";
				float ModuleWidth = ImGuiUtils::getTextWidth(spacing.c_str(), textSize);
				ImGuiUtils::drawText(Vec2(textPos.x + ModuleWidth, textPos.y), modName.c_str(), UIColor(255, 255, 255, (float)(255 * duration)), textSize, textShadow);
			}
			lastRect = rectPos;
			i++;
			yOffset += (textHeight - textPadding * 2.f) * duration * (bottom ? -1.f : 1.f);
		}
	}
	if (outline) {
		if (!bottom) ImGuiUtils::drawRectFilled(Vec4(lastRect.x - lineWidth, lastRect.w, windowSize.x, lastRect.w + lineWidth), mainColor);
		else ImGuiUtils::drawRectFilled(Vec4(lastRect.x - lineWidth, lastRect.y, windowSize.x, lastRect.y - lineWidth), mainColor);
	}
	modContainerList.clear();
}
