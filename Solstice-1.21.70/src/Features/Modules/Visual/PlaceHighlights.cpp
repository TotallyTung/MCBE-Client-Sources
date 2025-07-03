#include "PlaceHighlights.hpp"

#include <Features/FeatureManager.hpp>
#include <Features/Events/PacketOutEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/Actor/GameMode.hpp>
#include <SDK/Minecraft/Inventory/PlayerInventory.hpp>
#include <SDK/Minecraft/World/Level.hpp>
#include <SDK/Minecraft/World/HitResult.hpp>
#include <SDK/Minecraft/Network/Packets/InventoryTransactionPacket.hpp>

void PlaceHighlights::onEnable()
{
    gFeatureManager->mDispatcher->listen<RenderEvent, &PlaceHighlights::onRenderEvent>(this);
    gFeatureManager->mDispatcher->listen<PacketOutEvent, &PlaceHighlights::onPacketOutEvent>(this);
}

void PlaceHighlights::onDisable()
{
    gFeatureManager->mDispatcher->deafen<RenderEvent, &PlaceHighlights::onRenderEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketOutEvent, &PlaceHighlights::onPacketOutEvent>(this);
}

void PlaceHighlights::onRenderEvent(RenderEvent& event)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player || ClientInstance::get()->getMouseGrabbed() || mPlaceMap.empty()) return;

    auto drawList = ImGui::GetBackgroundDrawList();
    ImColor themedColor = ColorUtils::getThemedColor(0);

    std::vector<glm::ivec3> removeQueued;

    for (auto& it : mPlaceMap)
    {
        glm::ivec3 blockPos = it.first;
        uint64_t PlacedTime = it.second;
        float alpha = mMaxOpacity.mValue - ((float)(NOW - PlacedTime) / mDuration.mValue);
        int alphaInt = (int)(alpha * 200);

        if (alphaInt < 0) {
            removeQueued.emplace_back(PlacedTime);
            continue;
        }

        auto boxSize = glm::vec3(1, 1, 1);
        auto boxAABB = AABB(blockPos, boxSize);
        ImColor cColor = themedColor;
        if (mColorMode.mValue == ColorMode::Custom) cColor = mBoxColor.getAsImColor();
        cColor.Value.w = alpha;

        std::vector<ImVec2> imPoints = MathUtils::getImBoxPoints(boxAABB);

        if (mFilled.mValue) drawList->AddConvexPolyFilled(imPoints.data(), imPoints.size(), cColor);
        drawList->AddPolyline(imPoints.data(), imPoints.size(), cColor, 0, 2.0f);
    }

    if (!removeQueued.empty())
    {
        for (auto& it : removeQueued)
        {
            mPlaceMap.erase(it);
        }
    }
}

void PlaceHighlights::onPacketOutEvent(PacketOutEvent& event)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    if (event.mPacket->getId() == PacketID::InventoryTransaction)
    {
        if (const auto it = event.getPacket<InventoryTransactionPacket>(); it->mTransaction->type ==
            ComplexInventoryTransaction::Type::ItemUseTransaction)
        {
            const auto transac = reinterpret_cast<ItemUseInventoryTransaction*>(it->mTransaction.get());
            if (transac->mActionType == ItemUseInventoryTransaction::ActionType::Place)
            {
                ItemStack* holdingItem = player->getSupplies()->getContainer()->getItem(transac->mSlot);
                if (holdingItem->mItem && holdingItem->mBlock)
                {
                    int face = transac->mFace;
                    if (face < 0 || 5 < face) {
                        return;
                    }
                    mPlaceMap[transac->mBlockPos + mOffsetList[transac->mFace]] = NOW;
                }
            }
        }
    }
}
