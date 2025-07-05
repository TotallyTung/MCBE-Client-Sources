//
// Created by vastrakai on 4/17/2024.
//

#include <Prax/SDK/Game/Entity/Components/ActorTypeComponent.h>
#include <Prax/SDK/Game/Entity/Components/ActorOwnerComponent.h>
#include <Prax/Utils/Math.h>
#include <Prax/Utils/Render/Font.h>
#include <Prax/Utils/Render/Render.h>
#include <Prax/Module/Modules/Player/InventoryManager.h>
#include <Prax/Module/ModuleManager.h>
#include "ItemESP.h"

int ItemESP::BestHelmetValue = 0;
int ItemESP::BestChestplateValue = 0;
int ItemESP::BestLeggingsValue = 0;
int ItemESP::BestBootsValue = 0;
int ItemESP::BestSwordValue = 0;
int ItemESP::BestPickaxeValue = 0;
int ItemESP::BestAxeValue = 0;
int ItemESP::BestShovelValue = 0;

bool ItemESP::ShouldHighlightItem(ItemStack* stack) {
    if (!HighlightBetterItems.Enabled) return false;
    if (!stack || !stack->item) return false;
    Item* item = *stack->item;

    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return false;
    auto supplies = player->supplies;

    if (item->IsArmor()) {
        int value = item->getArmorValue();
        if (stack->compoundTag) value += stack->getEnchantValue(Enchant::PROTECTION);

        if (item->IsHelmet()) return value > BestHelmetValue;
        if (item->IsChestplate()) return value > BestChestplateValue;
        if (item->IsLeggings()) return value > BestLeggingsValue;
        if (item->IsBoots()) return value > BestBootsValue;

    } else if (item->IsTool()) {
        int value = supplies->GetToolItemValue(stack);
        if (item->IsSword() && stack->compoundTag) value += stack->getEnchantValue(Enchant::SHARPNESS);
        else if (stack->compoundTag) value += stack->getEnchantValue(Enchant::EFFICIENCY);

        if (item->IsSword()) return value > BestSwordValue;
        if (item->IsPickaxe()) return value > BestPickaxeValue;
        if (item->IsAxe()) return value > BestAxeValue;
        if (item->IsShovel()) return value > BestShovelValue;
    }

    return false;
}

void ItemESP::OnTick(){
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    if (!HighlightBetterItems.Enabled) return;

    auto supplies = player->supplies;

    int helmetValue = -1;
    int chestplateValue = -1;
    int leggingsValue = -1;
    int bootsValue = -1;

    ItemStack* helmet = player->getArmor(ArmorSlot::Helmet);
    ItemStack* chestplate = player->getArmor(ArmorSlot::Chestplate);
    ItemStack* leggings = player->getArmor(ArmorSlot::Leggings);
    ItemStack* boots = player->getArmor(ArmorSlot::Boots);

    if (helmet->item) helmetValue = (*helmet->item)->getArmorValue() + helmet->getEnchantValue(Enchant::PROTECTION);
    if (chestplate->item) chestplateValue = (*chestplate->item)->getArmorValue() + chestplate->getEnchantValue(Enchant::PROTECTION);
    if (leggings->item) leggingsValue = (*leggings->item)->getArmorValue() + leggings->getEnchantValue(Enchant::PROTECTION);
    if (boots->item) bootsValue = (*boots->item)->getArmorValue() + boots->getEnchantValue(Enchant::PROTECTION);

    BestHelmetValue = helmetValue;
    BestChestplateValue = chestplateValue;
    BestLeggingsValue = leggingsValue;
    BestBootsValue = bootsValue;

    int swordValue = -1;
    int pickaxeValue = -1;
    int axeValue = -1;
    int shovelValue = -1;

    for (int i = 0; i < 36; i++) {
        ItemStack* stack = supplies->inventory->getItem(i);
        if (!stack->item) continue;

        Item* item = *stack->item;
        if (item->IsSword()) {
            int value = supplies->GetToolItemValue(stack) + stack->getEnchantValue(Enchant::SHARPNESS);
            if (value > swordValue) swordValue = value;
        } else if (item->IsPickaxe()) {
            int value = supplies->GetToolItemValue(stack) + stack->getEnchantValue(Enchant::EFFICIENCY);
            if (value > pickaxeValue) pickaxeValue = value;
        } else if (item->IsAxe()) {
            int value = supplies->GetToolItemValue(stack) + stack->getEnchantValue(Enchant::EFFICIENCY);
            if (value > axeValue) axeValue = value;
        } else if (item->IsShovel()) {
            int value = supplies->GetToolItemValue(stack) + stack->getEnchantValue(Enchant::EFFICIENCY);
            if (value > shovelValue) shovelValue = value;
        }
    }

    BestSwordValue = swordValue;
    BestPickaxeValue = pickaxeValue;
    BestAxeValue = axeValue;
    BestShovelValue = shovelValue;




}

