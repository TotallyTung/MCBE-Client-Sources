#include "Burrow.h"

int testextend2 = 0;
int PastedRadiumDelaySKULL2 = 0;
int burrowSlot = 0;
using namespace std;
Burrow::Burrow() : IModule(0, Category::PLAYER, "Burrow with any block")
{
    registerEnumSetting("Mode", &mode, 0);
    mode.addEntry("Clip", 0);
    mode.addEntry("LagBack", 1);
    registerEnumSetting("Rotation", &rotation, 0);
    rotation.addEntry("None", 0);
    rotation.addEntry("Packet", 1);
    registerEnumSetting("Priority", &blockPriority, 0, "What block should we prioritize");
    blockPriority.addEntry("Smart", 0);
    blockPriority.addEntry("Fixed", 1);
    registerEnumSetting("Swap", &swap, 0);
    swap.addEntry("None", 0);
    swap.addEntry("NukkitSpoof", 1);
    registerBoolSetting("Only Ground", &groundOnly, groundOnly, "Only work when on ground");
    registerBoolSetting("Smooth", &lerping, lerping, "Make burrow SMOOTH");
    registerBoolSetting("Disable", &disableComplete, disableComplete, "Disable when completed");
}

const char *Burrow::getRawModuleName()
{
    return "Burrow";
}

void Burrow::validBlock() {
    C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
    C_Inventory* inv = supplies->inventory;
    for (int n = 0; n < 9; n++) {
        C_ItemStack* stack = inv->getItemStack(n);
        if (stack->item != nullptr) {
            // Smart
            if (blockPriority.getSelectedValue() == 0) {
                // Block is Obby
                if (blockType.getSelectedValue() == 0) {
                    // If theres no obby but there is echest
                    if (stack->getItem()->itemId != 49 && stack->getItem()->itemId == 130) {
                        supplies->selectedHotbarSlot = n;
                        return;
                    }
                    // if theres obby
                    else if (stack->getItem()->itemId == 49) {
                        supplies->selectedHotbarSlot = n;
                        return;
                    }
                }
                else if (blockType.getSelectedValue() == 1) {
                    // If theres no echest but there is obby
                    if (stack->getItem()->itemId != 130 && stack->getItem()->itemId == 49) {
                        supplies->selectedHotbarSlot = n;
                        return;
                    }
                    // if theres echest
                    else if (stack->getItem()->itemId == 130) {
                        supplies->selectedHotbarSlot = n;
                        return;
                    }
                }
            }
            // Fixed
            if (blockPriority.getSelectedValue() == 1) {
                // Block is Obby
                if (blockType.getSelectedValue() == 0 && stack->getItem()->itemId == 49) {
                    supplies->selectedHotbarSlot = n;
                    return;
                }
                // Block is echest
                else if (blockType.getSelectedValue() == 1 && stack->getItem()->itemId == 130) {
                    supplies->selectedHotbarSlot = n;
                    return;
                }
            }
        }
    }
}
void Burrow::activate() {
    // Go up, place block under, go down
    bool tpDown = false;
    int tpDelay = 0;
    C_GameMode* gm = g_Data.getCGameMode();
    vec3_t position = vec3_t(floor(g_Data.getLocalPlayer()->getPos()->x), g_Data.getLocalPlayer()->getPos()->y, floor(g_Data.getLocalPlayer()->getPos()->z));
    vec3_t position2 = vec3_t(floor(g_Data.getLocalPlayer()->getPos()->x), g_Data.getLocalPlayer()->getPos()->y, floor(g_Data.getLocalPlayer()->getPos()->z));
    position.y += 1;
    position2.y -= 1;
    auto pos = g_Data.getLocalPlayer()->getPos();
    auto flooredPos = pos->floor();
    auto buildPos = flooredPos.add(0.f, -1.f, 0.f);
    C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
    C_Inventory* inv = supplies->inventory;
    burrowSlot = supplies->selectedHotbarSlot;
    if (!lerping)
    g_Data.getLocalPlayer()->setPos(position);
    else {
        float dist2 = g_Data.getLocalPlayer()->getPos()->dist(position);
        g_Data.getLocalPlayer()->lerpTo(position, vec2_t(1, 1), (int)fmax((int)dist2 * 0.1, 1));
        g_Data.getLocalPlayer()->lerpMotion(position);
    }
    validBlock();
    gm->buildBlock(&vec3_ti(buildPos), 0);
    tpDown = true;
    if (tpDown) {
        tpDelay++;
        if (tpDelay >= 2) {
            if (!lerping)
                g_Data.getLocalPlayer()->setPos(position2);
            else {
                float dist2 = g_Data.getLocalPlayer()->getPos()->dist(position2);
                g_Data.getLocalPlayer()->lerpTo(position2, vec2_t(1, 1), (int)fmax((int)dist2 * 0.1, 1));
                g_Data.getLocalPlayer()->lerpMotion(position2);
            }
        }
    }
    if (swap.getSelectedValue() == 1) {
        g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot = burrowSlot;
    }
}
void Burrow::onTick(C_GameMode *gm)
{
    if (mode.getSelectedValue() == 0)
        gm->player->aabb.upper.y = gm->player->aabb.lower.y;
    activate();
    if (disableComplete)
    setEnabled(false);
}
void Burrow::onMove(C_MoveInputHandler* input) {
}
void Burrow::onSendPacket(C_Packet *packet)
{
}
void Burrow::onDisable()
{
    if (g_Data.getLocalPlayer() != nullptr && mode.getSelectedValue() == 0)
        g_Data.getLocalPlayer()->aabb.upper.y += 1.8f;
}
