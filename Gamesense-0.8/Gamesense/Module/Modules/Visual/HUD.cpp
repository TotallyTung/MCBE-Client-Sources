#include "HUD.h"
#include "../../../../Utils/Logger.h"
#include "../../Utils/DrawUtils.h"
#include "../../Utils/ImGuiUtils.h"
#include "../../ModuleManager.h"

using namespace std;

// Too lazy to recompile client moment
bool hideOnGUI;
bool showPercent = true;
bool showDuraBar = true;
bool directionView;
// ...

HUD::HUD() : IModule(0x0, Category::OTHER, "Renders the HUD")
{
	registerBoolSetting("ArmorHUD", &armorHUD, armorHUD, "Shows what armor you have and its durability");
	registerBoolSetting("InventoryHUD", &inv, inv, "Shows your inventory");
	registerBoolSetting("TotemCounter", &totemc, totemc, "Shows how many totems you have left");
	registerBoolSetting("Compass", &compass, compass, "Shows a big compass");
	registerBoolSetting("Frames", &fps, fps, "Shows your frames per second");
	registerBoolSetting("Speed", &bps, bps, "Shows your blocks per second");
	registerBoolSetting("Position", &xyz, xyz, "Shows where you are");
	registerBoolSetting("Durability", &showPercent, showPercent, "Shows a armor and item durability");
	//registerBoolSetting("Yaw", &directionView, directionView, "Shows yaw in Position");
	registerBoolSetting("InventoryOutline", &this->outline, this->outline, "Should render outline on inventory");
	// registerBoolSetting("Hide on GUI", &hideOnGUI, hideOnGUI); broken AND lazy

	registerFloatSetting("Inventory X", &this->invX, this->invX, 0.f, g_Data.getClientInstance()->getGuiData()->windowSize.x, 0.1f, "Position of Inventory view(X)");
	registerFloatSetting("Inventory Y", &this->invY, this->invY, 0.f, g_Data.getClientInstance()->getGuiData()->windowSize.y, 0.1f, "Position of Inventory view(Y)");

	registerFloatSetting("Opacity", &this->opa, this->opa, 0.f, 1.f, 0.1f, "Opacity");
	TextMode.addEntry("None", 0);
	TextMode.addEntry("Normal", 1);
	TextMode.addEntry("Gamesense", 2);
	TextMode.addEntry("Homie", 3);
	TextMode.addEntry("EgoBoost", 4);
	registerEnumSetting("Welcomer", &TextMode, 0, "Welcomes you at the top of the screen");
	registerIntSetting("Size", &size, size, 15, 40, "Changes size");
	shouldHide = true;
}

// TODO: Remove the FPS and BPS position setter and put it on bottom right
// TODO: Add fucking effects viewier!

HUD::~HUD()
{
}

