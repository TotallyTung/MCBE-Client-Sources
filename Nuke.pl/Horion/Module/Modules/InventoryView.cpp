#include "InventoryView.h"
#include "../../DrawUtils.h"
#include "../ModuleManager.h"

InventoryView::InventoryView() : IModule(0, Category::CLIENT, "I'm too lazy for this") {
	registerFloatSetting("PosX", &posX, posX, 0.f, Game.getGuiData()->windowSize.x);
	registerFloatSetting("PosY", &posY, posY, 0.f, Game.getGuiData()->windowSize.y);
	registerIntSetting("Alpha", &alpha, alpha, 0, 255);
	registerBoolSetting("Outline", &outline, outline);
}

const char* InventoryView::getModuleName() {
	return "InvViewer";
}

bool InventoryView::isVisible() {
	return false;
}

void InventoryView::onPostRender(MinecraftUIRenderContext* renderCtx) {
	LocalPlayer* localPlayer = Game.getLocalPlayer();
	if (localPlayer == nullptr) return;
	if (!Game.canUseMoveKeys()) return;

	static Colors* colorsMod = moduleMgr->getModule<Colors>();
	UIColor color = colorsMod->getColor();
	int x = posX;
	int y = posY;
	static const float itemScale = 1.f;
	static const float itemSize = 16.f * itemScale;
	static const float padding = 1.f;
	Vec4 rectPos = Vec4(x - padding, y - padding, x + itemSize * 9.f + padding, y + itemSize * 3.f + padding);
	DrawUtils::fillRectangle(rectPos, MC_Color(0, 0, 0), (float)alpha / 255.f);
	if (outline) DrawUtils::drawRectangle(rectPos, MC_Color(color.r, color.g, color.b), 1.f, 0.5f);

	Inventory* inv = Game.getLocalPlayer()->getSupplies()->inventory;
	for (int i = 9; i < 36; i++) {
		ItemStack* stack = inv->getItemStack(i);
		if (stack->isValid()) {
			DrawUtils::drawItem(stack, Vec2(x, y), itemScale);
			if (stack->count > 1) {
				std::string text = std::to_string(stack->count);
				DrawUtils::drawCenteredText(Vec2(x + 11.f, y + 12.5f), text, 1.f, 1.f);
			}
		}
		x += itemSize;
		if ((i + 1) % 9 == 0) {
			x -= itemSize * 9.f;
			y += itemSize;
		}
	}
}