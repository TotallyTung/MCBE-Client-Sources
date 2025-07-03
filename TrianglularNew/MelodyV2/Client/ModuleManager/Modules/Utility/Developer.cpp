#include "Developer.h"
#include "../../../Client.h"
bool doitemid = false;
bool burrowtest = false;
bool testwhat = false;
Developer::Developer() : Module("Developer", "help develop the client", Category::PLAYER) {
	pagesetting("Page", "Module Page", { "Place", "Break" });
	boolsetting("ItemID", "NULL", &doitemid, 0);
	boolsetting("burrowtest", "NULL", &burrowtest, 0);
	boolsetting("testwhat", "NULL", &testwhat, 1);
}
int delayTicks = 1; 
int tickCounter = 0;
void Developer::onNormalTick(Actor* actor) {
	GameMode* gm = mc.getGameMode();
	LocalPlayer* plr = mc.getLocalPlayer();
	if (gm == nullptr || plr == nullptr || !mc.getClientInstance()->minecraftGame->canUseKeys) return;
	auto plrInv = plr->getPlayerInventory();
	auto* actor1 = mc.getLocalPlayer();
	auto* armorPiece = actor1->getCarriedItem();
	if (doitemid) {
		std::string armorName = std::to_string(armorPiece->getitem() ? (*armorPiece->getitem())->getitemId() : 0);
		mc.DisplayClientMessage(("[%sDEVELOPER] %sID returned: " + armorName).c_str(), GREEN, WHITE);
		this->setEnabled(false);
	}

	if (burrowtest) {
		mc.DisplayClientMessage("[%sDEVELOPER] %sAttempting to burrow", GREEN, WHITE);
		Sleep(1);
		Vec3<float> currentPos = plr->stateVectorComponent->pos;
		Vec3<float> newPos = Vec3<float>(currentPos.x, currentPos.y - 1, currentPos.z);
		plr->setPos(newPos);
		if (tickCounter < delayTicks) {
			tickCounter++;
		}
		else {
			this->setEnabled(false);
			tickCounter = 0; 
		}
	}

}
void Developer::onRender(MinecraftUIRenderContext* ctx) {
	GameMode* gm = mc.getGameMode();
	LocalPlayer* localPlayer = mc.getLocalPlayer();
	if (gm == nullptr || localPlayer == nullptr || !mc.getClientInstance()->minecraftGame->canUseKeys) return;
	
}
void Developer::onSendPacket(Packet* packet, bool& shouldCancel) {
	GameMode* gm = mc.getGameMode();
	LocalPlayer* localPlayer = mc.getLocalPlayer();
	if (gm == nullptr || localPlayer == nullptr || !mc.getClientInstance()->minecraftGame->canUseKeys) return;
	
}
void Developer::onImGuiRender(ImDrawList* drawlist) {
	GameMode* gm = mc.getGameMode();
	LocalPlayer* localPlayer = mc.getLocalPlayer();
	if (gm == nullptr || localPlayer == nullptr || !mc.getClientInstance()->minecraftGame->canUseKeys) return;
}