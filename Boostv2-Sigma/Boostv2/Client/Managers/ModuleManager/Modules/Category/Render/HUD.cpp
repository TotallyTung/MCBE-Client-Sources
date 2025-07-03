#include "HUD.h"
#include "../../../ModuleManager.h"
bool inventoryHUD = false;
Vec2<float> invPos;
float size;
float size1;
Vec2<float> armourPos;
Vec2<float> totPos = Vec2<float>(-69, -420);
bool totemHUD;
bool showFPS = true;
bool showPing = true;
float fps = 0.f;
int frameCount = 0;
std::chrono::steady_clock::time_point lastFPSUpdate = std::chrono::steady_clock::now();
long long lastPingSentTime = 0;
int fakePing = 42;
HUD::HUD() : Module("HUD", "Displays HUD Elements", Category::CLIENT) {
	registerSetting(new BoolSetting("Position", "NULL", &showPosition, true));
	registerSetting(new BoolSetting("Direction", "NULL", &showDirection, true));
	registerSetting(new BoolSetting("Effects", "NULL", &showEffects, true));
	registerSetting(new BoolSetting("FPS", "Show FPS", &showFPS, true));
	registerSetting(new BoolSetting("Ping", "Show Ping", &showPing, true));
	registerSetting(new BoolSetting("ArmourHUD", "NULL", &showArmor, false));
	registerSetting(new BoolSetting("InventoryHUD", "NULL", &inventoryHUD, false));
	registerSetting(new SliderSetting<int>("Opacity", "NULL", &opacity, 130, 0, 255));
	registerSetting(new SliderSetting<int>("Offset", "NULL", &offset, 10, 0, 30));
	registerSetting(new SliderSetting<float>("InvPos.X", "NULL", &invPos.x, invPos.x, 0.f, 500.f));
	registerSetting(new SliderSetting<float>("InvPos.Y", "NULL", &invPos.y, invPos.y, 0.f, 500.f));
	registerSetting(new SliderSetting<float>("InvSize", "NULL", &size, size, 0.5f, 1.f));
	registerSetting(new SliderSetting<float>("ArmourPos.X", "NULL", &armourPos.x, armourPos.x, 0.f, 500.f));
	registerSetting(new SliderSetting<float>("ArmourPos.Y", "NULL", &armourPos.y, armourPos.y, 0.f, 500.f));
	registerSetting(new SliderSetting<float>("ArmourHUDSize", "NULL", &size1, size1, 0.5f, 1.f));
}

HUD::~HUD() {
}
void HUD::onMCRender(MinecraftUIRenderContext* ctx) {
	if (Game.getLocalPlayer() == nullptr) return;


	Vec2<float> windowSizeScaled = Game.clientInstance->guiData->windowSizeScaled;
	if (showArmor) {
		float scale = size1;
		float opacity = 1.f;
		float basePadding = 1.f;                        
		float spacing = (16.0f + basePadding) * scale;   

		Vec2<float> armorHudPos = armourPos;
		ItemStack* armorItemStack[4] = { nullptr, nullptr, nullptr, nullptr };

		for (int i = 0; i < 4; i++) {
			ItemStack* stack = Game.getLocalPlayer()->getArmor(i);
			if (stack->isValid()) {
				armorItemStack[i] = stack;
				armorHudPos.y -= spacing / 2.0f;
			}
		}

		for (int i = 0; i < 4; i++) {
			if (armorItemStack[i] != nullptr) {
				MCR::drawItem(armorItemStack[i], armorHudPos, opacity, scale);
				MCR::drawItemDurability(armorItemStack[i], armorHudPos, scale, opacity, 0);
				armorHudPos.y += spacing;
			}
		}
	}


	if (inventoryHUD) {
		const float itemScale = size;
		const float itemSize = 16.f * itemScale;
		const float padding = 2.f;

		float xStart = invPos.x;
		float yStart = invPos.y;
		float x = xStart;
		float y = yStart;

		Vec4<float> invRect = Vec4<float>(x - padding, y - padding, x + itemSize * 9.f + padding, y + itemSize * 3.f + padding);

		MCR::fillRectangle(invRect, UIColor(0.f, 0.f, 0.f, 125.f));
		Container* inv = Game.getLocalPlayer()->getplayerInventory()->container;
	
		const float border = 0.5f;
		UIColor blue = ModuleManager::getModule<Colors>()->getColor();

		MCR::fillRectangle(Vec4<float>(
			invRect.x, invRect.y,
			invRect.z, invRect.y + border
		), blue);

		MCR::fillRectangle(Vec4<float>(
			invRect.x, invRect.w - border,
			invRect.z, invRect.w
		), blue);

		MCR::fillRectangle(Vec4<float>(
			invRect.x, invRect.y + border,
			invRect.x + border, invRect.w - border
		), blue);
		MCR::fillRectangle(Vec4<float>(
			invRect.z - border, invRect.y + border,
			invRect.z, invRect.w - border
		), blue);

		for (int i = 9; i < 36; i++) {
			ItemStack* stack = inv->getItem(i);
			if (stack->isValid()) {
				MCR::drawItem(stack, Vec2<float>(x, y), 1.f, itemScale);
				MCR::drawItemDurability(stack, Vec2<float>(x, y), itemScale, 1.f, 0);

				if (stack->mCount > 1) {
					float centerX = x + itemSize / 2.f;
					float centerY = y + itemSize / 2.f;
					float textWidth = (stack->mCount >= 10 ? 8.f : 6.f) * itemScale;
					float textHeight = 8.f * itemScale;

					MCR::drawText(
						Vec2<float>(centerX - textWidth / 2.f, centerY - textHeight / 2.f),
						std::to_string(stack->mCount),
						UIColor(255, 255, 255, 255),
						itemScale
					);
				}
			}

			x += itemSize;
			if ((i + 1) % 9 == 0) {
				x = xStart;
				y += itemSize;
			}
		}
	}
}

