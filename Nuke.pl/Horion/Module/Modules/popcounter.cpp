#include "PopCounter.h"

using namespace std;
PopCounter::PopCounter() : IModule(0, Category::MISC, "Count players totem popped") {
	registerBoolSetting("Count Yourself", &countyourself, countyourself);
}

const char* PopCounter::getModuleName() {
	return "PopCounter";
}

static vector<Entity*> playerlist;
int yourpop = 0;
bool equipped = false;

void checkplayer(Entity* currentEntity, bool isRegularEntity) {
	if (currentEntity == nullptr) return;
	if (currentEntity == Game.getLocalPlayer()) return;

	if (currentEntity->getEntityTypeId() != 316) return;

	float dist = (*currentEntity->getPos()).dist(*Game.getLocalPlayer()->getPos());
	if (dist < 15) playerlist.push_back(currentEntity);
}
void PopCounter::onEnable() {
	yourpop = 0;
}

void PopCounter::onTick(GameMode* gm) {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;

	ItemStack* playertotem = player->getEquippedTotem();

	listempty = playerlist.empty();

	playerlist.clear();
	Game.forEachEntity(checkplayer);

	if (playertotem->item != NULL) equipped = true;

	if (!playerlist.empty() && Game.canUseMoveKeys()) {
		for (auto& i : playerlist) {
			if (!i->consumeTotem()) {
				ItemStack* playertotem = i->getEquippedTotem();
				clientMessageF("%s Popped int totems", i->getNameTag()->getText());
			}
		}
	}

	if (countyourself) {
		if (equipped) {
			if (!player->consumeTotem()) {
				clientMessageF("You popped %s totems", std::to_string(yourpop));
				yourpop += 1;
				equipped = false;
			}
		}
	}
}