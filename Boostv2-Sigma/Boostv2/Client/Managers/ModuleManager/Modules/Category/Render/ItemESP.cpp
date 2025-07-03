#include "ItemESP.h"

ItemESP::ItemESP() : Module("ItemESP", "Highlights items", Category::RENDER) {
    registerSetting(new ColorSetting("Color", "NULL", &color, UIColor(255, 255, 255), false));
    registerSetting(new SliderSetting<int>("Alpha", "NULL", &alpha, 40, 0, 255));
    registerSetting(new SliderSetting<int>("LineAlpha", "NULL", &lineAlpha, 135, 0, 255));
}

void ItemESP::onLevelRender() {
    LocalPlayer* localPlayer = Game.getLocalPlayer();
    for (auto& entity : localPlayer->level->getRuntimeActorList()) {
        uint32_t entityId = entity->getActorTypeComponent()->id;
        if (items && entityId == 64) {
            Vec3<float> entityEyePos = entity->getEyePos();
            AABBShapeComponent* entityAABBShape = entity->aabbShape;

            AABB aabbRender;
            float itemSize = 0.3f;
            aabbRender.lower = entityEyePos.sub(Vec3<float>(itemSize / 2.f, 0.f, itemSize / 2.f));
            aabbRender.upper = aabbRender.lower.add(Vec3<float>(itemSize, itemSize, itemSize));

            MCR::drawBox3dFilled(aabbRender, UIColor(color.r, color.g, color.b, alpha), UIColor(color.r, color.g, color.b, lineAlpha));
        }
    }
}
