#include "AirPlace.h"

AirPlace::AirPlace() : Module("AirPlace", "Place block on air", Category::WORLD) {
	registerSetting(new BoolSetting("Render", "Renders a block overlay where your block will be placed", &render, true));
	registerSetting(new ColorSetting("Color", "NULL", &color, UIColor(0, 255, 0), false));
	registerSetting(new SliderSetting<int>("Opacity", "NULL", &opacity, 0, 0, 255));
	registerSetting(new SliderSetting<int>("LineOpacity", "NULL", &lineOpacity, 255, 0, 255));
}

void AirPlace::onNormalTick(LocalPlayer* localPlayer) {
	shouldRender = false;

	BlockSource* region = Game::clientInstance->getRegion();
	if (region == nullptr)
		return;

	Level* level = localPlayer->level;
	if (level == nullptr)
		return;

	HitResult* hitResult = level->getHitResult();
	if (hitResult == nullptr)
		return;

	if (hitResult->type == HitResultType::AIR) {
		ItemStack* carriedItem = localPlayer->getCarriedItem();
		if (carriedItem->isValid() && carriedItem->isBlock()) {

			Vec3<float> endPos = hitResult->endPos;
			BlockPos placePos;
			placePos.x = (int)floorf(endPos.x);
			placePos.y = (int)floorf(endPos.y);
			placePos.z = (int)floorf(endPos.z);

			if (Game::canUseMoveKeys()) {
				if (Game::isRightClickDown()) {
					if (shouldPlace) {
						localPlayer->gameMode->buildBlock(placePos, 0, true);
						shouldPlace = false;
					}
				}
				else {
					shouldPlace = true;
				}
			}

			renderAABB.lower = placePos.CastTo<float>();
			renderAABB.upper = renderAABB.lower.add(Vec3<float>(1.f, 1.f, 1.f));
			shouldRender = true;
		}
	}
}

void AirPlace::onLevelRender() {
	if (!render)
		return;

	if (!shouldRender)
		return;

	MCR::drawBox3dFilled(renderAABB, UIColor(color.r, color.g, color.b, opacity), UIColor(color.r, color.g, color.b, lineOpacity));
}