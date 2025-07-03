#include "Anchor.h"
int rangeYMinus = 1.f;
bool onIdle = true;
bool cornerClip = false;
bool shouldY = false;
bool shouldLerp = false;
bool holeTP = true;
Anchor::Anchor() : IModule(0x0, Category::MOVEMENT, "More hole options!")
{
    registerBoolSetting("Hole TP", &holeTP, holeTP, "Teleports to nearest hole");
    registerBoolSetting("On Idle", &onIdle, onIdle, "Only tp when not pressing movement keys");
    registerBoolSetting("AutoClip", &cornerClip, cornerClip, "Automatically corner clips");
    registerBoolSetting("Smooth", &shouldLerp, shouldLerp, "Teleports to holes smoothly");
    registerIntSetting("Range", &this->range, 0.f, 0.f, 5.f, "How far a hole should be to tp");
}
const char *Anchor::getRawModuleName()
{
    return ("Hole+");
}
Anchor::~Anchor()
{
}
void Anchor::onTick(C_GameMode* gm)
{
    vec3_ti bestPos;
    if (!(g_Data.getLocalPlayer() == nullptr || !GameData::canUseMoveKeys()))
    {
        if (g_Data.isInGame())
        {
            // Odelay++;
            // if (Odelay > delay * 20) {
            vec3_t* pos = g_Data.getLocalPlayer()->getPos();
            for (int x = (int)pos->x - range; x < pos->x + range; x++)
            {
                for (int z = (int)pos->z - range; z < pos->z + range; z++)
                {
                    for (int y = (int)pos->y - 2; y < pos->y + 1; y++)
                    {
                        vec4_t underline;
                        vec3_t blockPos = vec3_t(x, y, z);
                        vec3_t air = vec3_t(x, y + 1, z);
                        vec3_t air2 = vec3_t(x, y + 2, z);

                        vec3_t blockPos2 = vec3_t(x + 1, y, z);
                        vec3_t blockPos3 = vec3_t(x, y, z + 1);
                        vec3_t blockPos4 = vec3_t(x, y - 1, z);
                        vec3_t blockPos5 = vec3_t(x - 1, y, z);
                        vec3_t blockPos6 = vec3_t(x, y, z - 1);

                        vec3_t blockPos22 = vec3_t(x + 1, y + 1, z);
                        vec3_t blockPos32 = vec3_t(x, y + 1, z + 1);
                        vec3_t blockPos52 = vec3_t(x - 1, y + 1, z);
                        vec3_t blockPos62 = vec3_t(x, y + 1, z - 1);

                        C_Block* block = g_Data.getLocalPlayer()->region->getBlock(blockPos);
                        C_Block* airpos = g_Data.getLocalPlayer()->region->getBlock(air);
                        C_Block* airpos2 = g_Data.getLocalPlayer()->region->getBlock(air2);
                        C_Block* block2 = g_Data.getLocalPlayer()->region->getBlock(blockPos2);
                        C_Block* block3 = g_Data.getLocalPlayer()->region->getBlock(blockPos3);
                        C_Block* block4 = g_Data.getLocalPlayer()->region->getBlock(blockPos4);
                        C_Block* block5 = g_Data.getLocalPlayer()->region->getBlock(blockPos5);
                        C_Block* block6 = g_Data.getLocalPlayer()->region->getBlock(blockPos6);

                        C_Block* block22 = g_Data.getLocalPlayer()->region->getBlock(blockPos22);
                        C_Block* block32 = g_Data.getLocalPlayer()->region->getBlock(blockPos32);
                        C_Block* block52 = g_Data.getLocalPlayer()->region->getBlock(blockPos52);
                        C_Block* block62 = g_Data.getLocalPlayer()->region->getBlock(blockPos62);

                        if (block != nullptr && block2 != nullptr && block3 != nullptr && block4 != nullptr)
                        {
                            int blockId = block->toLegacy()->blockId;
                            int aircheck = airpos->toLegacy()->blockId;
                            int aircheck2 = airpos2->toLegacy()->blockId;
                            int blockId2 = block2->toLegacy()->blockId;
                            int blockId3 = block3->toLegacy()->blockId;
                            int blockId4 = block4->toLegacy()->blockId;
                            int blockId5 = block5->toLegacy()->blockId;
                            int blockId6 = block6->toLegacy()->blockId;

                            int blockId22 = block22->toLegacy()->blockId;
                            int blockId32 = block32->toLegacy()->blockId;
                            int blockId52 = block52->toLegacy()->blockId;
                            int blockId62 = block62->toLegacy()->blockId;

                            if (obsidian)
                            {
                                if (gm->player->onGround && !gm->player->isInWater())
                                {
                                    if ((blockId2 == 49 || blockId2 == 7) && (blockId3 == 49 || blockId3 == 7) && (blockId4 == 49 || blockId4 == 7) && (blockId5 == 49 || blockId5 == 7) && (blockId6 == 49 || blockId6 == 7) && (blockId == 0) && (aircheck == 0) && !((blockId2 == 7 && blockId3 == 7 && blockId4 == 7 && blockId5 == 7 && blockId6 == 7 && blockId == 0) && (aircheck == 0)))
                                    {
                                        bool pressed = MoveUtil::keyPressed();
                                        auto clip = moduleMgr->getModule<AutoClip>();
                                        auto phase = moduleMgr->getModule<Phase>();
                                        vec3_t Pos = vec3_t(floor(g_Data.getLocalPlayer()->getPos()->x), g_Data.getLocalPlayer()->getPos()->y, floor(g_Data.getLocalPlayer()->getPos()->z));
                                        Pos.x = x + 0.5f;
                                        Pos.z = z + 0.5f;
                                        vec3_t Pos2 = vec3_t(floor(g_Data.getLocalPlayer()->getPos()->x), g_Data.getLocalPlayer()->getPos()->y, floor(g_Data.getLocalPlayer()->getPos()->z));
                                        Pos2.x = x + 0.5f;
                                        Pos2.y = y + 1.f;
                                        Pos2.z = z + 0.5f;
                                        if (!pressed && onIdle && !shouldLerp && holeTP)
                                            g_Data.getLocalPlayer()->setPos(Pos);
                                        if (!onIdle && !shouldLerp && holeTP)
                                            g_Data.getLocalPlayer()->setPos(Pos);
                                        if (cornerClip && !pressed)
                                            clip->enabled = true;
                                        if (cornerClip && pressed) {
                                            clip->enabled = false;
                                            phase->enabled = false;
                                        }
                                        if (shouldLerp && holeTP) {
                                            float dist2 = gm->player->getPos()->dist(Pos2);
                                            if (!pressed && onIdle) {
                                                g_Data.getLocalPlayer()->lerpTo(Pos2, vec2_t(1, 1), (int)fmax((int)dist2 * 0.1, 1));
                                                g_Data.getLocalPlayer()->lerpMotion(Pos2);
                                            }
                                            if (!onIdle) {
                                                g_Data.getLocalPlayer()->lerpTo(Pos2, vec2_t(1, 1), (int)fmax((int)dist2 * 0.1, 1));
                                                g_Data.getLocalPlayer()->lerpMotion(Pos2);
                                            }
                                        }
                                    }
                                }
                            }
                            if (bedrock)
                            {
                                if (gm->player->onGround && !gm->player->isInWater())
                                {
                                    if ((blockId2 == 7 && blockId3 == 7 && blockId4 == 7 && blockId5 == 7 && blockId6 == 7 && blockId == 0) && (aircheck == 0))
                                    {
                                        bool pressed = MoveUtil::keyPressed();
                                        auto clip = moduleMgr->getModule<AutoClip>();
                                        auto phase = moduleMgr->getModule<Phase>();
                                        vec3_t Pos = vec3_t(floor(g_Data.getLocalPlayer()->getPos()->x), g_Data.getLocalPlayer()->getPos()->y, floor(g_Data.getLocalPlayer()->getPos()->z));
                                        Pos.x = x + 0.5f;
                                        Pos.z = z + 0.5f;
                                        vec3_t Pos2 = vec3_t(floor(g_Data.getLocalPlayer()->getPos()->x), g_Data.getLocalPlayer()->getPos()->y, floor(g_Data.getLocalPlayer()->getPos()->z));
                                        Pos2.x = x + 0.5f;
                                        Pos2.y = y + 1.f;
                                        Pos2.z = z + 0.5f;
                                        if (!pressed && onIdle && !shouldLerp && holeTP)
                                            g_Data.getLocalPlayer()->setPos(Pos);
                                        if (!onIdle && !shouldLerp && holeTP)
                                            g_Data.getLocalPlayer()->setPos(Pos);
                                        if (cornerClip && !pressed)
                                            clip->enabled = true;
                                        if (cornerClip && pressed) {
                                            clip->enabled = false;
                                            phase->enabled = false;
                                        }
                                        if (shouldLerp && holeTP) {
                                            float dist2 = gm->player->getPos()->dist(Pos2);
                                            if (!pressed && onIdle) {
                                                g_Data.getLocalPlayer()->lerpTo(Pos2, vec2_t(1, 1), (int)fmax((int)dist2 * 0.1, 1));
                                                g_Data.getLocalPlayer()->lerpMotion(Pos2);
                                            }
                                            if (!onIdle) {
                                                g_Data.getLocalPlayer()->lerpTo(Pos2, vec2_t(1, 1), (int)fmax((int)dist2 * 0.1, 1));
                                                g_Data.getLocalPlayer()->lerpMotion(Pos2);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
void Anchor::onMove(C_MoveInputHandler *input)
{
};