#pragma once
#include "../../Utils/MemoryUtil.h"
#include "../Game.h"

class BaseActorRenderContext;
class ItemStack;

class ItemRenderer {
public:
	void renderGuiItemNew(BaseActorRenderContext* baseActorRenderCtx, ItemStack* item, int mode, float x, float y, float opacity, float scale, bool isEnchanted) {
		if (Game.getLocalPlayer() == nullptr) return;
		if (baseActorRenderCtx == nullptr) return;
		if (item == nullptr)return;
		if (this == nullptr) return;
		// Fonksiyon adresi
		using renderGuiItemNew_t = void(__thiscall*)(ItemRenderer*, BaseActorRenderContext*, ItemStack*, int, float, float, bool, float, float, float);
		static renderGuiItemNew_t renderGuiItemNew = reinterpret_cast<renderGuiItemNew_t>(Addresses::ItemRenderer_renderGuiItemNew);
		if (!renderGuiItemNew) return;

		// �a��r
		renderGuiItemNew(this, baseActorRenderCtx, item, mode, x, y, isEnchanted, opacity, 0.0f, scale);
	}
};
