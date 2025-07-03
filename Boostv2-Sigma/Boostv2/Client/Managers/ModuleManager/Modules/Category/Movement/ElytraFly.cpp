#include "ElytraFly.h"
static float Speedy = 4.0f;
bool noclip;
bool elytraspoofing;
ElytraFly::ElytraFly() : Module("ElytraFly", "Elytra flight", Category::MOVEMENT) {
    registerSetting(new KeybindSetting("Up Key", "Key for up", &upKey, 0));
    registerSetting(new KeybindSetting("Down Key", "Key for down", &downKey, 0));

    registerSetting(new SliderSetting<float>("Speed", "NULL", &Speed, 25.f, 0.f, 50.f));
    registerSetting(new SliderSetting<float>("Vertical Speed", "NULL", &Speedy, 25.f, 0.f, 50.f));

    registerSetting(new BoolSetting("Noclip", "Noclip flight", &noclip, true));
    registerSetting(new BoolSetting("Auto Take Off", "Auto take off. Useful for noclipping.", &elytraspoofing, true));

}

static Vec2<float> getMotion(float playerYaw, float speed)
{
    //mc.isKeyDown('L'))
    bool w = Game.isKeyDown('W');
    bool a = Game.isKeyDown('A');
    bool s = Game.isKeyDown('S');
    bool d = Game.isKeyDown('D');

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

    float calcYaw = playerYaw * 0.01745329251f; 
    return { cos(calcYaw) * speed / 10.0f,sin(calcYaw) * speed / 10.0f };
}
bool iszoving(bool jumpCheck) {
    if (!Game.canUseMoveKeys()) return false;
    if (Game.isKeyDown('W') || Game.isKeyDown('A') || Game.isKeyDown('S') || Game.isKeyDown('D')) return true;
    if (jumpCheck && Game.isKeyDown(VK_SPACE)) return true;
    return false;
}
void setbeed(float speed) {
    Vec2<float> motion = getMotion(Game.getLocalPlayer()->rotation->presentRot.y, speed);
    if (!iszoving(false)) {
        motion.x = 0.f;
        motion.y = 0.f;
    }
    Game.getLocalPlayer()->stateVector->velocity.x = motion.x;
    Game.getLocalPlayer()->stateVector->velocity.z = motion.y;
}

void ElytraFly::onNormalTick(LocalPlayer* actor) {
    LocalPlayer* localPlayer = Game.getLocalPlayer();
    float flySpeed = Speed;
    bool hasElytraInSlot = localPlayer->getArmor(1)->item && localPlayer->getArmor(1)->item->texture_name == "elytra";
    bool isRightSize = localPlayer->aabbShape->aabb.size.y <= 0.6f;
    Vec3<float> currentPosPtr = Game.getLocalPlayer()->getPos();
    Vec3<float> currentPos = Vec3<float>(currentPosPtr.x, currentPosPtr.y, currentPosPtr.z);
    if (isRightSize && hasElytraInSlot) {
        setbeed(flySpeed);
        if (Game.isKeyDown(upKey)) localPlayer->stateVector->velocity.y = Speedy / 10.0f;
        else if (Game.isKeyDown(downKey)) localPlayer->stateVector->velocity.y = -Speedy / 10.0f;
        else localPlayer->stateVector->velocity.y = 0.025f;
        if (noclip) {
            localPlayer->aabbShape->aabb.upper.z = currentPos.z;
            localPlayer->aabbShape->aabb.upper.x = currentPos.x;
            localPlayer->aabbShape->aabb.lower.z = currentPos.z;
            localPlayer->aabbShape->aabb.lower.x = currentPos.x;
            if (Game.getLocalPlayer() != nullptr) Game.getLocalPlayer()->aabbShape->aabb.upper.y = Game.getLocalPlayer()->aabbShape->aabb.lower.y - (float)1.6f;
        }
        if (!noclip) {
            localPlayer->aabbShape->aabb.lower.x = currentPos.x - 0.35f;
            localPlayer->aabbShape->aabb.lower.z = currentPos.z - 0.35f;
            localPlayer->aabbShape->aabb.upper.x = currentPos.x + 0.35f;
            localPlayer->aabbShape->aabb.upper.z = currentPos.z + 0.35f;
        }
    }
}

