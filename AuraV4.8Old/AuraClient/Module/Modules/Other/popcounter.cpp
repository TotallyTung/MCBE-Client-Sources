#include "PopCounter.h"

using namespace std;
PopCounter::PopCounter() : IModule(0, Category::OTHER, "Count players totem popped") {
	registerBoolSetting("Client Message", &cys, cys);
	registerBoolSetting("SendChat", &sendchat, sendchat);
}
const char* PopCounter::getModuleName() {
	return "PopCounter";
}
static std::vector<C_Entity*> pc;
void fp(C_Entity* currentEntity, bool isRegularEntity) {
	if (currentEntity == nullptr) return;
	if (currentEntity->getEntityTypeId() != 319) return;
	if (currentEntity == g_Data.getLocalPlayer()) return;
	if (!g_Data.getLocalPlayer()->canAttack(currentEntity, false)) return;
	if (!currentEntity->isAlive()) return;
	pc.push_back(currentEntity);
}
void PopCounter::onTick(C_GameMode* gm) {
	pc.clear();
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
	g_Data.forEachEntity(fp);
	if (!player->isAlive()) player->popcount = 0;
	if (!pc.empty()) {
		for (auto& i : pc) {
			C_ItemStack* current = i->getEquippedTotem();
			if (current->item == NULL) {
				if (!i->totem) {
					C_ItemStack* stack = i->getArmor(1);
					if (stack->item != nullptr) {
						i->popcount += 1;
						clientMessageF("%s%s%s %sPopped %s%i%s Totems!", Utils::getCommandClientName(), RED, i->getNameTag()->getText(), WHITE, GREEN, i->popcount, WHITE);
						//Notifications::addNotifBox(std::string(std::string(i->getNameTag()->getText()) + " Popped " + std::to_string(i->popcount) + " Totems!"), 4.f);
						if (sendchat) {
							Utils::sendChat(std::string(RED + std::string("@") + i->getNameTag()->getText() + WHITE + " Popped " + GREEN + std::to_string(i->popcount) + WHITE + " Totems"));
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