const char *HUD::getRawModuleName()
{
	return ("HUD");
}
struct CompareTargetEnArray
{
	bool operator()(C_Entity *lhs, C_Entity *rhs)
	{
		C_LocalPlayer *localPlayer = g_Data.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};
void HUD::onEnable()
{
}

void HUD::onImGuiRender(ImDrawList* d) {
	auto player = g_Data.getLocalPlayer();
	auto clickGUI = moduleMgr->getModule<ClickGUIMod>();
	if (g_Data.getLocalPlayer() == nullptr) return;
	if (!GameData::canUseMoveKeys())
		return;
	if (clickGUI->hasOpenedGUI)
		return;
	float textSize2 = (float)size;
	windowSize = g_Data.getClientInstance()->getGuiData()->windowSizeReal;
	float posY = windowSize.y - 35.f;
	float textHeight = ImGuiUtils::getTextHeight(d, "A", textSize2);
	if (fps) {
		ImGuiUtils::renderGradientText(d, ImVec2(5.f, posY), "FPS", textSize2);
		float textWidth = ImGuiUtils::getTextWidth(d, "FPS ", textSize2);
		int fpsCount = int(ImGui::GetIO().Framerate);
		ImGuiUtils::renderText(d, ImVec2(5.f + textWidth, posY), std::to_string(fpsCount).c_str(), ImColor(255, 255, 255, 255), textSize2);
		posY -= textHeight;
	}
	if (bps) {
		ImGuiUtils::renderGradientText(d, ImVec2(5.f, posY), "BPS", textSize2);
		float textWidth = ImGuiUtils::getTextWidth(d, "BPS ", textSize2);
		std::string bpsText = std::to_string((int)player->getBlocksPerSecond()) + std::string(".") + std::to_string((int)(player->getBlocksPerSecond() * 10) - ((int)player->getBlocksPerSecond() * 10)) + "m/s";
		ImGuiUtils::renderText(d, ImVec2(5.f + textWidth, posY), bpsText.c_str(), ImColor(255, 255, 255, 255), textSize2);
		posY -= textHeight;
	}
	if (xyz) {
		vec3_t* currPos = g_Data.getLocalPlayer()->getPos();
		int dimension = 0;
		player->getDimensionId(&dimension);
		std::string posTextXYZ = "XYZ: ";
		// Current coords
		std::string coordX = std::to_string((int)floorf(currPos->x + 0.001));
		std::string coordY = std::to_string((int)floorf(currPos->y + 0.001));
		std::string coordZ = std::to_string((int)floorf(currPos->z + 0.001));

		// Nether coords
		std::string coordXN = std::to_string((int)floorf(currPos->x + 0.001) / 8);
		std::string coordZN = std::to_string((int)floorf(currPos->z + 0.001) / 8);

		// Overworld coords
		std::string coordXO = std::to_string((int)floorf(currPos->x + 0.001) * 8);
		std::string coordZO = std::to_string((int)floorf(currPos->z + 0.001) * 8);

		ImGuiUtils::renderGradientText(d, ImVec2(5.f, posY), "XYZ", textSize2);
		float textWidth = ImGuiUtils::getTextWidth(d, "XYZ ", textSize2);
		std::string netherText = (coordX + ", " + coordY + ", " + coordZ + "[" + coordXO + ", " + coordZO + "]").c_str();
		std::string owText = (coordX + ", " + coordY + ", " + coordZ + "[" + coordXN + ", " + coordZN + "]").c_str();

		// Overworld and End
		if (dimension == 0 || dimension == 2) {
			ImGuiUtils::renderText(d, ImVec2(5.f + textWidth, posY), owText.c_str(), ImColor(255, 255, 255, 255), textSize2);
		}
		// Nether
		if (dimension == 1) {
			ImGuiUtils::renderText(d, ImVec2(5.f + textWidth, posY), netherText.c_str(), ImColor(255, 255, 255, 255), textSize2);
		}
	}
	if (TextMode.getSelectedValue() != 0) {
		vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
		static float rcolors[4];
		float nameTextSize = 1.f + 0.5f;
		static const float textHeight = (nameTextSize * 0.7f) * DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
		constexpr float borderPadding = 1;
		constexpr float margin = 5;
		std::string playerName = g_Data.getLocalPlayer()->getNameTag()->getText();

		switch (TextMode.getSelectedValue())
		{
		case 1:
			name = "Welcome " + playerName;
			break;

		case 2:
			name = "Welcome to Gamesense, " + playerName;
			break;

		case 3:
			name = "Sup Homie " + playerName;
			break;

		case 4:
			name = "Hello " + playerName + ", You smell good today ;)";
			break;
		}
		float nameLength = ImGuiUtils::getTextWidth(d, name.c_str(), textSize2);
		float x = windowSize.x - windowSize.x / 2 - nameLength / 2;
		float y = windowSize.y - windowSize.y;
		float texthai = ImGuiUtils::getTextHeight(d, "", textSize2);
		ImGuiUtils::renderGradientText(d, ImVec2(windowSize.x / 2, windowSize.y - windowSize.y + 7.5), name.c_str(), textSize2);
	}
}
void HUD::onPreRender(C_MinecraftUIRenderContext* renderCtx)
{
}
void HUD::onPostRender(C_MinecraftUIRenderContext *renderCtx)
{
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr)
		return;
	float testValYe = 40.f * 1.f;
	float lineWidth2 = 1.f * 1.f;
	auto clickGUI = moduleMgr->getModule<ClickGUIMod>();
	bool wlc = true;

