#include "PacketFly.h"

using namespace std;

PacketFly::PacketFly() : IModule(0x0, Category::MOVEMENT, "Flies using advanced packets, this is like NoClip but more advanced")
{
    registerEnumSetting("Mode", &mode, 0, "Controls how you should move");
    mode.addEntry("Factor", 0);
    mode.addEntry("Slow", 1);
    mode.addEntry("XZ", 2);
    registerEnumSetting("Phase", &phaseType, 0, "Controls how you phase");
    phaseType.addEntry("Full", 0);
    phaseType.addEntry("No Y", 1);
    registerEnumSetting("Timer", &timer, 0, "Should use timer");
    timer.addEntry("None", 0);
    timer.addEntry("IGN", 1);
    timer.addEntry("Vanilla", 2);
    registerFloatSetting("Factor Speed", &this->speed, this->speed, 0.1f, 1.f, 0.1f, "(If factor) Controls the movement speed");
    registerFloatSetting("X Speed", &this->Xspeed, this->Xspeed, 0.1f, 1.f, 0.1f, "(If XZ) Controls X Speed");
    registerFloatSetting("Z Speed", &this->Zspeed, this->Zspeed, 0.1f, 1.f, 0.1f, "(If XZ) Controls Z Speed");
    registerFloatSetting("Up Speed", &this->up, this->up, 0.1f, 1.f, 0.1f, "Controls Y Speed");
    registerFloatSetting("Down Speed", &this->down, this->down, 0.1f, 1.f, 0.1f, "Controls -Y Speed");
    registerIntSetting("Timer TPS", &vanillaTPS, vanillaTPS, 1, 30, "TPS for timer");
    registerBoolSetting("Render", &this->trails, this->trails, "Should render");
    registerBoolSetting("AntiKick", &this->antikick, this->antikick, "Should glide");
}

PacketFly::~PacketFly()
{
}

const char *PacketFly::getRawModuleName()
{
    return ("PacketFly");
}
const char *PacketFly::getModuleName()
{
    switch (mode.getSelectedValue())
    {
    case 0:
        name = string("PacketFly ") + string(GRAY) + string("Factor");
        break;
    case 1:
        name = string("PacketFly ") + string(GRAY) + string("Slow");
        break;
    case 2:
        name = string("PacketFly ") + string(GRAY) + string("XZ");
        break;
    }
    return name.c_str();
}

void PacketFly::onTick(C_GameMode *gm)
{
    gm->player->aabb.upper.y = gm->player->aabb.lower.y - (float)1.8f;
    gm->player->velocity = vec3_t(0, 0, 0);
    glideModEffective = -0.01;
    float oldGlide = 0.00;
    C_GameSettingsInput *input = g_Data.getClientInstance()->getGameSettingsInput();

    if (g_Data.canUseMoveKeys() && antikick && !phaseType.getSelectedValue() == 1)
    {
        if (GameData::isKeyDown(*input->spaceBarKey))
            glideModEffective += up / 2.5;
        if (GameData::isKeyDown(*input->sneakKey))
            glideModEffective -= down / 2.5;
    }
    if (g_Data.canUseMoveKeys() && !antikick && !phaseType.getSelectedValue() == 1)
    {
        if (GameData::isKeyDown(*input->spaceBarKey))
            oldGlide += up / 2.5;
        if (GameData::isKeyDown(*input->sneakKey))
            oldGlide -= down / 2.5;
    }
    if (antikick && !phaseType.getSelectedValue() == 1)
    {
        gm->player->velocity.y = glideModEffective;
    }
    if (!antikick && !phaseType.getSelectedValue() == 1)
    {
        gm->player->velocity.y = oldGlide; // aidzing
    }
    if (timer.getSelectedValue() == 2)
    {
        g_Data.getClientInstance()->minecraft->setTimerSpeed(vanillaTPS);
    }
    if (timer.getSelectedValue() == 1)
    {
        g_Data.getClientInstance()->minecraft->setTimerSpeed(26.f); // IGN Bypass
    }
    auto player = g_Data.getLocalPlayer();
    vec3_t eyePos = player->eyePos0;
    eyePos.y = eyePos.y - 1.5f;
    blockBelow.push_back(eyePos);
}

void PacketFly::onMove(C_MoveInputHandler *input)
{
    auto player = g_Data.getLocalPlayer();
    if (player == nullptr)
        return;

    float yaw = player->yaw;

    bool pressed = input->forward || input->backward || input->right || input->left;

    if (input->forward && input->backward)
        return;

    if (input->right)
    {
        yaw += 90.f;
        if (input->forward)
            yaw -= 45.f;
        else if (input->backward)
            yaw += 45.f;
    }
    if (input->left)
    {
        yaw -= 90.f;
        if (input->forward)
            yaw += 45.f;
        else if (input->backward)
            yaw -= 45.f;
    }

    if (input->backward && !input->left && !input->right)
        yaw += 180.f;

    float calcYaw = (yaw + 90) * (PI / 180);
    vec3_t moveVec;

    // Factor
    if (mode.getSelectedValue() == 0)
    {
        moveVec.x = cos(calcYaw) * speed / 5;
        moveVec.z = sin(calcYaw) * speed / 5;
    }

    // Slow
    if (mode.getSelectedValue() == 1)
    {
        moveVec.x = cos(calcYaw) * 0.3f / 6;
        moveVec.z = sin(calcYaw) * 0.3f / 6;
    }

    // XZ
    if (mode.getSelectedValue() == 2)
    {
        moveVec.x = cos(calcYaw) * Xspeed / 4;
        moveVec.z = sin(calcYaw) * Zspeed / 4;
    }

    // Phase Type
    if (phaseType.getSelectedValue() == 0)
    {
        moveVec.y = player->velocity.y;
    }
    if (phaseType.getSelectedValue() == 1)
    {
        moveVec.y = 0;
    }
    if (pressed)
        player->lerpMotion(moveVec);
}

void PacketFly::onEnable()
{
    if (NoPacket)
        if (g_Data.getLocalPlayer() != nullptr)
        {
            oldPos = *g_Data.getLocalPlayer()->getPos();
        }
}
void PacketFly::onLevelRender()
{
    auto player = g_Data.getLocalPlayer();
    if (player == nullptr)
        return;
    auto i = ColorUtil::interfaceColor(1);

    DrawUtils::setColor(i.r, i.g, i.b, 1);
    if (trails)
    {
        DrawUtils::drawLineStrip3D(blockBelow, 5);
    }
}
void PacketFly::onDisable()
{
    if (g_Data.getLocalPlayer() != nullptr)
        g_Data.getLocalPlayer()->aabb.upper.y = g_Data.getLocalPlayer()->aabb.lower.y + (float)1.8f;
    g_Data.getClientInstance()->minecraft->setTimerSpeed(20.f);
    if (NoPacket)
        g_Data.getLocalPlayer()->setPos(oldPos);
    blockBelow.clear();
}
