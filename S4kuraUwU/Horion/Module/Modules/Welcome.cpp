#include "Welcome.h"
#include "../../DrawUtils.h"
#include "../ModuleManager.h"
#include "Theme.h"
using namespace std;
int Welcome::rFloat{255};
int Welcome::gFloat{255};
int Welcome::bFloat{255};
bool reberse = false;
Welcome::Welcome() : IModule(0x0, Category::HUD, "Welcomes you with various messages") {
	registerEnumSetting("Color", &Color, 0);
	Color = SettingEnum(this)
				  .addEntry(EnumEntry("Custom Color", 0))
				  .addEntry(EnumEntry("Rainbow Wave", 1));
	registerEnumSetting("Mode", &TextMode, 0);
	TextMode = SettingEnum(this)
				   .addEntry(EnumEntry("Meow", 0))
				   .addEntry(EnumEntry("Good Morning!", 1))
				   .addEntry(EnumEntry("God Bless", 2))
				   .addEntry(EnumEntry("S4kuraUwU", 3));
	registerBoolSetting("Reverse", &reberse, reberse);

	registerFloatSetting("PrimaryRed", &r1, r1, 0.f, 255.f);
	registerFloatSetting("PrimaryGreen", &g1, g1, 0.f, 255.f);
	registerFloatSetting("PrimaryBlue", &b1, b1, 0.f, 255.f);
	registerFloatSetting("SecondaryRed", &r2, r2, 0.f, 255.f);
	registerFloatSetting("SecondaryGreen", &g2, g2, 0.f, 255.f);
	registerFloatSetting("SecondaryBlue", &b2, b2, 0.f, 255.f);
	registerFloatSetting("Wave Speed", &sped, sped, 0.f, 100.f);
	registerFloatSetting("Scale", &this->scale, this->scale, 0.5f, 1.5f);
}

Welcome::~Welcome() {
}

const char* Welcome::getModuleName() {
	return ("Welcome");
}

void Welcome::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	static auto ThemeMod = moduleMgr->getModule<Theme>();

	vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	static float rcolors[4];

	if (g_Data.isInGame() && GameData::canUseMoveKeys()) {
		float nameTextSize = scale + 0.5f;
		static const float textHeight = (nameTextSize * 0.7f) * DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
		constexpr float borderPadding = 1;
		constexpr float margin = 5;
		std::string playerName = std::string(g_Data.getLocalPlayer()->getNameTag()->getText());

		switch (TextMode.selected) {
		default:
			name = "Meow~  " + playerName;
			break;

		case 1:
			name = "Good Morning! " + playerName + " :^)";
			break;

		case 2:
			name = "God Blessing To " + playerName;
			break;

		case 3:
			name = "Welcome To S4kuraUwU  " + playerName;
			break;
		}
		float nameLength = DrawUtils::getTextWidth(&name, nameTextSize);
		float fullTextLength = nameLength;
		float x = windowSize.x - windowSize.x / 2 - nameLength / 2;
		float y = windowSize.y - windowSize.y;
		vec4_t rect = vec4_t(
			windowSize.x - margin - fullTextLength - borderPadding * 415,
			windowSize.y - margin - textHeight,
			windowSize.x - margin + borderPadding,
			windowSize.y - margin);
		static float rcolors[4];          // Rainbow color array RGBA
		static float disabledRcolors[4];  // Rainbow Colors, but for disabled modules
		static float currColor[4];        // ArrayList colors
		{
			Utils::ApplyRainbow(rcolors, 0.0008f);  // Increase Hue of rainbow color array
			disabledRcolors[0] = std::min(1.f, rcolors[0] * 0.4f + 0.2f);
			disabledRcolors[1] = std::min(1.f, rcolors[1] * 0.4f + 0.2f);
			disabledRcolors[2] = std::min(1.f, rcolors[2] * 0.4f + 0.2f);
			disabledRcolors[3] = 1;
		}
		currColor[3] = rcolors[5];
		currColor[3] = rcolors[5];
		Utils::ColorConvertRGBtoHSV(rcolors[0 & 1], rcolors[2], rcolors[01], currColor[0], currColor[1], currColor[2]);
		currColor[0] += 0.5f;
		Utils::ColorConvertHSVtoRGB(currColor[0 & 1], currColor[2], currColor[2], currColor[0], currColor[1], currColor[2]);
		switch (Color.selected) {
		default: {
			static float rcolors2[4];
			static float disabledRcolors2[4];
			static float currColor2[4];
			int a = 0;
			int b = 0;
			int c = 0;
			{
				Utils::ApplyRainbow(rcolors2);  // Increase Hue of rainbow color array
				disabledRcolors2[0] = std::min(1.f, rcolors2[0] * 0.4f + 0.2f);
				disabledRcolors2[1] = std::min(1.f, rcolors2[1] * 0.4f + 0.2f);
				disabledRcolors2[2] = std::min(1.f, rcolors2[2] * 0.4f + 0.2f);
				disabledRcolors2[3] = 1;
			}
			c++;
			b++;
			if (b < 20)
				a = moduleMgr->getEnabledModuleCount() * 2;
			else
				b = 0;
			currColor2[3] = rcolors[3];
			Utils::ColorConvertRGBtoHSV(rcolors2[0], rcolors2[1], rcolors2[2], currColor2[0], currColor2[1], currColor2[2]);
			currColor2[0] += 80.f / a * c;
			Utils::ColorConvertHSVtoRGB(currColor2[0], currColor2[1], currColor2[2], currColor2[0], currColor2[1], currColor2[2]);
			int ind = 1;
			for (char c : name) {
				ind++;
				int charWidth = DrawUtils::getTextWidth(&string(&c), nameTextSize);
				if (ThemeMod->rainbow) {
					DrawUtils::drawText(vec2_t(x, y), &string(&c), MC_Color(currColor2), nameTextSize, 1);
				} else {
					DrawUtils::drawText(vec2_t(x, y), &string(&c), MC_Color(Theme::rFloat, Theme::gFloat, Theme::bFloat), nameTextSize, 1);
				}
				x += charWidth;
			}
			break;
		}
		case 1: {
			int ind = 1;
			for (char c : name) {
				ind++;
				int charWidth = DrawUtils::getTextWidth(&string(&c), nameTextSize);
				if (reberse) {
					MC_Color arrayColor = DrawUtils::getWaveRGB(r1, g1, b1, r2, g2, b2, -ind * sped);
					DrawUtils::drawText(vec2_t(x, y), &string(&c), arrayColor, nameTextSize, 1);
				} else {
					MC_Color arrayColor = DrawUtils::getWaveRGB(r1, r1, r1, r2, g2, b2, -ind * (sped * -1));
					DrawUtils::drawText(vec2_t(x, y), &string(&c), arrayColor, nameTextSize, 1);
				}
				x += charWidth;
			}
			break;
		}
		}
	}
}