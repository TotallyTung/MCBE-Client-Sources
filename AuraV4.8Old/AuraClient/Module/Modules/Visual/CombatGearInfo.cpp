#include "CombatGearInfo.h"

#include "../../Utils/DrawUtils.h"
#include "../../ModuleManager.h"
//#include "Theme.h"

pvpResources::pvpResources() : IModule(0x0, Category::VISUAL, "Displays items on screen that are used for PVP") {
	//registerBoolSetting("Textual", &this->text, this->text);
	registerFloatSetting("Display on X", &this->pvpX, this->pvpX, 0.f, g_Data.getClientInstance()->getGuiData()->windowSize.x);
	registerFloatSetting("Display on Y", &this->pvpY, this->pvpY, 0.f, g_Data.getClientInstance()->getGuiData()->windowSize.y);
	registerFloatSetting("Scale", &this->scale, this->scale, 0.5f, 1.5f);
}

pvpResources::~pvpResources() {
}

const char* pvpResources::getModuleName() {
	return ("CombatGearInfo");
}

float xDif = -1.f;
float yDif = -1.f;

void pvpResources::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
	float testValYe = 40.f * scale;
	float lineWidth2 = 1.f * scale;
	vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	vec2_t windowSizeReal = g_Data.getClientInstance()->getGuiData()->windowSizeReal;
	vec2_t mousePosition = *g_Data.getClientInstance()->getMousePos();
	mousePosition = mousePosition.div(windowSizeReal);
	mousePosition = mousePosition.mul(windowSize);

	if (!text) {
		static float rcolors[4];
		//static auto ThemeMod = moduleMgr->getModule<Theme>();
		//if (ThemeMod->rainbow) {
		if (rcolors[3] < 1) {
			rcolors[0] = 1;
			rcolors[1] = 0.2f;
			rcolors[2] = 0.2f;
			rcolors[3] = 1;
			//}

			Utils::ColorConvertRGBtoHSV(rcolors[0], rcolors[1], rcolors[2], rcolors[0], rcolors[1], rcolors[2]);  // perfect code, dont question this

			rcolors[0] += 0.0015f;
			if (rcolors[0] >= 1) {
				rcolors[0] = 0;
			}

			Utils::ColorConvertHSVtoRGB(rcolors[0], rcolors[1], rcolors[2], rcolors[0], rcolors[1], rcolors[2]);
			DrawUtils::setColor(rcolors[0], rcolors[1], rcolors[2], 1.f);
		}
		else {
			DrawUtils::setColor(1.f, 1.f, 1.f, 1.f);
		}
		DrawUtils::drawRectangle(vec2_t(pvpX, pvpY), vec2_t(pvpX + (testValYe), pvpY + testValYe), lineWidth2);
		vec2_t centerPoint = vec2_t(pvpX + testValYe / 2, pvpY + testValYe / 2);
		DrawUtils::fillRectangle(vec2_t(centerPoint.x - lineWidth2 / 2, centerPoint.y - testValYe / 2), vec2_t(centerPoint.x + lineWidth2 / 2, centerPoint.y + testValYe / 2));
		DrawUtils::fillRectangle(vec2_t(centerPoint.x - testValYe / 2, centerPoint.y - lineWidth2 / 2), vec2_t(centerPoint.x + testValYe / 2, centerPoint.y + lineWidth2 / 2));

		if (isThereCrystal()) {
			int gSlot = pullSlot(637);
			DrawUtils::drawItem(g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(gSlot), vec2_t(pvpX + (lineWidth2 * scale + 1.f) + testValYe / 2, pvpY + (lineWidth2 * scale - 1.f + testValYe / 2)), 1.f, scale, false);
			std::string yikes = std::to_string(countItem(637));
			DrawUtils::drawText(vec2_t(pvpX + (lineWidth2 * scale + 4.f) + testValYe / 2, pvpY + (lineWidth2 * scale + 7.f) + testValYe / 2), &yikes, MC_Color(255, 255, 255), scale);
		}

		if (isThereTotem()) {
			int gSlot = pullSlot(568);
			DrawUtils::drawItem(g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(gSlot), vec2_t(pvpX + (lineWidth2 * scale + 1.f), pvpY + (lineWidth2 * scale - 1.f + testValYe / 2)), 1.f, scale, false);
			std::string yikes = std::to_string(countItem(568));
			DrawUtils::drawText(vec2_t(pvpX + (lineWidth2 * scale + 4.f), pvpY + (lineWidth2 * scale + 7.f) + 20.f), &yikes, MC_Color(255, 255, 255), scale);
		}

		if (isThereGapples()) {
			int gSlot = pullSlot(259);
			DrawUtils::drawItem(g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(gSlot), vec2_t(pvpX + (lineWidth2 * scale + 1.f), pvpY + (lineWidth2 * scale - 1.f)), 1.f, scale, false);
			std::string yikes = std::to_string(countItem(259));
			DrawUtils::drawText(vec2_t(pvpX + (lineWidth2 * scale + 4.f), pvpY + (lineWidth2 * scale + 7.f)), &yikes, MC_Color(255, 255, 255), scale);
		}

		if (isThereStrength()) {
			int gSlot = pullSlot(426);
			DrawUtils::drawItem(g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(gSlot), vec2_t(pvpX + (lineWidth2 * scale + 1.f) + testValYe / 2, pvpY + (lineWidth2 * scale - 1.f)), 1.f, scale, false);
			std::string yikes = std::to_string(countItem(426));
			DrawUtils::drawText(vec2_t(pvpX + (lineWidth2 * scale + 4.f) + testValYe / 2, pvpY + (lineWidth2 * scale + 7.f)), &yikes, MC_Color(255, 255, 255), scale);
		}
		if (isThereObsidian()) {
			int gSlot = pullSlot(49);
			DrawUtils::drawItem(g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(gSlot), vec2_t(pvpX + (lineWidth2 * scale + 1.f), pvpY + (lineWidth2 * scale - 1.f + testValYe / 2)), 1.f, scale, false);
			std::string yikes = std::to_string(countItem(49));
			DrawUtils::drawText(vec2_t(pvpX + (lineWidth2 * scale + 4.f), pvpY + (lineWidth2 * scale + 7.f) + 20.f), &yikes, MC_Color(255, 255, 255), scale);
		}

	}
	else {
		//strength
		std::string strength = std::to_string(countItem(426));
		DrawUtils::drawText(vec2_t(pvpX, pvpY), &std::string("Potions: " + strength), MC_Color(160, 255, 0), scale);
		//totems
		std::string totems = std::to_string(countItem(568));
		DrawUtils::drawText(vec2_t(pvpX, pvpY + 10.f * scale), &std::string("Totems: " + totems), MC_Color(255, 255, 130), scale);
		//Gapples
		std::string gapples = std::to_string(countItem(259));
		DrawUtils::drawText(vec2_t(pvpX, pvpY + 20.f * scale), &std::string("Gapples: " + gapples), MC_Color(200, 200, 0), scale);
		//Crystals
		std::string crystals = std::to_string(countItem(637));
		DrawUtils::drawText(vec2_t(pvpX, pvpY + 30.f * scale), &std::string("Crystals: " + crystals), MC_Color(215, 0, 235), scale);
		//Obsidians
		std::string obsidians = std::to_string(countItem(49));
		DrawUtils::drawText(vec2_t(pvpX, pvpY + 40.f * scale), &std::string("Obsidian: " + obsidians), MC_Color(0.1f, 0.1f, 0.1f), scale);
	}
}

