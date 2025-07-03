#include "BetterBow.h"

#include "../../Utils/TargetUtil.h"
#include "../../Utils/DrawUtils.h"

std::vector<C_Entity *> targetList;

BetterBow::BetterBow() : IModule(0, Category::COMBAT, "More bow options")
{
    aimbotType = SettingEnum(this)
                     .addEntry(EnumEntry("Disabled", 0))
                     .addEntry(EnumEntry("Normal", 1));
    registerEnumSetting("Aimbot", &this->aimbotType, 0, "Aim for you");
    registerBoolSetting("Visualize", &visualize, visualize, "Visualize aimbot target");
    registerBoolSetting("BowSpam", &spamBow, spamBow, "Should automatically release the bow at a certain charge");
    registerIntSetting("Charge", &this->charge, this->charge, 1, 20, "The charge for bowspam");
}

BetterBow::~BetterBow()
{
}

struct CompareTargetEnArray
{
    bool operator()(C_Entity *lhs, C_Entity *rhs)
    {
        C_LocalPlayer *localPlayer = g_Data.getLocalPlayer();
        return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
    }
};

const char *BetterBow::getRawModuleName()
{
    return ("BetterBow");
}

void findTargets(C_Entity *currentEntity, bool isRegularEntitie)
{
    if (!TargetUtil::isValidTarget(currentEntity))
        return;

    float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

    if (dist < 130)
    {
        targetList.push_back(currentEntity);
    }
}
void BetterBow::onTick(C_GameMode *gm)
{
    auto player = g_Data.getLocalPlayer();
    if (player == nullptr)
        return;

    auto itemInHand = player->getSelectedItem();

    if (g_Data.isLeftClickDown && (player->getSelectedItemId() == 300) && spamBow)
    {
        i++;
        // clientMessageF("Bow use: %f", i);

        if (i >= charge)
        {
            g_Data.getCGameMode()->releaseUsingItem();
            i = 0;
            if (shotOnce)
                player->completeUsingC_Item();
        }
        shotOnce = true;
    }
    shotOnce = false;
}

void BetterBow::onPostRender(C_MinecraftUIRenderContext *renderCtx)
{
    targetList.clear();

    C_LocalPlayer *localPlayer = g_Data.getLocalPlayer();
    if (localPlayer == nullptr)
        return;

    if (localPlayer->getSelectedItemId() != 300) // Bow in hand?
        return;

    if (!(GameData::isRightClickDown() && GameData::canUseMoveKeys())) // is aiming?
        return;

    g_Data.forEachEntity(findTargets);

    if (!targetList.empty())
    {
        std::sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
        vec3_t origin = g_Data.getLocalPlayer()->eyePos0; // TODO: sort list
        C_Entity *entity = targetList[0];
        vec3_t pos = entity->aabb.centerPoint();
        if (aimbotType.getSelectedValue() == 2 && !aimbotType.getSelectedValue() == 0)
        {
            vec3_t velocity = entity->getPos()->sub(*entity->getPosOld());
            velocity.x *= origin.dist(pos) / 2.f;
            velocity.z *= origin.dist(pos) / 2.f;
            pos = pos.add(velocity);
        }
        if (visualize)
            auto interfaceColor = moduleMgr->getModule<Interface>();
        auto interfaceColor = ColorUtil::interfaceColor(1);
        DrawUtils::setColor(interfaceColor.r, interfaceColor.g, interfaceColor.b, 1); // retarded
        DrawUtils::drawBox(pos.sub(0.5), pos.add(0.5), 0.3f, true);
        pos = pos.sub(origin);
        float yaw = (atan2f(pos.z, pos.x) * DEG_RAD) - 90;
        float len = pos.magnitudexz();
        constexpr float g = 0.002f; // nukkit = 0.012, some servers need different values
        float tmp = 1 - g * (g * (len * len) + 2 * pos.y);
        float pitch = DEG_RAD * -atanf((1 - sqrtf(tmp)) / (g * len));

        if (aimbotType.getSelectedValue() == 3 && !aimbotType.getSelectedValue() == 0)
        {
            angle = vec2_t(pitch, yaw);
            C_MovePlayerPacket p(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
            p.pitch = angle.x;
            p.yaw = angle.y;
            p.headYaw = angle.y;
            g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p);
        }
        else if (aimbotType.getSelectedValue() == 1 && !aimbotType.getSelectedValue() == 0)
        {
            if (pitch < 89 && pitch > -89)
            {
                vec2_t angles = vec2_t(pitch, yaw);

                vec2_t appl = angles.sub(localPlayer->viewAngles).normAngles();
                appl.x = -appl.x;
                appl = appl.div(7); // Smooth dat boi

                localPlayer->applyTurnDelta(&appl);
            }
        }
    }
}

void BetterBow::onSendPacket(C_Packet *packet)
{
    if (packet->isInstanceOf<C_MovePlayerPacket>() && aimbotType.getSelectedValue() == 3)
    {
        if (!targetList.empty())
        {
            auto *movePacket = reinterpret_cast<C_MovePlayerPacket *>(packet);
            movePacket->pitch = angle.x;
            movePacket->headYaw = angle.y;
            movePacket->yaw = angle.y;
        }
    }
}
