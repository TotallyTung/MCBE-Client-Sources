// Created by Tony on 2024-10-10 10:01:54

#include "Fly.h"
float Speedy = 5.0f;
Fly::Fly() : Module("Fly", "Extend reach.", Category::MOVEMENT) {
	addSlider<float>("X-Axis", "NULL", ValueType::FLOAT_T, &Speed, 0.f, 20.f);
    addSlider<float>("Y-Axis", "NULL", ValueType::FLOAT_T, &Speedy, 0.f, 20.f);
}
static Vec2<float> getMotion(float playerYaw, float speed)
{
   //mc.isKeyDown('L'))
    bool w = mc.isKeyDown('W');
    bool a = mc.isKeyDown('A');
    bool s = mc.isKeyDown('S');
    bool d = mc.isKeyDown('D');

    if (w)
    {
        if (!a && !d)
            playerYaw += 90.0f;
        if (a)
            playerYaw += 45.0f;
        else if (d)
            playerYaw += 135.0f;
    }
    else if (s)
    {
        if (!a && !d)
            playerYaw -= 90.0f;
        if (a)
            playerYaw -= 45.0f;
        else if (d)
            playerYaw -= 135.0f;
    }
    else
    {
        if (!a && d) playerYaw += 180.0f;
    }

    float calcYaw = playerYaw * 0.01745329251f; //0.01745329251f = PI / 180
    return { cos(calcYaw) * speed / 10.0f,sin(calcYaw) * speed / 10.0f };
}

void Fly::onNormalTick(Actor* actor) {
    LocalPlayer* localPlayer = mc.getLocalPlayer();
	Level* level = localPlayer->getLevel();
	GameMode* gm = mc.getGameMode();
	PlayerInventory* plrInv = localPlayer->getPlayerInventory();
	Inventory* inv = plrInv->inventory;
	BlockSource* region = localPlayer->dimension->blockSource;
    float flySpeed = Speed;
    Vec2<float> motion = getMotion(localPlayer->rotationComponent->Get().y, flySpeed);

    bool w = mc.isKeyDown('W');
    bool a = mc.isKeyDown('A');
    bool s = mc.isKeyDown('S');
    bool d = mc.isKeyDown('D');
    bool usingMoveKeys = w || a || s || d;
    if (!usingMoveKeys) {
        motion.x = 0.0f;
        motion.y = 0.0f;
    }
    //EntityContext* entitescontext = localPlayer->entityContext;
    //localPlayer->setonground(entitescontext, true);
    
    localPlayer->stateVectorComponent->velocity.x = motion.x;
    localPlayer->stateVectorComponent->velocity.z = motion.y;

    if (mc.isKeyDown(VK_SPACE)) {
        localPlayer->stateVectorComponent->velocity.y = Speedy / 10.0f;
    }
    else if (mc.isKeyDown(VK_SHIFT)) {
        localPlayer->stateVectorComponent->velocity.y = -Speedy / 10.0f;
    }
    else {
        localPlayer->stateVectorComponent->velocity.y = 0.0f;
    }
}
