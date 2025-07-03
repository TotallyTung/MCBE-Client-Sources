#include "BreakIndicator.h"
BreakIndicator::BreakIndicator() : IModule(0, Category::VISUAL, "Make rendering cool when destroying blocks.") {
	registerBoolSetting("Outline", &this->line, this->line);
	registerBoolSetting("Rainbow", &this->rainbow, this->rainbow);
	registerFloatSetting("Red", &this->rSelect, this->rSelect, 0.f, 1.f);
	registerFloatSetting("Green", &this->gSelect, this->gSelect, 0.f, 1.f);
	registerFloatSetting("Blue", &this->bSelect, this->bSelect, 0.f, 1.f);
	registerFloatSetting("Thickness", &this->thickness, this->thickness, 0.f, 1.f);
	registerFloatSetting("LineOpacity", &this->lineop, this->lineop, 0.f, 1.f);
	registerBoolSetting("Filled", &this->filled, this->filled);
	registerFloatSetting("FillOpacity", &this->fillop, this->fillop, 0.f, 1.f);
	registerBoolSetting("NoHighlight", &this->disable, this->disable);
}

BreakIndicator::~BreakIndicator() {
}

const char* BreakIndicator::getModuleName() {
	return ("BreakIndicators");
}
void BreakIndicator::onPreRender(MinecraftUIRenderContext* ctx) {
	static int delay = 0;//fix crashing for 

	if (!Game.isInGame()) {
		delay = 0;
		return;
	}
	if (delay >= 30) {
		static float rainbowColors[4];
		{
			if (rainbowColors[3] < 1) {
				rainbowColors[0] = 0.2f;
				rainbowColors[1] = 0.2f;
				rainbowColors[2] = 1.f;
				rainbowColors[3] = 1;
			}

			Utils::ApplyRainbow(rainbowColors, 0.0015f);
		}  // 0.0015f

		if (Game.getGameMode() == nullptr) return;
		static float percentsmooth = 0;
		static float opasmooth = 0;
		float progress = Game.getGameMode()->destroyProgress;
		auto pStruct = Game.getLocalPlayer()->getlevel();
		int rayType = pStruct->rayHitType;
		float speed = 0.1f;
		doBreaking = (bool)progress;
		if (!doBreaking) {
			speed = 0.06f;
		}
		percentsmooth = smoothLerp(progress, percentsmooth, speed);
		opasmooth = smoothLerp((double)doBreaking, percentsmooth, 0.2);
		if (percentsmooth == 0) return;
		float percent = percentsmooth;
		if (line) {
			if (rainbow)
				DrawUtils::setColor(rainbowColors[0], rainbowColors[1], rainbowColors[2], lineop * percent);
			else
				DrawUtils::setColor(rSelect, gSelect, bSelect, lineop * percent);
			DrawUtils::drawBox(pStruct->block.toVec3t().add(0.5).sub(0.5 * percent), pStruct->block.toVec3t().add(0.5).add(0.5 * percent), 0.4f, false);
		}
		if (filled) {
			if (rainbow)
				DrawUtils::setColor(rainbowColors[0], rainbowColors[1], rainbowColors[2], fillop * percent);
			else
				DrawUtils::setColor(rSelect, gSelect, bSelect, fillop * percent);
			DrawUtils::drawBox(pStruct->block.toVec3t().add(0.5).sub(0.5 * percent), pStruct->block.toVec3t().add(0.5).add(0.5 * percent), 0, true);
		}
	} else {
		delay++;
	}
}