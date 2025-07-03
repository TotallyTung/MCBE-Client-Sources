// Created by Tony on 2024-10-10 10:01:54

#include "Tracer.h"

Tracer::Tracer() : Module("Tracer", "Draws lines to ESP highlighted entities.", Category::RENDER) {
	addBoolCheck("Mobs", "NULL", &mobs);
	addColorPicker("Color", "NULL", &color);
}

void Tracer::onLevelRender() {
	LocalPlayer* localPlayer = mc.getLocalPlayer();
	Tessellator* tess3D = RenderUtils::getTessellator3D();

	float calcYaw = (localPlayer->getRotation()->y + 90) * (PI / 180);
	float calcPitch = (localPlayer->getRotation()->x) * -(PI / 180);
	Vec3<float> moveVec;
	moveVec.x = cos(calcYaw) * cos(calcPitch) * 0.5f;
	moveVec.y = sin(calcPitch) * 0.5f;
	moveVec.z = sin(calcYaw) * cos(calcPitch) * 0.5f;

	Vec3<float> origin = mc.getClientInstance()->getLevelRenderer()->levelRendererPlayer->cameraPos1;

	Vec3<float> start = moveVec;
	RenderUtils::setColor(color.toMC_Color());
	for (Actor*& actor : localPlayer->getLevel()->getRuntimeActorList()) {
		if (TargetUtils::isTargetValid(actor, this->mobs)) {
			Vec3<float> end = actor->getEyePos().sub(origin);
			tess3D->begin(VertextFormat::LINE_LIST, 2);
			tess3D->vertex(start.x, start.y, start.z);
			tess3D->vertex(end.x, end.y, end.z);
			tess3D->renderMeshImmediately(RenderUtils::screenContext3D, RenderUtils::blendMaterial);
		}
	}
}