void ElytraFly::onSendPacket(Packet* packet) {
    bool hasElytraInSlot = Game.getLocalPlayer()->getArmor(1)->item && Game.getLocalPlayer()->getArmor(1)->item->texture_name == "elytra";
    bool isRightSize = Game.getLocalPlayer()->aabbShape->aabb.size.y <= 0.6f;
    if (isRightSize && hasElytraInSlot) {
        if (noclip) {
            static float offset = 0.0f;
            static bool goingUp = true;
            Vec3<float> currentPosPtr = Game.getLocalPlayer()->getPos();
            Vec3<float> currentPos1 = Vec3<float>(currentPosPtr.x, currentPosPtr.y, currentPosPtr.z);
            Vec3<float> currentPos = Game.getLocalPlayer()->stateVector->pos;

            if (packet->getId() == PacketID::MovePlayer && Game.getLocalPlayer() != nullptr) {
                auto* movePacket = reinterpret_cast<MovePlayerPacket*>(packet);
                movePacket->mPos = currentPos1;
                movePacket->mPos = currentPos;
                movePacket->mOnGround = true;
            }
            if (packet->getId() == PacketID::PlayerAuthInput && Game.getLocalPlayer() != nullptr) {
                auto* authPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
                authPacket->position = currentPos;
                authPacket->position = currentPos1;
                if (Game.isKeyDown(downKey)) authPacket->mAnalogMoveVector.y = -Speedy / 10.0f; //this spoofs that we are always going up [antikick on 2b2e]
            }
        }

        if (elytraspoofing) {
            static float verticalVelocity = 0.0f;
            static bool gliding = true;
            static float glideSpeed = 0.1f;
            Vec3<float> currentPosPtr = Game.getLocalPlayer()->getPos();
            Vec3<float> currentPos = Game.getLocalPlayer()->stateVector->pos;

            if (packet->getId() == PacketID::MovePlayer && Game.getLocalPlayer() != nullptr) {
                auto* movePacket = reinterpret_cast<MovePlayerPacket*>(packet);
                verticalVelocity -= glideSpeed;
                currentPos.y += verticalVelocity;
                movePacket->mOnGround = false;
                movePacket->mPos = currentPos;
            }

            if (packet->getId() == PacketID::PlayerAuthInput && Game.getLocalPlayer() != nullptr) {
                auto* authPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
                authPacket->mInputData |= InputData::StartGliding;
                authPacket->mInputData |= InputData::HandledTeleport;
                authPacket->mInputData |= InputData::StopSprinting;
                authPacket->mInputData |= InputData::StopSneaking;
            }
        }
    }
}

void ElytraFly::onDisable() {
    if (noclip) {
        LocalPlayer* localPlayer = Game.getLocalPlayer();
        Vec3<float> currentPosPtr = Game.getLocalPlayer()->getPos();
        Vec3<float> currentPos = Vec3<float>(currentPosPtr.x, currentPosPtr.y, currentPosPtr.z);
        if (Game.getLocalPlayer() != nullptr) Game.getLocalPlayer()->aabbShape->aabb.upper.y = Game.getLocalPlayer()->aabbShape->aabb.lower.y + (float)1.6f;
        localPlayer->aabbShape->aabb.lower.x = currentPos.x - 0.35f;
        localPlayer->aabbShape->aabb.lower.z = currentPos.z - 0.35f;
        localPlayer->aabbShape->aabb.upper.x = currentPos.x + 0.35f;
        localPlayer->aabbShape->aabb.upper.z = currentPos.z + 0.35f;
    }
}