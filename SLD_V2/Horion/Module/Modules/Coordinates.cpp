#include "Coordinates.h"
#include "../../DrawUtils.h"

Coordinates::Coordinates() : IModule(0, Category::HUD, "Shows the postion of the player") {
	registerFloatSetting("CoordsX", &this->coordsX, this->coordsX, 0.f, g_Data.getClientInstance()->getGuiData()->windowSize.x);
	registerFloatSetting("CoordsY", &this->coordsY, this->coordsY, 0.f, g_Data.getClientInstance()->getGuiData()->windowSize.y);
	registerFloatSetting("Scale", &this->scale, this->scale, 0.5f, 1.5f);
	registerBoolSetting("RGBA", &this->rgba, this->rgba);
}

Coordinates::~Coordinates() {
}

const char* Coordinates::getModuleName() {
	return ("Coordinates");
}

void Coordinates::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	static float rcolors2[4];          // Rainbow color array RGBA
	static float rcolors[4];           // Rainbow color array RGBA
	static float disabledRcolors[4];   // Rainbow Colors, but for disabled modules
	static float disabledRcolors1[4];  // Rainbow Colors, but for disabled modules
	static float currColor[4];         // ArrayList colors
	{
		// Rainbow color updates
		{
			Utils::ApplyRainbow(rcolors, 0.002);  // Increase Hue of rainbow color array
			disabledRcolors[0] = std::min(10.f, rcolors[0] * 0.20f + -22.f);
			disabledRcolors[1] = std::min(10.f, rcolors[2] * 0.21f + -24.f);
			disabledRcolors[2] = std::min(10.f, rcolors[3] * 0.22f + -26.f);
			disabledRcolors[3] = 1;
		}
		// Rainbow color updates
		{
			Utils::ApplyRainbow(rcolors2, 0.008);  // Increase Hue of rainbow color array
			disabledRcolors1[0] = std::min(10.f, rcolors2[0] * 0.20f + -22.f);
			disabledRcolors1[1] = std::min(10.f, rcolors2[2] * 0.21f + -24.f);
			disabledRcolors1[2] = std::min(10.f, rcolors2[3] * 0.22f + -26.f);
			disabledRcolors1[3] = 1;
		}
	}
	int a = 0;
	int c = 0;
	c++;
	a++;

	float di = 0;
	di++;
	/*float timer = 0;
					if ((di == 0 <= timer) && !(di == 255 <= timer)) di++;
					if (!(di == 0 <= timer) && (di == 255 <= timer)) di--;
					if (timer == 510) di = 0;
					if (timer == 510) timer = 0; */

	currColor[3] = rcolors[3];
	Utils::ColorConvertRGBtoHSV(rcolors[0], rcolors[1], rcolors[2], currColor[0], currColor[3], currColor[2]);
	currColor[0] += 0.2f / a * c;
	Utils::ColorConvertHSVtoRGB(currColor[0], currColor[2], currColor[2], currColor[0], currColor[1], currColor[2]);

	if (!(g_Data.getLocalPlayer() == nullptr || !GameData::canUseMoveKeys())) {
		std::string tempStr("Movement");
		float len = DrawUtils::getTextWidth(&tempStr, scale) + 7.f;
		float f = 10.f * this->scale;

		if (!(g_Data.getLocalPlayer() == nullptr)) {
			vec3_t* pos = g_Data.getLocalPlayer()->getPos();
			float yVal = coordsY;
			float xVal = coordsX;
			int curDim = 0;
			C_LocalPlayer* player = g_Data.getLocalPlayer();
			player->getDimensionId(&curDim);

			if (curDim == 0 || curDim == 2) {
				std::string coordsX = std::to_string((int)floorf(pos->x));
				std::string coordsY = std::to_string((int)floorf(pos->y - 1.f));
				std::string coordsZ = std::to_string((int)floorf(pos->z));

				vec4_t rectPos = vec4_t(2.5f, yVal + 15.f * scale - 0.001f, len, yVal + 35.f * scale - 0.001f);
				vec2_t textPos = vec2_t(xVal, yVal);
				vec2_t outLinePos = vec2_t(textPos.x + 0.6f * scale - 0.001f, textPos.y - 0.6f * scale - 0.001f);
				// DrawUtils::drawRectangle(rectPos, MC_Color(0, 0, 0), 1.f);
				// DrawUtils::fillRectangle(rectPos, MC_Color(255, 255, 255), 1.f);
				if (curDim == 0) {
					std::string coordsText = "Position: " + coordsX + " " + coordsY + " " + coordsZ;
					if (rgba) {
						DrawUtils::drawText(textPos, &coordsText, MC_Color(currColor), scale);
					} else {
						DrawUtils::drawText(textPos, &coordsText, MC_Color(255, 255, 255), scale);
					}
					yVal += f;
				} else if (curDim == 2) {
					std::string coordsText = "End: " + coordsX + " " + coordsY + " " + coordsZ;
					if (rgba) {
						DrawUtils::drawText(textPos, &coordsText, MC_Color(currColor), scale);
					} else {
						DrawUtils::drawText(textPos, &coordsText, MC_Color(255, 255, 255), scale);
					}
					yVal += f;
				}
				std::string NcoordsX = std::to_string((int)floorf(pos->x + 0.001) / 8);
				std::string NcoordsZ = std::to_string((int)floorf(pos->z + 0.001) / 8);
				vec4_t NrectPos = vec4_t(2.5f, yVal + 15.f * scale - 0.001f, len, yVal + 45.f * scale - 0.001f);
				vec2_t NtextPos = vec2_t(xVal, yVal);
				vec2_t NoutLinePos = vec2_t(NtextPos.x + 0.6f * scale - 0.001f, NtextPos.y - 0.6f * scale - 0.001f);
				std::string NcoordsText = "Nether: " + NcoordsX + " " + coordsY + " " + NcoordsZ;
				if (rgba) {
					DrawUtils::drawText(NtextPos, &NcoordsText, MC_Color(currColor), scale);
				} else {
					DrawUtils::drawText(NtextPos, &NcoordsText, MC_Color(255, 255, 255), scale);
				}
			}

			else if (curDim == 1) {
				std::string coordsY = std::to_string((int)floorf(pos->y - 1.f));
				std::string NcoordsX = std::to_string((int)floorf(pos->x));
				std::string NcoordsZ = std::to_string((int)floorf(pos->z));
				vec4_t NrectPos = vec4_t(2.5f, yVal + 15.f * 1, len, yVal + 35.f * scale - 0.001f);
				vec2_t NtextPos = vec2_t(xVal, yVal);
				vec2_t NoutLinePos = vec2_t(NtextPos.x + 0.6f * 1, NtextPos.y - 0.6f * scale - 0.001f);
				std::string NcoordsText = "Nether: " + NcoordsX + " " + coordsY + " " + NcoordsZ;
				DrawUtils::drawText(NtextPos, &NcoordsText, MC_Color(currColor), scale);
				yVal += f;

				std::string coordsX = std::to_string((int)floorf(pos->x + 0.001) * 8);
				std::string coordsZ = std::to_string((int)floorf(pos->z + 0.001) * 8);

				vec4_t rectPos = vec4_t(2.5f, yVal + 15.f * scale - 0.001f, len, yVal + 45.f * scale - 0.001f);
				vec2_t textPos = vec2_t(xVal, yVal);
				vec2_t outLinePos = vec2_t(textPos.x + 0.6f * scale - 0.001f, textPos.y - 0.6f * scale - 0.001f);
				// DrawUtils::fillRectangle(rectPos, MC_Color(0.f, 0.f, 0.f), 0.37f);
				std::string coordsText = coordsX + " " + coordsY + " " + coordsZ;
				DrawUtils::drawText(textPos, &coordsText, MC_Color(currColor), scale);
			}
		}
	}
}