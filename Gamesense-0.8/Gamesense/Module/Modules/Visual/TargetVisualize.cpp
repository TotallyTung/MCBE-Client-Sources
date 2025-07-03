#include "TargetVisualize.h"

using namespace std;
TargetVisualize::TargetVisualize() : IModule(0, Category::VISUAL, "Add big moving circle on who the client is attacking") {
	registerIntSetting("Range", &range, range, 3, 20);
}

const char* TargetVisualize::getRawModuleName() {
	return ("TargetVisualize");
}

static vector<C_Entity*> targetList;
struct CompareTargetEnArray {
	bool operator()(C_Entity* lhs, C_Entity* rhs) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};
void findEntitysss(C_Entity* currentEntity, bool isRegularEntity) {
	static auto TargetVisualizes = moduleMgr->getModule<TargetVisualize>();

	if (currentEntity == nullptr) return;
	if (currentEntity == g_Data.getLocalPlayer()) return;
	if (!g_Data.getLocalPlayer()->canAttack(currentEntity, false)) return;
	if (!g_Data.getLocalPlayer()->isAlive()) return;
	if (!currentEntity->isAlive()) return;
	if (currentEntity->getEntityTypeId() == 66) // falling block
		return;
	if (currentEntity->getEntityTypeId() == 69)  // XP
		return;
	if (currentEntity->getEntityTypeId() == 64) // item
		return;
	if (currentEntity->getEntityTypeId() == 80 || currentEntity->getEntityTypeId() == 69) return;
	if (!TargetUtil::isValidTarget(currentEntity)) return;
	if (currentEntity->getEntityTypeId() == 51 || currentEntity->getEntityTypeId() == 1677999) return;
	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
	if (dist < TargetVisualizes->range) {
		targetList.push_back(currentEntity);
		sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
	}
}
void TargetVisualize::onLevelRender() {
		auto player = g_Data.getLocalPlayer();
		if (player == nullptr) return;
		auto interfacrColor = ColorUtil::interfaceColor(1);
		static float n = 0;
		static float anim = 0;
		targetList.clear();

		g_Data.forEachEntity(findEntitysss);
		if (!targetList.empty()) {
			anim++;
			DrawUtils::setColor(interfacrColor.r, interfacrColor.g, interfacrColor.b, 1.f);
			vec3_t permutations[56];
			for (int i = 0; i < 56; i++) {
				permutations[i] = { sinf((i * 10.f) / (180 / PI)), 0.f, cosf((i * 10.f) / (180 / PI)) };
			}
			vec3_t permutations2[56];
			n++;
			if (n == 360)
				n = 0;
			for (int i = 0; i < 56; i++) {
				permutations2[i] = { sinf((i * 10.f) / (180 / PI)), sinf((i * 10.f) / (180 / PI)) ,cosf((i * 10.f) / (180 / PI)) };
			}
			const float coolAnim = 0.9f + 0.9f * sin((anim / 60) * PI * 1);
			vec3_t* start = targetList[0]->getPosOld();
			vec3_t* end = targetList[0]->getPos();
			auto te = DrawUtils::getLerpTime();
			vec3_t pos = start->lerp(end, te);
			auto yPos = pos.y;
			yPos -= 1.6f;
			yPos += coolAnim;
			vector<vec3_t> posList;
			posList.reserve(56);
			for (auto& perm : permutations) {
				vec3_t curPos(pos.x, yPos, pos.z);
				posList.push_back(curPos.add(perm));
			}
			auto interfaceMod = moduleMgr->getModule<Interface>();
			DrawUtils::drawLineStrip3D(posList, 5);
	}
}