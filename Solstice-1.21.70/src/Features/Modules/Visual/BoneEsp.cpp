//
// Created by vastrakai on 9/13/2024.
//

#include "BoneEsp.hpp"

#include <Features/Events/BoneRenderEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/Actor/ActorPartModel.hpp>
#include <SDK/Minecraft/Rendering/GuiData.hpp>

void BoneEsp::onEnable()
{
    gFeatureManager->mDispatcher->listen<BoneRenderEvent, &BoneEsp::onBoneRenderEvent>(this);
    gFeatureManager->mDispatcher->listen<RenderEvent, &BoneEsp::onRenderEvent>(this);
}

void BoneEsp::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BoneRenderEvent, &BoneEsp::onBoneRenderEvent>(this);
    gFeatureManager->mDispatcher->deafen<RenderEvent, &BoneEsp::onRenderEvent>(this);
}

void BoneEsp::onRenderEvent(RenderEvent& event)
{
    auto ci = ClientInstance::get();
    auto actors = ActorUtils::getActorList(false, true);
    auto localPlayer = ci->getLocalPlayer();

    for (auto actor : actors)
    {
        if (actor == localPlayer && !mRenderLocal.mValue) continue;

        auto boneList = mBoneMap.find(actor);
        if (boneList == mBoneMap.end()) continue;

        for (auto& [bone, partModel] : boneList->second)
        {
            if (!MemUtils::isValidPtr(reinterpret_cast<uintptr_t>(bone)) || !MemUtils::isValidPtr(reinterpret_cast<uintptr_t>(partModel))) continue;
            glm::vec3 pos = partModel->mPos;
            glm::vec3 size = partModel->mSize;
            glm::vec3 rot = partModel->mRot;


            glm::vec3 playerPos = RenderUtils::transform.mPlayerPos;
            glm::vec3 origin = RenderUtils::transform.mOrigin;

            // example: [21:37:08.432] [info] Bone: rightarm, pos: (-5, -2, 0), size: (1, 1, 1), rot: (-90, 0, 5.4537144)
            // Divide pos by 8
            pos.x /= 16;

            glm::vec3 worldPos = glm::vec3(
                pos.x * size.x,
                pos.y * size.y,
                pos.z * size.z
            );

            worldPos = glm::rotateX(worldPos, glm::radians(rot.x));
            worldPos = glm::rotateY(worldPos, glm::radians(rot.y));
            worldPos = glm::rotateZ(worldPos, glm::radians(rot.z));
            worldPos += playerPos;



            glm::vec2 screenPos;
            if (!RenderUtils::transform.mMatrix.OWorldToScreen(RenderUtils::transform.mOrigin, worldPos, screenPos, MathUtils::fov, ClientInstance::get()->getGuiData()->mResolution)) continue;

            // Draw a circle at the bone's position
            ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(screenPos.x, screenPos.y), 5, IM_COL32(255, 0, 0, 255));

        }
    }
}

void BoneEsp::onBoneRenderEvent(BoneRenderEvent& event)
{
    auto bone = event.mBone;
    auto partModel = event.mPartModel;
    auto actor = event.mActor;

    if (bone->mBoneStr != "rightarm") return;
    mBoneMap[actor].emplace_back(bone, partModel);
}
