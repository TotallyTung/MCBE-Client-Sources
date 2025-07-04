#include "ElytraFly.h"
static float Speedy = 4.0f;
ElytraFly::ElytraFly() : Module("ElytraFly", "Extend reach.", Category::MOVEMENT) {
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
static __int64 ms;
static __int64 prevMS = 0;
static __int64 timeMS = -1;
static __int64 getCurrentMs() {
    FILETIME f;
    GetSystemTimeAsFileTime(&f);
    (long long)f.dwHighDateTime;
    __int64 nano = ((__int64)f.dwHighDateTime << 32LL) + (__int64)f.dwLowDateTime;
    return (nano - 116444736000000000LL) / 10000;
}
static __int64 lastMS = getCurrentMs();
static bool hasTimePassed(__int64 ms) {
    return getCurrentMs() >= timeMS + ms;
}

static __int64 getTime() {
    return getCurrentMs();
}

static void reset2() {
    prevMS = getTime();
}

static __int64 getDifference() {
    return getTime() - prevMS;
}

static void setDifference(__int64 difference) {
    prevMS = getTime() - difference;
}

static void resetTime() {
    lastMS = getCurrentMs();
    timeMS = getCurrentMs();
}

static __int64 getElapsedTime1() {
    return (getCurrentMs() - timeMS);
}

static bool hasTimedElapsed(__int64 time, bool reset) {
    if (getCurrentMs() - lastMS > time) {
        if (reset)
            resetTime();
        return true;
    }
    return false;
}

static void reset1() {
    ms = getCurrentMs();
}

static bool elapsed(__int64 milliseconds) {
    return (getCurrentMs() - ms) > milliseconds;
}

static __int64 getElapsedTime() {
    return getCurrentMs() - ms;
}
//Vec2<float> rotanglesafe = Vec2<float>(36.636612,45.005440); <- i found this value when killaura strafe and glide value 0.05
void ElytraFly::onNormalTick(Actor* actor) {
    LocalPlayer* localPlayer = mc.getLocalPlayer();
    float flySpeed = Speed;
    Vec2<float> motion = getMotion(localPlayer->rotationComponent->Get().y, flySpeed);
    bool hasElytraInSlot = localPlayer->getArmor(1)->item && localPlayer->getArmor(1)->getItemPtr()->texture_name == "elytra";  // Elytra check
    bool isRightSize = localPlayer->aabbComponent->height <= 0.6f; // Only 2 places where your hitbox height goes below 0.6f is when swimming and using the elytra
   // float yr = localPlayer->rotationComponent->Get().y;
    //float xr = localPlayer->rotationComponent->Get().x;
    bool w = mc.isKeyDown('W');
    bool a = mc.isKeyDown('A');
    bool s = mc.isKeyDown('S');
    bool d = mc.isKeyDown('D');
    bool usingMoveKeys = w || a || s || d;

    if (isRightSize && hasElytraInSlot) {
        // If move keys are not pressed, set motion to zero
        if (!usingMoveKeys) {
            motion.x = 0.0f;
            motion.y = 0.0f;
        }

        // Apply motion
        localPlayer->stateVectorComponent->velocity.x = motion.x;
        localPlayer->stateVectorComponent->velocity.z = motion.y;
       /* auto rotAngle = localPlayer->getPosition()->CalcAngle(localPlayer->getPosition()->sub(Vec3<float>(0,0,-1)));
        localPlayer->rotationComponent->rotation = rotAngle;
        localPlayer->rotationComponent->Set(rotAngle);
        localPlayer->rotationComponent->oldRotation = *localPlayer->getRotationPrev();*/
        // Set Y velocity based on key presses
        if (mc.isKeyDown(VK_TAB)) {
            // If TAB key is pressed, fly upwards
            localPlayer->stateVectorComponent->velocity.y = Speedy / 10.0f;
        }
        else if (mc.isKeyDown(VK_SHIFT)) {
            // If SHIFT key is pressed, fly downwards
            localPlayer->stateVectorComponent->velocity.y = -Speedy / 10.0f;
        }
        else {
            // Otherwise, maintain constant Y velocity (no gliding down)
            localPlayer->stateVectorComponent->velocity.y = 0.00f;
        }

        
    }
   /*char message[256];  // Adjust the buffer size as needed
   char message2[256];
    sprintf(message, "yrot = %f", yr);
    sprintf(message2, "xrot = %f", xr);
    mc.DisplayClientMessage(message);
    mc.DisplayClientMessage(message2);*/
}
// aabb height 1.49f la sneaking;
//aabb 0.2f la sleeping in bed;
//aabb 0.6f la swinming;
//aabb elytrafly la 0.6
