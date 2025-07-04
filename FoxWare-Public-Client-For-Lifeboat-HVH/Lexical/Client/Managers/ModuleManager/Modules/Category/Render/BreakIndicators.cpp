#include "BreakIndicators.h"

BreakIndicators::BreakIndicators() : Module("BreakIndicators", "Renders the progress of a block being broken", Category::RENDER) {
	registerSetting(new ColorSetting("StartColor", "The color for the non-broken block", &startColor, UIColor(255, 0, 0), false));
	registerSetting(new ColorSetting("EndColor", "The color for the non-broken block", &endColor, UIColor(0, 255, 0), false));
	registerSetting(new SliderSetting<int>("Alpha", "NULL", &alpha, 40, 0, 255));
	registerSetting(new SliderSetting<int>("LineAlpha", "NULL", &lineAlpha, 175, 0, 255));
}

void BreakIndicators::onLevelRender() {
	LocalPlayer* localPlayer = Game::getLocalPlayer();

	BlockSource* region = Game::clientInstance->getRegion();
	if (region == nullptr)
		return;

	GameMode* gameMode = localPlayer->gameMode;
	if (gameMode == nullptr)
		return;

	float destroyProgress = gameMode->destroyProgress;
	if (destroyProgress > 0.f) {
		BlockPos blockLookingAt = gameMode->lastBreakPos;
		Block* block = region->getBlock(blockLookingAt);
		AABB blockAABB;
		block->blockLegacy->getOutline(block, region, &blockLookingAt, &blockAABB);

		UIColor fillColor = ColorUtil::lerp(startColor, endColor, destroyProgress);
		UIColor lineColor = fillColor;

		fillColor.a = alpha;
		lineColor.a = lineAlpha;

		MCR::drawBox3dFilled(blockAABB, fillColor, lineColor, destroyProgress);
	}
}