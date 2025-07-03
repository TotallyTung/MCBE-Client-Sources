//
// Created by vastrakai on 8/7/2024.
//

#include "ItemESP.hpp"

#include <Features/Events/BaseTickEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/ItemActor.hpp>
#include <SDK/Minecraft/Inventory/Item.hpp>
#include <SDK/Minecraft/Rendering/GuiData.hpp>
#include <Features/Events/BaseTickEvent.hpp>
#include <Features/Modules/Player/InvManager.hpp>

void ItemESP::onEnable()
{
    gFeatureManager->mDispatcher->listen<RenderEvent, &ItemESP::onRenderEvent>(this);
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &ItemESP::onBaseTickEvent>(this);
}

void ItemESP::onDisable()
{
    gFeatureManager->mDispatcher->deafen<RenderEvent, &ItemESP::onRenderEvent>(this);
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &ItemESP::onBaseTickEvent>(this);
}

struct ItemInfo
{
    glm::vec3* pos;
    glm::vec2 size;
    std::string name;
    bool isUseful;
    std::vector<std::pair<std::string, int>> enchants;
};

std::vector<ItemInfo> items;
std::mutex itemMutex;

void ItemESP::onBaseTickEvent(BaseTickEvent& event)
{
    auto player = event.mActor;
    std::lock_guard<std::mutex> lock(itemMutex);

    items.clear();
    auto actors = ActorUtils::getActorsTyped<ItemActor>(ActorType::ItemEntity);

    for (auto actor : actors)
    {
        if (!actor) continue;
        if (!actor->mItem.mItem) continue;
        if (!actor->getStateVectorComponent()) continue;
        if (mDistanceLimited.mValue && player->distanceTo(actor) > mDistance.mValue) continue;

        auto renderPosComp = actor->getRenderPositionComponent();
        if (!renderPosComp) continue;
        auto shape = actor->getAABBShapeComponent();
        if (!shape) continue;

        glm::vec3 pos = renderPosComp->mPosition;
        glm::vec3 origin = RenderUtils::transform.mOrigin;
        float aabbHeight = shape->mHeight;
        float aabbWidth = shape->mWidth;

        ItemStack* stack = &actor->mItem;
        if (!stack->mItem) continue;

        bool isUseful = !InvManager::isItemUseless(stack, -1);
        std::string name = actor->mItem.getItem()->mName;
        if (name.empty()) return;
        name += " x" + std::to_string(stack->mCount);

        std::vector<std::pair<std::string, int>> enchants;
        for (int i = 0; i <= static_cast<int>(Enchant::SWIFT_SNEAK); ++i)
        {
            int enchantValue = stack->getEnchantValue(i);
            if (enchantValue > 0)
            {
                enchants.emplace_back(stack->getEnchantName(static_cast<Enchant>(i)), enchantValue);
            }
        }

        items.push_back({&renderPosComp->mPosition, {aabbWidth, aabbHeight}, name, isUseful, enchants});
    }
}

void ItemESP::onRenderEvent(RenderEvent& event)
{
    std::lock_guard<std::mutex> lock(itemMutex);
    if (!ClientInstance::get()->getLevelRenderer()) return;

    auto ci = ClientInstance::get();
    auto player = ci->getLocalPlayer();
    if (!player) return;
    if (!ci->getLevelRenderer()) return;

    auto drawList = ImGui::GetBackgroundDrawList();

    for (auto actor : items)
    {
        AABB aabb;

        glm::vec3 ppos = *actor.pos;
        ppos = ppos - glm::vec3(actor.size.x / 2, 0, actor.size.x / 2);
        aabb.mMin = ppos;
        aabb.mMax = ppos + glm::vec3(actor.size.x, actor.size.y, actor.size.x);


        std::vector<ImVec2> imPoints = MathUtils::getImBoxPoints(aabb);

        ImColor themeColor = mThemedColor.mValue ? ColorUtils::getThemedColor(0) : ImColor(1.0f, 1.0f, 1.0f);

        if (mRenderFilled.mValue) drawList->AddConvexPolyFilled(imPoints.data(), imPoints.size(), ImColor(themeColor.Value.x, themeColor.Value.y, themeColor.Value.z, 0.25f));
        drawList->AddPolyline(imPoints.data(), imPoints.size(), themeColor, 0, 2.0f);

        glm::vec3 pos = *actor.pos;
        glm::vec3 origin = RenderUtils::transform.mOrigin;
        glm::vec2 screen = glm::vec2(0, 0);

        if (!RenderUtils::transform.mMatrix.OWorldToScreen(origin, pos, screen, ci->getFov(), ci->getGuiData()->mResolution)) continue;

        if (!mShowNames.mValue) continue;
        std::string name = actor.name;

        if (mShowEnchant.mValue && !actor.enchants.empty()) {
            std::string enchantText;
            for (const auto& enchant : actor.enchants) {
                if (!enchantText.empty()) {
                    enchantText += ", ";
                }
                enchantText += "[" + enchant.first + "]";
            }
            name += " " + enchantText;
        }

        FontHelper::pushPrefFont(true, true);

        float fontSize = mFontSize.mValue;

        if (mDistanceScaledFont.mValue)
        {
            // use distance to origin, not actor
            float distance = glm::distance(origin, pos) + 2.5f;
            if (distance < 0) distance = 0;
            fontSize = 1.0f / distance * 100.0f * mScalingMultiplier.mValue;
            if (fontSize < 1.0f) fontSize = 1.0f;
        }

        ImVec2 textSize = ImGui::GetFont()->CalcTextSizeA(fontSize, FLT_MAX, 0, name.c_str());
        ImVec2 textPos = ImVec2(screen.x - textSize.x / 2, screen.y - textSize.y - 5);
        //drawList->AddText(ImGui::GetFont(), fontSize, textPos, ImColor(1.0f, 1.0f, 1.0f, 1.0f), name.c_str());
        ImColor textCol = actor.isUseful && mHighlightUsefulItems.mValue ? ImColor(0.0f, 1.0f, 0.0f) : ImColor(1.0f, 1.0f, 1.0f); // Green for useful and white for others

        ImRenderUtils::drawShadowText(drawList, name, textPos, textCol, fontSize, true);

        FontHelper::popPrefFont();
    }
}