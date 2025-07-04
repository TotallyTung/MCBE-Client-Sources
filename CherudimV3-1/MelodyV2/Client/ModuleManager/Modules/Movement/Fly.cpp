#include "Fly.h"
#include "../Combat/Killaura.h"
#include "../../../Client.h"
float Speedy = 5.0f;
Fly::Fly() : Module("Fly", "Extend reach.", Category::MOVEMENT) {
	addSlider<float>("X-Axis", "NULL", ValueType::FLOAT_T, &Speed, 0.f, 50.f);
    addSlider<float>("Y-Axis", "NULL", ValueType::FLOAT_T, &Speedy, 0.f, 20.f);
    addBoolCheck("Strafe", "NULL", &Strafe);
}
static Vec2<float> getMotion(float playerYaw, float speed)
{
    static Killaura* kaMod2 = (Killaura*)client->moduleMgr->getModule("Killaura");
   //mc.isKeyDown('L'))
    bool w = mc.isKeyDown('W');
    bool a = mc.isKeyDown('A');
    bool s = mc.isKeyDown('S');
    bool d = mc.isKeyDown('D');

    if (w)
    {
        if (!a && !d) {
            if (kaMod2->isEnabled() && kaMod2->TargetDis < kaMod2->MinSrange && kaMod2->TargetDis !=0)
                playerYaw += 0.f;
            else
                playerYaw += 90.f;
        }
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
    static Killaura* kaMod = (Killaura*)client->moduleMgr->getModule("Killaura");

    float AheadY = localPlayer->rotationComponent->rotation.y;
    float KaAhead = kaMod->rotAngle.y;
    //mc.DisplayClientMessage("FLY = %f", AheadY);
    if(kaMod->isEnabled() && kaMod->TargetDis < kaMod->MaxSrange && kaMod->TargetDis != 0)
     motion = getMotion(KaAhead, flySpeed);
    else
     motion = getMotion(AheadY, flySpeed);

    if (mc.isKeyDown(VK_SPACE)) {
        localPlayer->stateVectorComponent->velocity.y += Speedy / 10.0f;
    }
    else if (mc.isKeyDown(VK_SHIFT)) {
        localPlayer->stateVectorComponent->velocity.y -= Speedy / 10.0f;
    }
    else {
        localPlayer->stateVectorComponent->velocity.y = 0.0f;
    }
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
    //localPlayer->setonground(entitescontext, true);;
        MoveVec.x = motion.x;
        MoveVec.y = localPlayer->stateVectorComponent->velocity.y;
        MoveVec.z = motion.y;
        localPlayer->lerpMotion(MoveVec);
    
}
