#include "Clip.h"

Clip::Clip() : Module("Wallclip", "Clip onto walls to prevent crystal damage.", Category::MISC) {
	addSlider<float>("Value", "How far you want to clip into the wall.", ValueType::FLOAT_T, &Clipvalue, 0.f, 0.35f);
}
void Clip::onNormalTick(Actor* actor) {
	LocalPlayer* localPlayer = mc.getLocalPlayer();
	auto playerPos = *localPlayer->getPosition();
	localPlayer->getAABB()->lower.x = playerPos.x - Clipvalue;
	localPlayer->getAABB()->lower.z = playerPos.z - Clipvalue;
	localPlayer->getAABB()->upper.x = playerPos.x + Clipvalue;
	localPlayer->getAABB()->upper.z = playerPos.z + Clipvalue;
}
void Clip::onDisable() {
	LocalPlayer* localPlayer = mc.getLocalPlayer();
	auto playerPos = *localPlayer->getPosition();
	localPlayer->getAABB()->lower.x = playerPos.x - 0.35f;
	localPlayer->getAABB()->lower.z = playerPos.z - 0.35f;
	localPlayer->getAABB()->upper.x = playerPos.x + 0.35f;
	localPlayer->getAABB()->upper.z = playerPos.z + 0.35f;
}