void HUD::onD2DRender() {
	LocalPlayer* localPlayer = Game.getLocalPlayer();
	if (!localPlayer) return;

	frameCount++;
	auto now = std::chrono::steady_clock::now();
	float deltaTime = std::chrono::duration<float>(now - lastFPSUpdate).count();
	if (deltaTime >= 1.0f) {
		fps = (float)frameCount / deltaTime;
		frameCount = 0;
		lastFPSUpdate = now;
	}

	long long nowMillis = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::steady_clock::now().time_since_epoch()
	).count();
	if (nowMillis - lastPingSentTime >= 1000) {
		fakePing = 40 + rand() % 30;
		lastPingSentTime = nowMillis;
	}

	Vec2<float> windowSize = Game.clientInstance->guiData->windowSizeReal;
	float textSize = 1.f;
	float textPaddingX = std::max(1.f, ((float)spacing + 3.f)) * textSize;
	float textPaddingY = (float)spacing * textSize;
	float textHeight = D2D::getTextHeight("", textSize);
	float posX = (float)offset;

	std::vector<std::string> lines;

	if (showFPS) {
		char buf[32];
		sprintf_s(buf, "FPS: %d", (int)fps);
		lines.push_back(buf);
	}
	if (showPing) {
		char buf[32];
		sprintf_s(buf, "Ping: %d ms", fakePing);
		lines.push_back(buf);
	}
	if (showDirection) {
		float rot = localPlayer->rotation->presentRot.y;
		if (rot < 0) rot += 360.f;
		std::string dir;
		if (rot < 45 || rot >= 315) dir = "South (+Z)";
		else if (rot < 135) dir = "West (-X)";
		else if (rot < 225) dir = "North (-Z)";
		else dir = "East (+X)";
		lines.push_back("Direction: " + dir);
	}
	if (showPosition) {
		Vec3<float> pos = localPlayer->getPos();
		int dim = localPlayer->getDimensionTypeComponent()->type;

		char buf[64];
		if (dim == 0) {
			sprintf_s(buf, "Nether: %.1f, %.1f, %.1f", pos.x / 8.f, pos.y - 1.6f, pos.z / 8.f);
			lines.push_back(buf);
		}
		else if (dim == 1) {
			sprintf_s(buf, "Overworld: %.1f, %.1f, %.1f", pos.x * 8.f, pos.y - 1.6f, pos.z * 8.f);
			lines.push_back(buf);
		}
		sprintf_s(buf, "Position: %.1f, %.1f, %.1f", pos.x, pos.y - 1.6f, pos.z);
		lines.push_back(buf);
	}

	float totalHeight = lines.size() * (textHeight + textPaddingY * 2.f);
	float posY = windowSize.y - offset - totalHeight;

	for (const std::string& str : lines) {
		float textWidth = D2D::getTextWidth(str, textSize);
		Vec4<float> box = Vec4<float>(
			posX,
			posY,
			posX + textWidth + textPaddingX * 2.f,
			posY + textHeight + textPaddingY * 2.f
		);
		D2D::fillRectangle(box, UIColor(0, 0, 0, opacity));
		D2D::drawText(Vec2<float>(posX + textPaddingX, posY + textPaddingY), str, UIColor(255, 255, 255), textSize);
		posY += textHeight + textPaddingY * 2.f;
	}
}
