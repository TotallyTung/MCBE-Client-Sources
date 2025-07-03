#include "blockHighlight.h"

#include "../../../Utils/Target.h"
selectionHighlight::selectionHighlight() : IModule(0x0, Category::VISUAL, "Custom selection box!") {
	registerBoolSetting("Outline", &this->doOutline, this->doOutline);
	registerFloatSetting("Thickness", &this->thickness, this->thickness, 0.f, 1.f);
	registerFloatSetting("LineOpacity", &this->opacityVal, this->opacityVal, 0.f, 1.f);
	registerBoolSetting("Rainbow", &this->selectRainbow, this->selectRainbow);
	registerFloatSetting("Red", &this->rSelect, this->rSelect, 0.f, 1.f);
	registerFloatSetting("Green", &this->gSelect, this->gSelect, 0.f, 1.f);
	registerFloatSetting("Blue", &this->bSelect, this->bSelect, 0.f, 1.f);
	registerBoolSetting("Animation", &this->animation, this->animation);
	registerBoolSetting("Filled", &this->fill, this->fill);
	registerFloatSetting("FillOpacity", &this->fOpacity, this->fOpacity, 0.f, 1.f);
}

selectionHighlight::~selectionHighlight() {
}

const char* selectionHighlight::getModuleName() {
	return ("BlockOutline");
}
Vec3 posi;
void selectionHighlight::onPreRender(MinecraftUIRenderContext* renderCtx) {
	float customHighlight[4];

	customHighlight[0] = rSelect;
	customHighlight[1] = rSelect;
	customHighlight[2] = rSelect;
	customHighlight[3] = 1.f;
	static float rainbowColors[4];
	if (!(Game.getLocalPlayer() == nullptr || !GameData::canUseMoveKeys())) {
		{
			if (rainbowColors[3] < 1) {
				rainbowColors[0] = 0.2f;
				rainbowColors[1] = 0.2f;
				rainbowColors[2] = 1.f;
				rainbowColors[3] = 1;
			}

			Utils::ApplyRainbow(rainbowColors, 0.0015f);
		}  //0.0015f

		if (Game.getLocalPlayer() != nullptr) {
			auto pStruct = Game.getLocalPlayer()->getlevel();

			if (animation) {
				if (!pStruct->block.iszero())
					posi = Vec3(smoothLerp(pStruct->block.toVec3t().x, posi.x, 0.1), smoothLerp(pStruct->block.toVec3t().y, posi.y, 0.1), smoothLerp(pStruct->block.toVec3t().z, posi.z, 0.1));
			} else
				posi = pStruct->block.toVec3t();
			float mC = thickness / 2;
			Vec3 hLower = posi;
			hLower = hLower;
			Vec3 hUpper = posi;
			hUpper = hUpper;
			hUpper.x += 1.f;
			hUpper.y += 1.f;
			hUpper.z += 1.f;

			if (selectRainbow) {
				int face = pStruct->blockSide;
				int rayType = pStruct->rayHitType;
				if (rayType == 0) {
					if (doOutline) {
						DrawUtils::setColor(rainbowColors[0], rainbowColors[1], rainbowColors[2], opacityVal);
						DrawUtils::drawBox(hLower, hUpper, thickness, false);
					}
					if (fill) {
						DrawUtils::setColor(rainbowColors[0], rainbowColors[1], rainbowColors[2], fOpacity);
						DrawUtils::drawBox(hLower, hUpper, 0, true);
					}
				}

			} else {
				int rayType = pStruct->rayHitType;
				if (rayType == 0) {
					if (doOutline) {
						DrawUtils::setColor(rSelect, gSelect, bSelect, opacityVal);
						DrawUtils::drawBox(hLower, hUpper, thickness, false);
					}
					if (fill) {
						DrawUtils::setColor(rSelect, gSelect, bSelect, fOpacity);
						DrawUtils::drawBox(hLower, hUpper, 0, true);
					}
				}
			}
		}
	}
}