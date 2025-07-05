#include "Derp.h"

#include "../ModuleManager.h"

Derp::Derp() : IModule(0, Category::MISC, "100% entirely unhittable AA (real)") {
	registerBoolSetting("stop on throw", &this->disableOnThrowableItemHeld, this->disableOnThrowableItemHeld);
	registerBoolSetting("stop on mine", &this->disableOnMine, this->disableOnMine);

	registerBoolSetting("Enable Real", &this->real, this->real);

	registerBoolSetting("Enable rPitch", &this->enableRealPitch, this->enableRealPitch);
	registerIntSetting("Real Pitch", &this->realPitch, this->realPitch, -270, 270);
	registerIntSetting("RPitch Jitter", &this->realPitchJitter, this->realPitchJitter, 0, 360);

	registerBoolSetting("Enable rLBY", &this->enableRealLBY, this->enableRealLBY);
	registerIntSetting("Real LBY", &this->realLBY, this->realLBY, -270, 270);
	registerIntSetting("rLBY Jitter", &this->realLBYJitter, this->realLBYJitter, 0, 180);

	registerBoolSetting("Enable Desync", &this->desync, this->desync);

	registerBoolSetting("Enable dPitch", &this->enableFakePitch, this->enableFakePitch);
	registerIntSetting("Desync Pitch", &this->fakePitch, this->fakePitch, -270, 270);
	registerIntSetting("DPitch Jitter", &this->fakePitchJitter, this->fakePitchJitter, 0, 180);

	registerBoolSetting("Enable dLBY", &this->enableFakeLBY, this->enableFakeLBY);
	registerIntSetting("Desync LBY", &this->fakeLBY, this->fakeLBY, -270, 270);
	registerIntSetting("dLBY Jitter", &this->fakeLBYJitter, this->fakeLBYJitter, 0, 180);
}

Derp::~Derp() {
}

const char* Derp::getModuleName() {
	return "AntiAim";
}

void Derp::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr || !g_Data.canUseMoveKeys())
		return;

	if ((g_Data.getLocalPlayer()->level->hasEntity() != 0) && GameData::isLeftClickDown() && disableOnMine)
		return;

	C_ItemStack* stack = gm->player->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot);
	if (disableOnThrowableItemHeld && ((GameData::isRightClickDown() && stack->item != nullptr && (*stack->item)->itemId != 0 && (stack->getItem()->itemId == 508 || stack->getItem()->itemId == 374 || stack->getItem()->itemId == 422))))
	
	if (desync) {
		C_MovePlayerPacket p(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());

		int pJit2 = 0;
		int lJit2 = 0;

		if (fakeLBYJitter != 0)
			int lJit2 = (rand() % fakeLBYJitter);
			
		if (fakePitchJitter != 0)
			int pJit2 = (rand() % fakePitchJitter);

		if (enableFakePitch)
			p.pitch = (float)(fakePitch + pJit2);

		if (enableFakeLBY)
			p.yaw = (float)(fakeLBY + lJit2);

		if (enableFakeLBY || enableFakePitch)
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p);
	}
	
	if (real) {

		if (realLBYJitter != 0)
			lJit = (rand() % realLBYJitter);

		if (realPitchJitter != 0)
			pJit = (rand() % realPitchJitter);
		
		if (enableRealPitch)
			gm->player->pitch =   (float) (realPitch + pJit);
		if (enableRealLBY)
			gm->player->bodyYaw = (float) (realLBY + lJit);
			
	}

	return;
}
