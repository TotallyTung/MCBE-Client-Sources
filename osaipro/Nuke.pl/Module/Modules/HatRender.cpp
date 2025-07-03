#include "HatRender.h"
#include "../../DrawUtils.h"
HatRender::HatRender() : IModule(0x0, Category::VISUAL,  "HatRender dayo!!!!") {
	registerBoolSetting("Rainbow", &rainbow, rainbow);
	registerFloatSetting("Opacity", &opacity, opacity, 0.01f, 1.f);
};

const char* HatRender::getModuleName() {
	return "HatRender";
}
void HatRender::onLevelRender() {
	if (Game.getLocalPlayer() != nullptr) {
		float radius = Game.getLocalPlayer()->aabb.upper.x - Game.getLocalPlayer()->aabb.lower.x;
		Vec3 pos = Game.getLocalPlayer()->eyePos0;
		if (!Game.getLocalPlayer()->isSneaking()) pos.y += 0.25f;
		Vec3 permutations[361];
		for (int i = 0; i < 361; i++) permutations[i] = {(sinf((i * 1.f) / (180 / PI))) * radius, 0.f, (cosf((i * 1.f) / (180 / PI))) * radius};

		std::vector<Vec3> posList;
		for (Vec3& perm : permutations) posList.push_back(pos.add(perm));
		DrawUtils::drawHat(posList, opacity, 0.3f, pos, rainbow);

	}
}