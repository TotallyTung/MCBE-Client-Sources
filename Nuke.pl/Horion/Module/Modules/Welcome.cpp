#include "Welcome.h"
#include "../../DrawUtils.h"

Welcome::Welcome() : IModule(0x0, Category::VISUAL, "Welcomes you at the top of the screen") {
	TextMode.addEntry("Welcome", 1);
	TextMode.addEntry("Nuke", 2);
	TextMode.addEntry("Xin Chao", 3);
	TextMode.addEntry("Ditmemay", 4);
	registerEnumSetting("Text Modes", &TextMode, 1);
	
	registerFloatSetting("Scale", &this->scale, this->scale, 0.5f, 1.5f);
	registerIntSetting("Opacity", &opati, opati, 0, 255);
	registerBoolSetting("Outline", &outline, outline);
}

Welcome::~Welcome() {
}

const char* Welcome::getModuleName() {
	return ("Welcome");
}

void Welcome::onImGUIRender(MinecraftUIRenderContext* renderCtx) {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;

	Vec2 windowSize = Game.getClientInstance()->getGuiData()->windowSize;
	static float rcolors[4];

	if (Game.isInGame() && GameData::canUseMoveKeys()) {
		float nameTextSize = scale + 0.5f;
		static const float textHeight = (nameTextSize * 0.7f) * DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
		constexpr float borderPadding = 1;
		constexpr float margin = 5;
		std::string playerName = std::string(Game.getLocalPlayer()->getNameTag()->getText());

		switch (TextMode.getSelectedValue()) {
		case 1:
			name = "Welcome " + playerName + " :^)";
			break;

		case 2:
			name = "Welcome to Nuke.pl " + playerName;
			break;

		case 3:
			name = "Xin chao " + playerName;
			break;
		case 4:
			name = "Ditmemay " + playerName + " =))";
			break;
		}
		float nameLength = DrawUtils::getTextWidth(&name, scale);
		float fullTextLength = nameLength;
		float x = windowSize.x - windowSize.x / 2 - nameLength / 2;
		float y = windowSize.y - windowSize.y;
		Vec4 rect = Vec4(
			windowSize.x - margin - fullTextLength - borderPadding * 415,
			windowSize.y - margin - textHeight,
			windowSize.x - margin + borderPadding,
			windowSize.y - margin);
		//DrawUtils::drawText(Vec2(x, y + 2), &name, MC_Color(interfaceColor), nameTextSize, 1.f, true);
		float texthai = DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() * scale;
		DrawUtils::drawCenteredString(Vec2(windowSize.x / 2, windowSize.y - windowSize.y + 7.5), &name, scale, MC_Color(interfaceColor), true);
		if (outline)
			DrawUtils::drawRectangle(Vec4(x - 1.5, y - 2, windowSize.x / 2 + nameLength / 2 + 1.5, 2 + texthai), MC_Color(interfaceColor), 1.f, 1.f);
		
		if (opati != 0)
			DrawUtils::fillRectangleA(Vec4(x - 1.5, y - 2, windowSize.x / 2 + nameLength / 2 + 1.5, 2 + texthai), MC_Color(0, 0, 0, opati));
			
	}
}
