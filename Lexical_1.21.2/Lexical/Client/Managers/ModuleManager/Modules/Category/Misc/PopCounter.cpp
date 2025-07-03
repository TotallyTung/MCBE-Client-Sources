#include "PopCounter.h"

using namespace std;
PopCounter::PopCounter() : Module("PopCounter", "Counter Pop", Category::MISC) {
}

static std::vector<Actor*> playerlist;


void PopCounter::onNormalTick(LocalPlayer* actor) {
	playerlist.clear();
	LocalPlayer* player = Game::getLocalPlayer();
	PlayerInventory* plrInv = player->getplayerInventory();
	Container* inv = plrInv->container;
	GameMode* gm = Game::getLocalPlayer()->getgameMode();
	BlockSource* region = Game::getClientInstance()->getRegion();
	Level* level = player->getlevel();
	if (!level) return;
	if (player == nullptr)
		return;
	for (Actor* actor : level->getRuntimeActorList()) {
		if (TargetUtil::isTargetValid(actor, false)) {
			float dist = actor->getPos().dist(player->getPos());
			if (dist < 50) playerlist.push_back(actor);
		}
	}
	if (!playerlist.empty()) {
		if (popcounts > 37) {
			popcounts = 0;
		}
		if (!playerlist[0]->getOffhandSlot()->item) {
			if (!totem) {
				ItemStack* stack = playerlist[0]->getArmor(1);
				if (stack->item) {
					popcounts += 1;
					Game::DisplayClientMessage("[%sLexical%s] %s%s%s popped %s%i%s totems!",
						MCTF::GRAY, MCTF::DARK_GREEN,
						MCTF::RED, playerlist[0]->getNameTag()->c_str(), MCTF::GRAY,
						MCTF::WHITE, MCTF::DARK_GREEN, popcounts, MCTF::WHITE);
					std::string messagebox = playerlist[0]->getNameTag()->c_str() + std::to_string(popcounts) + " totems!";
					totem = true;
				}
			}
		}
		else {
			totem = false;
		};

	}

}