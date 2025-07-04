#include "InventoryView.h"

#include "../../../../Utils/Logger.h"
#include "../../../../Utils/DrawUtils.h"

InventoryView::InventoryView() : IModule(0x0, Category::VISUAL, "renders inventory on screen") {
	registerFloatSetting("invPosX", &this->invX, this->invX, 0.f, g_Data.getClientInstance()->getGuiData()->windowSize.x);
	registerFloatSetting("invPosY", &this->invY, this->invY, 0.f, g_Data.getClientInstance()->getGuiData()->windowSize.y);
	registerFloatSetting("Opacity", &this->opa, this->opa, 0.f, 1.f);
	registerBoolSetting("Outline", &this->outline, this->outline);
}

InventoryView::~InventoryView() {
}

const char* InventoryView::getModuleName() {
	return ("InventoryView");
}

void InventoryView::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	vec2_t windowSizeReal = g_Data.getClientInstance()->getGuiData()->windowSizeReal;
	auto clickGUI = moduleMgr->getModule<ClickGUIMod>();
	if (g_Data.canUseMoveKeys() && !clickGUI->hasOpenedGUI) 
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
			float sideBars = 2.5f;
			float yVal = invY + sideBars;

			xVal = invX + sideBars;

			auto interfaceColor = ColorUtil::interfaceColor(1);
			vec4_t breh = vec4_t(invX, invY, invX + 153.f + sideBars + 2, invY + 58.f + sideBars);
			if (outline) {
				DrawUtils::drawRectangle(vec4_t(breh.x ,breh.y ,breh.z ,breh.w), MC_Color(interfaceColor), 1.f, sideBars / 2);
			}
			//DrawUtils::setColor(br, bg, bb, opa);
			//DrawUtils::fillRectangle(vec2_t(breh.x, breh.y), vec2_t(invX + 153.f + sideBars, invY + 58.f + sideBars));
			DrawUtils::fillRectangle(breh, MC_Color(0.f, 0.f, 0.f), opa);
			for (int j = 9; j < 18; j++) {
				if ((inv2->getItemStack(j)->item != nullptr)) {
					DrawUtils::drawItem(inv2->getItemStack(j), vec2_t(xVal, yVal + 0.f), 1.f, 1.f, false);
					char stackAmount = inv2->getItemStack(j)->count;
					std::string stackStr = std::to_string(inv2->getItemStack(j)->count);
					if (inv2->getItemStack(j)->count > 1)
						DrawUtils::drawText(vec2_t(xVal + 7.f, (yVal + 0.f) + 10.f), &stackStr, MC_Color(255, 255, 255), 1.f - .20f, 1.f, true);
				}
				xVal += 17.f;
			}
			xVal -= 180.f;
			for (int j = 18; j < 27; j++) {
				if ((inv2->getItemStack(j)->item != nullptr)) {
					DrawUtils::drawItem(inv2->getItemStack(j), vec2_t(xVal + 27.f, yVal + 20.f), 1.f, 1.f, false);
					std::string stackStr = std::to_string(inv2->getItemStack(j)->count);
					if (inv2->getItemStack(j)->count > 1)
						DrawUtils::drawText(vec2_t(xVal + 34.f, (yVal + 20.f) + 10.f), &stackStr, MC_Color(255, 255, 255), 1.f - .20f, 1.f, true);
				}
				xVal += 17.f;
			}
			xVal -= 180.f;
			for (int j = 27; j < 36; j++) {
				if ((inv2->getItemStack(j)->item != nullptr)) {
					DrawUtils::drawItem(inv2->getItemStack(j), vec2_t(xVal + 54.f, yVal + 40.f), 1.f, 1.f, false);
					std::string stackStr = std::to_string(inv2->getItemStack(j)->count);
					if (inv2->getItemStack(j)->count > 1)
						DrawUtils::drawText(vec2_t(xVal + 61.f, (yVal + 40.f) + 10.f), &stackStr, MC_Color(255, 255, 255), 1.f - .20f, 1.f,  true);
				}
				xVal += 17.f;
			}
			xVal -= 180.f;
		}
}