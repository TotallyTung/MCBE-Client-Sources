#include "BlockHighlight.h"

BlockHighlight::BlockHighlight() : Module("BlockHighlight", "Highlights the block that you are currently looking at", Category::RENDER) {
	registerSetting(new ColorSetting("Color", "NULL", &color, UIColor(255, 255, 255), false));
	registerSetting(new SliderSetting<int>("Alpha", "NULL", &alpha, 40, 0, 255));
	registerSetting(new SliderSetting<int>("LineAlpha", "NULL", &lineAlpha, 175, 0, 255));
	registerSetting(new BoolSetting("Slide", "NULL", &slide, false));
	registerSetting(new SliderSetting<float>("SlideSpeed", "NULL", &slideSpeed, 10.f, 5.f, 20.f));
	oldRenderAABB.lower.y = -999.f;
}

void BlockHighlight::onLevelRender() {
	BlockSource* region = Game::clientInstance->getRegion();
	if (region == nullptr)
		return;

	Level* level = Game::getLocalPlayer()->level;
	if (level == nullptr)
		return;

	HitResult* hitResult = level->getHitResult();
	if (hitResult == nullptr)
		return;

	if (hitResult->type == HitResultType::BLOCK) {
		BlockPos blockLookingAt = hitResult->blockPos;
		Block* block = region->getBlock(blockLookingAt);
		AABB blockAABB;
		block->blockLegacy->getOutline(block, region, &blockLookingAt, &blockAABB);

		AABB renderAABB = blockAABB;
		if (slide) {
			if (oldRenderAABB.lower.y == -999.f) {
				oldRenderAABB = renderAABB;
			}
			else {
				float animSpeed = MCR::deltaTime * slideSpeed;
				oldRenderAABB.lower = oldRenderAABB.lower.lerp(blockAABB.lower, animSpeed, animSpeed, animSpeed);
				oldRenderAABB.upper = oldRenderAABB.upper.lerp(blockAABB.upper, animSpeed, animSpeed, animSpeed);
			}
			renderAABB = oldRenderAABB;
		}
		else {
			oldRenderAABB.lower.y = -999.f;
		}
		MCR::drawBox3dFilled(renderAABB, UIColor(color.r, color.g, color.b, alpha), UIColor(color.r, color.g, color.b, lineAlpha));
	}
	else {
		oldRenderAABB.lower.y = -999.f;
	}
}