void ItemESP::DoRender(Actor* actor) {
    auto player = Minecraft::ClientInstance->ThePlayer();
    ItemActor* itemActor = (ItemActor*)actor;
    if (!itemActor) return;
    ItemStack* stack = &itemActor->mItem;
    if (!stack || !stack->item) return;
    Item* item = *stack->item;




    Vector3 pos = *actor->GetRenderPositionComponent() - Vector3(0.f, 1.62f, 0.f);
    if (!actor->isPlayer())
    {
        pos = *actor->GetRenderPositionComponent();
    }

    if (DistanceLimited.Enabled && player->getPos()->Distance(pos) > Distance.Value) return;

    float hitboxWidth = actor->GetAABBShapeComponent()->hitboxWidth;
    float hitboxHeight = actor->GetAABBShapeComponent()->hitboxHeight;
    // since the pos is in the center of the hitbox
    Vector3 aabbMin = Vector3(pos.x - hitboxWidth / 2, pos.y, pos.z - hitboxWidth / 2);
    Vector3 aabbMax = Vector3(pos.x + hitboxWidth / 2, pos.y + hitboxHeight, pos.z + hitboxWidth / 2);

    // Slightly increase the size of the AABB so it matches the actual hitbox
    aabbMin = aabbMin - Vector3(0.1f, 0.1f, 0.1f);
    aabbMax = aabbMax + Vector3(0.1f, 0.1f, 0.1f);


    AABB aabb = AABB(aabbMin, aabbMax, true);

    ImColor color = ImColor(255, 255, 255, 255);

    if (ShouldHighlightItem(stack)) {
        Math::DrawOutlinedBox(aabb.lower, aabb.upper, 1.0f, ImColor(0, 255, 0, 255));
        color = ImColor(0, 255, 0, 255);
    }
    else {
        Math::DrawOutlinedBox(aabb.lower, aabb.upper, 1.0f, ImColor(255, 255, 255, 255));
    }


    std::vector<Vector2> points = Math::GetHexForAABB(aabb);
    float yVal = FLT_MAX;
    float xVal = FLT_MAX;
    for (auto&& point : points) {
        if (point.y < yVal) yVal = point.y;
        if (point.x < xVal) xVal = point.x;
    }

    if (points.empty()) return;



    // Draw the name as well with ImGui (get pos based on above points)
    ImVec2 pos2d = ImVec2(xVal, yVal);

    std::string name = item->GetName();
    if (name.empty()) return;
    name += " x" + std::to_string(stack->amount);

    if (stack->compoundTag && color.Value.x != 0) {
        // purple for enchanted items
        color = ImColor(255, 0, 255, 255);
    }

    int font_size = FontSize.Value;

    //  Render
    ImGui::PushFont(Font::Fonts["greycliff_demibold_large"]);
    ImVec2 textSize = ImGui::GetFont()->CalcTextSizeA(font_size, FLT_MAX, 0, name.c_str());
    ImVec2 textPos = ImVec2(pos2d.x - textSize.x / 2, pos2d.y - textSize.y / 2);
    Render::RenderTextColored(name, textPos, font_size, color, false);

    ImGui::PopFont();
}

void ItemESP::OnRender() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    for (auto&& [_, moduleOwner, type] : player->entityCtx.registry->view<ActorOwnerComponent, ActorTypeComponent>().each()) {
        auto actor = moduleOwner.actor;
        if (player == actor) continue;

        if (type.type == ActorType::ItemEntity) {
            if (DistanceLimited.Enabled && player->getPos()->Distance(*actor->GetRenderPositionComponent()) > Distance.Value) continue;
            DoRender(actor); // this will crash sometimes, fix it yourself :)
        }
    }
}
