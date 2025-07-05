#include "../../../Utils/Logger.h"
#include "../../DrawUtils.h"
#include "InvView.h"
#include "Theme.h"
float InventoryView::rFloat{255};
float InventoryView::gFloat{255};
float InventoryView::bFloat{255};

InventoryView::InventoryView() : IModule(0x0, Category::VISUAL, "renders inventory on screen") {
	registerBoolSetting("Outline", &this->outline, this->outline);
	registerEnumSetting("Color", &Color, 0);
	Color = SettingEnum(this)
				  .addEntry(EnumEntry("Custom Color", 0));
	//registerFloatSetting("Red", &rFloat, rFloat, 0.f, 255.f);
	//registerFloatSetting("Green", &gFloat, gFloat, 0.f, 255.f);
	//registerFloatSetting("Blue", &bFloat, bFloat, 0.f, 255.f);
	registerFloatSetting("invPosX", &this->invX, this->invX, 0.f, g_Data.getClientInstance()->getGuiData()->windowSize.x);
	registerFloatSetting("invPosY", &this->invY, this->invY, 0.f, g_Data.getClientInstance()->getGuiData()->windowSize.y);
}

InventoryView::~InventoryView() {
}

const char* InventoryView::getModuleName() {
	return ("InvView");
}

float xDiff2 = -1.f;
float yDiff2 = -1.f;
void InventoryView::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	static auto ThemeMod = moduleMgr->getModule<Theme>();
	switch (Color.selected) {
	default:
		pastel = true;
		rainbow = false;

		vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
		vec2_t windowSizeReal = g_Data.getClientInstance()->getGuiData()->windowSizeReal;
		vec2_t mousePosition = *g_Data.getClientInstance()->getMousePos();
		mousePosition = mousePosition.div(windowSizeReal);
		mousePosition = mousePosition.mul(windowSize);

		if (!(g_Data.getLocalPlayer() == nullptr || !GameData::canUseMoveKeys())) {
			C_LocalPlayer* player = g_Data.getLocalPlayer();
			C_PlayerInventoryProxy* supplies2 = g_Data.getLocalPlayer()->getSupplies();
			C_Inventory* inv2 = supplies2->inventory;
			g_Data.getLocalPlayer()->getSupplies()->inventory;
			vec2_t invPos = vec2_t(100.f, 100.f);
			vec2_t invPos2 = vec2_t(100.f, 120.f);
			C_ItemStack* test = inv2->getItemStack(1);
			C_ItemStack* test2 = inv2->getItemStack(2);
			float testF = 100.f;
			float sideBars = 1.5f;
			float yVal = invY + sideBars;

			xVal = invX + sideBars;

			static float rcolors[4];

			if (rcolors[3] < 1) {
				rcolors[0] = 1;
				rcolors[1] = 0.2f;
				rcolors[2] = 0.2f;
				rcolors[3] = 1;
			}

			Utils::ColorConvertRGBtoHSV(rcolors[0], rcolors[1], rcolors[2], rcolors[0], rcolors[1], rcolors[2]);  // perfect code, dont question this

			rcolors[0] += 0.0015f;
			if (rcolors[0] >= 1) {
				rcolors[0] = 0;
			}

			Utils::ColorConvertHSVtoRGB(rcolors[0], rcolors[1], rcolors[2], rcolors[0], rcolors[1], rcolors[2]);

			int a = 0;
			int b = 0;
			int c = 0;
			static float rcolors2[4];
			static float disabledRcolors2[4];
			static float currColor2[4];
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
			currColor2[3] = rcolors2[3];
			Utils::ColorConvertRGBtoHSV(rcolors2[0], rcolors2[1], rcolors2[2], currColor2[0], currColor2[1], currColor2[2]);
			currColor2[0] += 80.f / a * c;
			Utils::ColorConvertHSVtoRGB(currColor2[0], currColor2[1], currColor2[2], currColor2[0], currColor2[1], currColor2[2]);

			if (ThemeMod->rainbow) {
				DrawUtils::setColor(currColor2[0], currColor2[1], currColor2[2], 1.f);
			}
			else {
				DrawUtils::setColor(Theme::rFloat, Theme::gFloat, Theme::bFloat, 1.f);
			}
			if (outline)
				DrawUtils::drawRectangle(vec2_t(invX, invY), vec2_t(invX + 150.f + sideBars, invY + 60.f + sideBars), sideBars / 2);

			for (int j = 9; j < 18; j++) {
				if ((inv2->getItemStack(j)->item != nullptr)) {
					DrawUtils::drawItem(inv2->getItemStack(j), vec2_t(xVal, yVal + 0.f), 1.f, 1.f - .30f, false);
					char stackAmount = inv2->getItemStack(j)->count;
					std::string stackStr = std::to_string(inv2->getItemStack(j)->count);
					if (inv2->getItemStack(j)->count > 1) {
						if (ThemeMod->rainbow) {
							DrawUtils::drawText(vec2_t(xVal + 6.5f, (yVal + 0.f) + 9.5f), &stackStr, MC_Color(currColor2), 1.f - .20f);
						} else {
							DrawUtils::drawText(vec2_t(xVal + 6.5f, (yVal + 0.f) + 9.5f), &stackStr, MC_Color(Theme::rFloat, Theme::gFloat, Theme::bFloat), 1.f - .20f);
						}
					}
				}
				xVal += 17.f;
			}
			xVal -= 180.f;
			for (int j = 18; j < 27; j++) {
				if ((inv2->getItemStack(j)->item != nullptr)) {
					DrawUtils::drawItem(inv2->getItemStack(j), vec2_t(xVal + 27.f, yVal + 20.f), 1.f, 1.f - .30f, false);
					std::string stackStr = std::to_string(inv2->getItemStack(j)->count);
					if (inv2->getItemStack(j)->count > 1) {
						if (ThemeMod->rainbow) {
							DrawUtils::drawText(vec2_t(xVal + 33.5f, (yVal + 20.f) + 9.5f), &stackStr, MC_Color(currColor2), 1.f - .20f);
						} else {
							DrawUtils::drawText(vec2_t(xVal + 33.5f, (yVal + 20.f) + 9.5f), &stackStr, MC_Color(Theme::rFloat, Theme::gFloat, Theme::bFloat), 1.f - .20f);
						}
					}
				}
				xVal += 17.f;
			}
			xVal -= 180.f;
			for (int j = 27; j < 36; j++) {
				if ((inv2->getItemStack(j)->item != nullptr)) {
					DrawUtils::drawItem(inv2->getItemStack(j), vec2_t(xVal + 54.f, yVal + 40.f), 1.f, 1.f - .30f, false);
					std::string stackStr = std::to_string(inv2->getItemStack(j)->count);
					if (inv2->getItemStack(j)->count > 1) {
						if (ThemeMod->rainbow) {
							DrawUtils::drawText(vec2_t(xVal + 60.5f, (yVal + 40.f) + 9.5f), &stackStr, MC_Color(currColor2), 1.f - .20f);
						} else {
							DrawUtils::drawText(vec2_t(xVal + 60.5f, (yVal + 40.f) + 9.5f), &stackStr, MC_Color(Theme::rFloat, Theme::gFloat, Theme::bFloat), 1.f - .20f);
						}
					}
				}
				xVal += 17.f;
			}
			xVal -= 180.f;

			if ((mousePosition.x >= invX && mousePosition.x <= invX + 180.f + sideBars) && (mousePosition.y >= invY && mousePosition.y <= invY + 60.f + sideBars)) {
				if (!g_Data.isLeftClickDown()) {
					xDiff2 = mousePosition.x - invX;
					yDiff2 = mousePosition.y - invY;
				}

				if (xDiff2 <= 0.f) {
					xDiff2 = 40.f;
				}

				if (yDiff2 <= 0.f) {
					yDiff2 = 40.f;
				}
			}
		}
	}
}