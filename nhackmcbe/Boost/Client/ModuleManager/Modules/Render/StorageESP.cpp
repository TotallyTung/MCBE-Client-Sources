#include "StorageESP.h"

#include "../../../../Utils/RenderUtils.h"
StorageESP::StorageESP() : Module("StorageESP", "ESP for but storage blocks.", Category::RENDER) {
	addSlider<float>("Opacity", "NULL", ValueType::FLOAT_T, &opacity, 0.1f, 255.f);
	addSlider<int>("Radius", "NULL", ValueType::INT_T, &esese, 1, 40);
}

void StorageESP::getBlocks(std::vector<Vec3<int>>& blocks) {
	blocks.clear();
	int radius = static_cast<int>(esese);
	for (auto x = -radius; x <= radius; x++) {
		for (auto y = -radius; y <= radius; y++) {
			for (auto z = -radius; z <= radius; z++) {
				Vec3<int> blockPos(static_cast<int>(mc.getLocalPlayer()->getPosition()->x) + x,
					static_cast<int>(mc.getLocalPlayer()->getPosition()->y) + y,
					static_cast<int>(mc.getLocalPlayer()->getPosition()->z) + z);
				Block* block = mc.getLocalPlayer()->dimension->blockSource->getBlock(blockPos);
				if (block->blockLegacy->blockId == 54 || block->blockLegacy->blockId == 146 || block->blockLegacy->blockId == 130 || block->blockLegacy->blockId == 458 || block->blockLegacy->blockId == 205 || block->blockLegacy->blockId == 218 || block->blockLegacy->blockId == 154)
					blocks.push_back(blockPos);
			}
		}
	}
}

void StorageESP::onRender(MinecraftUIRenderContext* renderCtx) { // kkk
	LocalPlayer* localPlayer = mc.getLocalPlayer();
	if (localPlayer == nullptr) return;
	if (!mc.canUseMoveKeys()) return;
	Level* level = localPlayer->getLevel();
	if (level == nullptr) return;
	getBlocks(blocks);
	if (blocks.empty()) return;
	for (const Vec3<int>& offset : blocks) {
		Vec3<int> blockPos = offset;//Vec3<int>(mc.getLocalPlayer()->getPosition()->x + offset.x, mc.getLocalPlayer()->getPosition()->y + offset.y, mc.getLocalPlayer()->getPosition()->z + offset.z);
		Block* block = mc.getLocalPlayer()->dimension->blockSource->getBlock(blockPos);
		AABB aabb = mc.getLocalPlayer()->makeAABB(&blockPos);
		UIColor flushColor = UIColor();
		if (block->blockLegacy->blockId == 54) flushColor = UIColor(1.f, 1.f, 1.f, opacity);                     // Normal Chest
		if (block->blockLegacy->blockId == 146) flushColor = UIColor(.92f, .14f, .14f, opacity);                 // Trapped Chest
		if (block->blockLegacy->blockId == 130) flushColor = UIColor(0.435294f, 0.215686f, 0.631372f, opacity);  // Ender Chest
		if (block->blockLegacy->blockId == 458) flushColor = UIColor(0.62f, 0.31f, 0.04f, opacity);              // Barrel
		if (block->blockLegacy->blockId == 205) flushColor = UIColor(.49f, .17f, .95f, opacity);                 // Undyed Shulker Box
		if (block->blockLegacy->blockId == 218) flushColor = UIColor(.08f, .91f, .99f, opacity);                 // Shulker Box
		if (block->blockLegacy->blockId == 154) flushColor = UIColor(166, 166, 166, opacity);               // Hopper
		RenderUtils::setColor(flushColor.r, flushColor.g, flushColor.b, flushColor.a);
		RenderUtils::drawBox(blockPos.toFloat(), flushColor, flushColor, .3f, false, true);
	}
}