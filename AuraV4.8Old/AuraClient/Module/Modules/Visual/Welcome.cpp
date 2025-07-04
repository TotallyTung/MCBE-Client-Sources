#include "Welcome.h"
#include "../../../../Utils/DrawUtils.h"
#include "../../ModuleManager.h"

Welcome::Welcome() : IModule(0x0, Category::VISUAL, "Welcomes you at the top of the screen") {
	TextMode.addEntry("Welcome", 1);
	TextMode.addEntry("Aura", 2);
	TextMode.addEntry("Nya", 3);
	TextMode.addEntry("HowAreU", 4);
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

void Welcome::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	static float rcolors[4];

	if (g_Data.isInGame() && GameData::canUseMoveKeys()) {
		float nameTextSize = scale + 0.5f;
		static const float textHeight = (nameTextSize * 0.7f) * DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
		constexpr float borderPadding = 1;
		constexpr float margin = 5;
		std::string playerName = std::string(g_Data.getLocalPlayer()->getNameTag()->getText());

		switch (TextMode.getSelectedValue()) {
		case 1:
			name = "Greetings, " + playerName + "!";
			break;

		case 2:
			name = "Welcome to AuraV4, " + playerName;
			break;

		case 3:
			name = "Nyaa~ " + playerName;
			break;
		case 4:
			name = "How are you today, " + playerName + "?";
			break;
		}
		auto interfaceColor = ColorUtil::interfaceColor(1);
		float nameLength = DrawUtils::getTextWidth(&name, scale);
		float fullTextLength = nameLength;
		float x = windowSize.x - windowSize.x / 2 - nameLength / 2;
		float y = windowSize.y - windowSize.y;
		vec4_t rect = vec4_t(
			windowSize.x - margin - fullTextLength - borderPadding * 415,
			windowSize.y - margin - textHeight,
			windowSize.x - margin + borderPadding,
			windowSize.y - margin);
		//DrawUtils::drawText(vec2_t(x, y + 2), &name, MC_Color(interfaceColor), nameTextSize, 1.f, true);
		float texthai = DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() * scale;
		DrawUtils::drawCenteredString(vec2_t(windowSize.x / 2, windowSize.y - windowSize.y + 7.5), &name, scale, MC_Color(interfaceColor), true);
		if (outline)
			DrawUtils::drawRectangle(vec4_t(x - 1.5, y - 2, windowSize.x / 2 + nameLength / 2 + 1.5, 2 + texthai), MC_Color(interfaceColor), 1.f, 1.f);
		
		if (opati != 0)
			DrawUtils::fillRectangleA(vec4_t(x - 1.5, y - 2, windowSize.x / 2 + nameLength / 2 + 1.5, 2 + texthai), MC_Color(0, 0, 0, opati));
			
	}
}
