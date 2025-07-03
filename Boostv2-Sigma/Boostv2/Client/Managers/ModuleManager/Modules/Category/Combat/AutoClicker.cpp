#include "AutoClicker.h"

AutoClicker::AutoClicker() : Module("AutoClicker", "A simple autoclicker, automatically clicks for you.", Category::COMBAT) {
	registerSetting(new BoolSetting("RightClick", "Click mouse With right too", & rightclick, rightclick));
	registerSetting(new BoolSetting("Break Blocks", "Break the Minecraft blocks", &breakBlocks, breakBlocks));
	registerSetting(new SliderSetting<int>("Delay", "The interval", & delay, delay, 0, 20));
	registerSetting(new BoolSetting("Hold", "Holding mode", & hold, hold));
}

Actor* getActorFromEntityId2(EntityId entityId) {
	auto player = Game.getLocalPlayer();
	for (auto actor : player->level->getRuntimeActorList()) {
		if (actor->getEntityContext()->mEntity.mRawId == entityId.mRawId) {
			return actor;
		}
	}
	return nullptr;
}
void AutoClicker::onNormalTick(LocalPlayer* gm) {
	if (gm == nullptr) return;
	if ((Game.isLeftClickDown() || !hold) && Game.canUseMoveKeys()) {
		LocalPlayer* player = Game.getLocalPlayer();
		Level* level = player->level;
		if (level == nullptr) return;
		Odelay++;

		if (Odelay >= delay) {
			auto selectedItem = player->getCarriedItem();

			player->swing();

			if (Game.getLocalPlayer()->level->getHitResult()->type == HitResultType::ENTITY) {
				Actor* hitActor = getActorFromEntityId2(Game.getLocalPlayer()->level->getHitResult()->entity.id);
				if (hitActor->getEntityContext()->mEntity.mRawId == player->getEntityContext()->mEntity.mRawId) return;
				gm->gameMode->attack(hitActor);
			}
			else if (breakBlocks) {
				bool isDestroyed = false;
				gm->gameMode->startDestroyBlock(level->getHitResult()->blockPos, level->getHitResult()->selectedFace, isDestroyed);
				gm->gameMode->stopDestroyBlock(level->getHitResult()->blockPos);
				if (isDestroyed && Game.getClientInstance()->getRegion()->getBlock(level->getHitResult()->blockPos)->blockLegacy->blockId != 0)
					gm->gameMode->destroyBlock(level->getHitResult()->blockPos, 0);
			}
			Odelay = 0;
		}
	}

	if (rightclick) {
		if ((Game.isRightClickDown() || !hold) && Game.canUseMoveKeys()) {
			Level* level = Game.getLocalPlayer()->level;
			Odelay++;
			if (Odelay >= delay) {
				bool idk = true;
				gm->gameMode->buildBlock(Vec3<int>(level->getHitResult()->blockPos), level->getHitResult()->selectedFace, idk);
				Odelay = 0;
			}
		}
	}
}