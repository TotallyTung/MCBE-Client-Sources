#include "PopCounter.h"

using namespace std;
PopCounter::PopCounter() : Module("PopCounter", "Counter Pop", Category::COMBAT) {
}

static std::vector<Actor*> playerlist;


void PopCounter::onNormalTick(LocalPlayer* actor) {
	playerlist.clear();

	LocalPlayer* player = Game.getLocalPlayer();
	PlayerInventory* plrInv = player->getplayerInventory();
	Container* inv = plrInv->container;
	GameMode* gm = Game.getGameMode();
	BlockSource* region = Game.getClientInstance()->getRegion();
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
		if (popcount > 37) {
			popcount = 0;
		}
		//ItemStack *current = i->getOffhandSlot()->item;
		if (!playerlist[0]->getOffhandSlot()->item) {
			if (!totem) {
				ItemStack* stack = playerlist[0]->getArmor(1);
				if (stack->item) {
					popcount += 1;

					Game.DisplayClientMessage("[%s%s%s] %s%s %sPopped %s%i%s Totems!", MCTF::RED, "PopCounter", MCTF::WHITE, MCTF::WHITE, playerlist[0]->getNameTag()->c_str(), MCTF::WHITE, MCTF::RED, popcount, MCTF::WHITE);

					std::string messagebox = playerlist[0]->getNameTag()->c_str() + std::to_string(popcount) + " totems!";
					totem = true;
					render = true;
				}
			}
		}
		else {
			totem = false;
		};

	}

}