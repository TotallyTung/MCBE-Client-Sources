#include "Tracer.h"

Tracer::Tracer() : Module("Tracer", "Draws lines to ESP highlighted entities.", Category::RENDER) {
	registerSetting(new ColorSetting("Color", "NULL", &color, UIColor(255, 255, 255, 175)));
	registerSetting(new BoolSetting("Mobs", "NULL", &mobs, false));
}

void Tracer::onLevelRender() {
	LocalPlayer* localPlayer = Game::getLocalPlayer();
	Tessellator* tessellator = MCR::tessellator;
	
	float calcYaw = (localPlayer->rotation->presentRot.y + 90.f) * (PI / 180.f);
	float calcPitch = (localPlayer->rotation->presentRot.x) * -(PI / 180.f);
	Vec3<float> moveVec;
	moveVec.x = cos(calcYaw) * cos(calcPitch) * 0.2f;
	moveVec.y = sin(calcPitch) * 0.2f;
	moveVec.z = sin(calcYaw) * cos(calcPitch) * 0.2f;

	Vec3<float> origin = Game::clientInstance->getLevelRenderer()->levelRendererPlayer->realOrigin;

	Vec3<float> start = moveVec;
	MCR::setColor(color);
	for (auto& actor : localPlayer->level->getRuntimeActorList()) {
		if (TargetUtil::isTargetValid(actor, mobs)) {
			Vec3<float> end = actor->getEyePos().sub(origin);
			tessellator->begin(VertextFormat::LINE_LIST, 2);
			tessellator->vertex(start.x, start.y, start.z);
			tessellator->vertex(end.x, end.y, end.z);
			MeshHelpers::renderMeshImmediately(MCR::screenCtx, tessellator, MCR::blendMaterial);
		}
	}
}