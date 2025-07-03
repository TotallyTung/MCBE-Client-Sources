#include "PopCounter.h"

using namespace std;
PopCounter::PopCounter() : IModule(0, Category::MISC, "Count players totem popped") {
	registerBoolSetting("Count YourSelf", &cys, cys);
	registerBoolSetting("SendChat", &sendchat, sendchat);
}
const char* PopCounter::getModuleName() {
	return "PopCounter";
}
static std::vector<Entity*> pc;
void fp(Entity* currentEntity, bool isRegularEntity) {
	if (currentEntity == nullptr) return;
	if (currentEntity->getEntityTypeId() != 319) return;
	if (currentEntity == Game.getLocalPlayer()) return;
	if (!Game.getLocalPlayer()->canAttack(currentEntity, false)) return;
	if (!currentEntity->isAlive()) return;
	pc.push_back(currentEntity);
}
void PopCounter::onTick(GameMode* gm) {
	pc.clear();
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;
	Game.forEachEntity(fp);
	if (!player->isAlive()) player->popcount = 0;
	if (!pc.empty()) {
		for (auto& i : pc) {
			ItemStack* current = i->getEquippedTotem();
			if (current->item == NULL) {
				if (!i->totem) {
					ItemStack* stack = i->getArmor(1);
					if (stack->item != nullptr) {
						i->popcount += 1;
						clientMessageF("%s%s%s %sPopped %s%i%s Totems!", Utils::getCommandClientName(), RED, i->getNameTag()->getText(), WHITE, GREEN, i->popcount, WHITE);
						Notifications::addNotifBox(std::string(std::string(i->getNameTag()->getText()) + " Popped " + std::to_string(i->popcount) + " Totems!"), 4.f);
#include <vector>
#include <cstdlib> // For rand()
#include <ctime>   // For seeding rand()
						if (sendchat) {
							static bool seeded = false;
							if (!seeded) {
								std::srand(std::time(nullptr));
								seeded = true;
							}
							std::vector<std::string> messages = {
								RED + std::string("@") + i->getNameTag()->getText() + WHITE + " got clapped! " + GREEN + std::to_string(i->popcount) + WHITE + " Totems down!",
								RED + std::string("@") + i->getNameTag()->getText() + WHITE + " just got destroyed! " + GREEN + std::to_string(i->popcount) + WHITE + " Totems popped!",
								RED + std::string("@") + i->getNameTag()->getText() + WHITE + " got wrecked! " + GREEN + std::to_string(i->popcount) + WHITE + " Totems gone!",
								RED + std::string("@") + i->getNameTag()->getText() + WHITE + " is struggling! " + GREEN + std::to_string(i->popcount) + WHITE + " Totems used!",
								RED + std::string("@") + i->getNameTag()->getText() + WHITE + " is losing totems fast! " + GREEN + std::to_string(i->popcount) + WHITE + " down!",
								RED + std::string("@") + i->getNameTag()->getText() + WHITE + " popping off... literally! " + GREEN + std::to_string(i->popcount) + WHITE + " Totems gone!"
							};
							std::string randomMessage = messages[rand() % messages.size()];
							Utils::sendChat(randomMessage);
						}

						i->totem = true;
					}
				}
			}
			else {
				i->totem = false;
			}
		}
	}
}
void PopCounter::onJoiningServer() {
	pc.clear();
}