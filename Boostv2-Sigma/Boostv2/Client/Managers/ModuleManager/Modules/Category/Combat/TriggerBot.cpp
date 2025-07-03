#include "TriggerBot.h"

int delay = 0;

TriggerBot::TriggerBot() : Module("TriggerBot", "Just Triggers A bot.", Category::COMBAT) {
    // "Delay" slider'ý, saldýrý arasýndaki tick bazlý gecikmeyi ayarlar.
    registerSetting(new SliderSetting<int>("Delay", "Attack delay (Tick)", &delay, 0, 0, 20));
}

Actor* getActorFromEntityId(EntityId entityId) {
    auto player = Game.getLocalPlayer();
    for (auto actor : player->level->getRuntimeActorList()) {
            if (actor->getEntityContext()->mEntity.mRawId == entityId.mRawId) {
                return actor;
        }
    }
    return nullptr;
}

void TriggerBot::onNormalTick(LocalPlayer* localPlayer) {
    auto player = Game.getLocalPlayer();
    HitResult* hitResult = player->level->getHitResult();

    if (hitResult->type == HitResultType::ENTITY) {
        Actor* hitActor = getActorFromEntityId(hitResult->entity.id);
        if (!hitActor) return;
        if (hitActor->getEntityContext()->mEntity.mRawId == player->getEntityContext()->mEntity.mRawId) return;

        static int tickDelay = 0;
        if (tickDelay > 0) {
            tickDelay--;
            return;
        }

        player->swing();
        player->gameMode->attack(hitActor);

        tickDelay = delay;
    }
}