	static int index = 0;
	windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	if (!clickGUI->hasOpenedGUI)
	{
		if (armorHUD) {
			static float constexpr scale = 1.f;
			static float constexpr opacity = 0.25f;
			static float constexpr spacing = scale + 20.f;
			C_LocalPlayer* player = g_Data.getLocalPlayer();
			float x = windowSize.x / 2.f + 9.5f;
			float y = windowSize.y - 57.5f;
			if (player->isInWater() || player->isInWall()) y -= 10.f;
			for (int i = 0; i < 4; i++) {
				C_ItemStack* stack = player->getArmor(i);
				if (stack->isValid()) {
					float dura1 = stack->getItem()->getMaxDamage();
					float dura2 = stack->getItem()->getDamageValue(stack->tag);
					float dura3 = dura1 - dura2;
					int dura4 = dura3 / dura1 * 100;
					std::string durastr = std::to_string((int)dura4);
					MC_Color green(0, 255, 0);
					MC_Color red(255, 0, 0);
					float dura5 = dura3 / dura1 * 100;
					float fraction = dura5 / 100;
					vec3_t greenVec(0, 0, 0);
					vec3_t redVec(0, 0, 0);
					Utils::ColorConvertRGBtoHSV(green.r, green.g, green.b, greenVec.x, greenVec.y, greenVec.z);
					Utils::ColorConvertRGBtoHSV(red.r, red.g, red.b, redVec.x, redVec.y, redVec.z);
					vec3_t colorVec = redVec.lerp(greenVec, fraction, fraction, fraction);
					MC_Color gamerColor(0, 0, 0);

					Utils::ColorConvertHSVtoRGB(colorVec.x, colorVec.y, colorVec.z, gamerColor.r, gamerColor.g, gamerColor.b);
					if (showPercent) {
						DrawUtils::drawCenteredString(vec2_t(x + 8.f, y - 2.5f), &durastr, 0.9f, MC_Color(gamerColor), true);
					}
					DrawUtils::drawItem(stack, vec2_t(x, y), opacity, scale, stack->isEnchanted());
					if (showDuraBar)
					DrawUtils::DrawBarItem(stack, vec2_t(x, y), 1.f);
					x += scale * spacing;
				}
			}
		}
		if (totemc)
		{
			pvpX = windowSize.x / 2.05;
			pvpY = windowSize.y - 75.f;
			if (isThereTotem())
			{
				int gSlot = pullSlot(568);
				std::string yikes = std::to_string(countItem(568));
				DrawUtils::drawItem(g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(gSlot), vec2_t(pvpX + (lineWidth2 * 1.f + 1.f), pvpY + (lineWidth2 * 1.f - 1.f + testValYe / 2)), 1.f, 1.f, false);
				DrawUtils::drawText(vec2_t(pvpX + (lineWidth2 * 1.f + 4.f), pvpY + (lineWidth2 * 1.f + 7.f) + 20.f), &yikes, MC_Color(255, 255, 255), 1.f, 1.f, true);
    		}
		}
		if (inv)
		{
			vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
			vec2_t windowSizeReal = g_Data.getClientInstance()->getGuiData()->windowSizeReal;
			auto clickGUI = moduleMgr->getModule<ClickGUIMod>();
			if (!clickGUI->hasOpenedGUI)
			{
				if (g_Data.canUseMoveKeys())
					if (!(g_Data.getLocalPlayer() == nullptr || !GameData::canUseMoveKeys()))
					{
						C_LocalPlayer *player = g_Data.getLocalPlayer();
						C_PlayerInventoryProxy *supplies2 = g_Data.getLocalPlayer()->getSupplies();
						C_Inventory *inv2 = supplies2->inventory;
						g_Data.getLocalPlayer()->getSupplies()->inventory;
						vec2_t invPos = vec2_t(100.f, 100.f);
						vec2_t invPos2 = vec2_t(100.f, 120.f);
						C_ItemStack *test = inv2->getItemStack(1);
						C_ItemStack *test2 = inv2->getItemStack(2);
						float testF = 100.f;
						float sideBars = 2.5f;
						float yVal = invY + sideBars;

						xVal = invX + sideBars;

						auto interfaceColor = ColorUtil::interfaceColor(1);
						vec4_t breh = vec4_t(invX, invY, invX + 153.f + sideBars + 2, invY + 58.f + sideBars);
						if (outline)
						{
							DrawUtils::drawRectangle(vec4_t(breh.x, breh.y, breh.z, breh.w), MC_Color(interfaceColor), 1.f, sideBars / 2);
						}
						DrawUtils::fillRectangle(breh, MC_Color(0.f, 0.f, 0.f), opa);
						for (int j = 9; j < 18; j++)
						{
							if ((inv2->getItemStack(j)->item != nullptr))
							{
								DrawUtils::drawItem(inv2->getItemStack(j), vec2_t(xVal, yVal + 0.f), 1.f, 1.f, false);
								char stackAmount = inv2->getItemStack(j)->count;
								std::string stackStr = std::to_string(inv2->getItemStack(j)->count);
								if (inv2->getItemStack(j)->count > 1)
									DrawUtils::drawText(vec2_t(xVal + 7.f, (yVal + 0.f) + 10.f), &stackStr, MC_Color(255, 255, 255), 1.f - .20f, 1.f, true);
							}
							xVal += 17.f;
						}
						xVal -= 180.f;
						for (int j = 18; j < 27; j++)
						{
							if ((inv2->getItemStack(j)->item != nullptr))
							{
								DrawUtils::drawItem(inv2->getItemStack(j), vec2_t(xVal + 27.f, yVal + 20.f), 1.f, 1.f, false);
								std::string stackStr = std::to_string(inv2->getItemStack(j)->count);
								if (inv2->getItemStack(j)->count > 1)
									DrawUtils::drawText(vec2_t(xVal + 34.f, (yVal + 20.f) + 10.f), &stackStr, MC_Color(255, 255, 255), 1.f - .20f, 1.f, true);
							}
							xVal += 17.f;
						}
						xVal -= 180.f;
						for (int j = 27; j < 36; j++)
						{
							if ((inv2->getItemStack(j)->item != nullptr))
							{
								DrawUtils::drawItem(inv2->getItemStack(j), vec2_t(xVal + 54.f, yVal + 40.f), 1.f, 1.f, false);
								std::string stackStr = std::to_string(inv2->getItemStack(j)->count);
								if (inv2->getItemStack(j)->count > 1)
									DrawUtils::drawText(vec2_t(xVal + 61.f, (yVal + 40.f) + 10.f), &stackStr, MC_Color(255, 255, 255), 1.f - .20f, 1.f, true);
							}
							xVal += 17.f;
						}
						xVal -= 180.f;
					}
			}
		}
		// make me a fucking sandwich hydro
		if (compass)
		{
			C_LocalPlayer *player = g_Data.getLocalPlayer();

			if (player == nullptr || !GameData::canUseMoveKeys())
				return;

			auto extraPoints = std::multimap<int, std::string>{};

			std::vector<vec2_t> stacking{};

			const int deg = (int)(player->yaw + 180);
			const float degSubOffset = 0; // -fmodf(player->yaw, 1)
			const float sCenter = g_Data.getGuiData()->widthGame / 2;

			for (int off = -range; off <= range; off++)
			{
				int oDeg = (deg + off) % 360;
				if (oDeg < 0)
					oDeg += 360;

				const float xOff = sCenter + off + degSubOffset;
				constexpr float fadeOutPos = 0.25f;
				constexpr float centerCutoff = 5, centerFadeout = 10;
				float opacity = opa;
				float majorOpacity = opacity;	   // big compass text
				float minorOpacity = majorOpacity; // minor features, degree numbers

				// Fading logic
				{
					if ((range - abs(off)) < range * fadeOutPos) // Far from center
						minorOpacity = majorOpacity = lerp(0, opacity, (range - abs(off)) / (range * fadeOutPos));
					else if (abs(off) < centerFadeout) // Close to center
						minorOpacity = lerp(0, opacity, (abs(off) - centerCutoff) / (centerFadeout - centerCutoff));
				}

				switch (oDeg)
				{
				case 0:
					drawCenteredText(vec2_t(xOff, 30.f), "N", 1, majorOpacity);
					break;
				case 45:
					drawCenteredText(vec2_t(xOff, 30.f), "NE", 1, majorOpacity);
					break;
				case 90:
					drawCenteredText(vec2_t(xOff, 30.f), "E", 1, majorOpacity);
					break;
				case 135:
					drawCenteredText(vec2_t(xOff, 30.f), "SE", 1, majorOpacity);
					break;
				case 180:
					drawCenteredText(vec2_t(xOff, 30.f), "S", 1, majorOpacity);
					break;
				case 225:
					drawCenteredText(vec2_t(xOff, 30.f), "SW", 1, majorOpacity);
					break;
				case 270:
					drawCenteredText(vec2_t(xOff, 30.f), "W", 1, majorOpacity);
					break;
				case 315:
					drawCenteredText(vec2_t(xOff, 30.f), "NW", 1, majorOpacity);
					break;
				}
				auto result = extraPoints.equal_range(oDeg);
				for (auto it = result.first; it != result.second; it++)
				{
					std::string pName = it->second;
					std::transform(pName.begin(), pName.end(), pName.begin(), ::toupper);

					vec2_t pos = vec2_t(xOff, 31.f);

					int overlapping = 0;
					const float tSize = 0.75f;
					const float tWidth = DrawUtils::getTextWidth(&pName, tSize);
					pos.x -= tWidth / 2;
					const vec2_t myTextRange = vec2_t(pos.x, pos.x + tWidth);
					for (const vec2_t otherTextRange : stacking)
					{
						// Check if other text overlaps us
						if (myTextRange.x < otherTextRange.y && otherTextRange.x < myTextRange.y)
						{
							overlapping++;
						}
					}
					stacking.push_back(myTextRange);
					pos.y += 5 * (overlapping + 1);
					DrawUtils::drawText(pos, &pName, MC_Color(255, 255, 255), tSize, majorOpacity);
				}

				if (off != 0 && minorOpacity > 0)
				{
					if ((oDeg % 15) != 0)
					{
						if ((oDeg % 5) == 0)
						{
							DrawUtils::fillRectangle(vec4_t(xOff - 0.25f, 16, xOff + 0.25f, 19), MC_Color(255, 255, 255), minorOpacity);
						}
						continue;
					}

					// Bigger line with degree displayed
					DrawUtils::fillRectangle(vec4_t(xOff - 0.5f, 15, xOff + 0.5f, 20), MC_Color(255, 255, 255), minorOpacity);
					drawCenteredText(vec2_t(xOff, 20.f), std::to_string(oDeg), 0.75f, minorOpacity);
				}
			}

			// Center line
			DrawUtils::fillRectangle(vec4_t(sCenter - 0.5f, 15, sCenter + 0.5f, 25), MC_Color(255, 255, 255), opa);
			drawCenteredText(vec2_t(sCenter, 25.f), std::to_string(deg), 0.75f, opa);
			DrawUtils::flush();
		}
	}
}
void HUD::drawCenteredText(vec2_t pos, std::string text, float size, float textOpacity)
{
	pos.x -= DrawUtils::getTextWidth(&text, size) / 2;
	DrawUtils::drawText(pos, &text, MC_Color(255, 255, 255), size, textOpacity);
}
void HUD::onDisable()
{
}

bool HUD::isThereTotem()
{
	int mafs = 0;
	for (int i = 0; i < 36; i++)
	{
		int itemIDC = 568;
		C_ItemStack *itemToCount = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
		if (itemToCount->item != NULL && (*itemToCount->item)->itemId == itemIDC)
		{
			int itemCountC = itemToCount->count;
			mafs += itemCountC;
		}
	}
	if (mafs == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}
int HUD::countItem(int itemIDCount)
{
	int mafs = 0;
	for (int i = 0; i < 36; i++)
	{
		C_ItemStack *itemToCount = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
		if (itemToCount->item != NULL && (*itemToCount->item)->itemId == itemIDCount)
		{
			int itemCountC = itemToCount->count;
			mafs += itemCountC;
		}
	}
	return mafs;
	// mafs = 0;
}

int HUD::pullSlot(int itemIDPog)
{
	int mafs = 0;
	for (int i = 0; i < 36; i++)
	{
		C_ItemStack *itemToCount = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
		if (itemToCount->item != NULL && (*itemToCount->item)->itemId == itemIDPog)
		{
			return i;
		}
	}
}