bool pvpResources::isThereTotem() {
	int mafs = 0;
	for (int i = 0; i < 36; i++) {
		int itemIDC = 568;
		C_ItemStack* itemToCount = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
		if (itemToCount->item != NULL && (*itemToCount->item)->itemId == itemIDC) {
			int itemCountC = itemToCount->count;
			mafs += itemCountC;
		}
	}
	if (mafs == 0) {
		return false;
	}
	else {
		return true;
	}
}

bool pvpResources::isThereObsidian() {
	int mafs = 0;
	for (int i = 0; i < 36; i++) {
		int itemIDC = 49;
		C_ItemStack* itemToCount = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
		if (itemToCount->item != NULL && (*itemToCount->item)->itemId == itemIDC) {
			int itemCountC = itemToCount->count;
			mafs += itemCountC;
		}
	}
	if (mafs == 0) {
		return false;
	}
	else {
		return true;
	}
}

bool pvpResources::isThereCrystal() {
	int mafs = 0;
	for (int i = 0; i < 36; i++) {
		int itemIDC = 637;
		C_ItemStack* itemToCount = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
		if (itemToCount->item != NULL && (*itemToCount->item)->itemId == itemIDC) {
			int itemCountC = itemToCount->count;
			mafs += itemCountC;
		}
	}
	if (mafs == 0) {
		return false;
	}
	else {
		return true;
	}
}

bool pvpResources::isThereGapples() {
	int mafs = 0;
	for (int i = 0; i < 36; i++) {
		int itemIDC = 259;
		C_ItemStack* itemToCount = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
		if (itemToCount->item != NULL && (*itemToCount->item)->itemId == itemIDC) {
			int itemCountC = itemToCount->count;
			mafs += itemCountC;
		}
	}
	if (mafs == 0) {
		return false;
	}
	else {
		return true;
	}
}

bool pvpResources::isThereStrength() {
	int mafs = 0;
	for (int i = 0; i < 36; i++) {
		int itemIDC = 426;
		C_ItemStack* itemToCount = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
		if (itemToCount->item != NULL && (*itemToCount->item)->itemId == itemIDC) {
			int itemCountC = itemToCount->count;
			mafs += itemCountC;
		}
	}
	if (mafs == 0) {
		return false;
	}
	else {
		return true;
	}
}

int pvpResources::countItem(int itemIDCount) {
	int mafs = 0;
	for (int i = 0; i < 36; i++) {
		C_ItemStack* itemToCount = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
		if (itemToCount->item != NULL && (*itemToCount->item)->itemId == itemIDCount) {
			int itemCountC = itemToCount->count;
			mafs += itemCountC;
		}
	}
	return mafs;
	//mafs = 0;
}

int pvpResources::pullSlot(int itemIDPog) {
	int mafs = 0;
	for (int i = 0; i < 36; i++) {
		C_ItemStack* itemToCount = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
		if (itemToCount->item != NULL && (*itemToCount->item)->itemId == itemIDPog) {
			return i;
		}
	}
}
