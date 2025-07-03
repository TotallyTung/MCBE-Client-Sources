//
// Created by vastrakai on 8/10/2024.
//

#include "Nametags.hpp"

#include <Features/Events/NametagRenderEvent.hpp>
#include <Features/IRC/IrcClient.hpp>
#include <Features/Modules/Misc/Friends.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/MinecraftSim.hpp>
#include <SDK/Minecraft/Options.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/Actor/Components/FlagComponent.hpp>
#include <SDK/Minecraft/Rendering/GuiData.hpp>

void Nametags::onEnable()
{
    gFeatureManager->mDispatcher->listen<RenderEvent, &Nametags::onRenderEvent>(this);
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &Nametags::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->listen<NametagRenderEvent, &Nametags::onNametagRenderEvent>(this);
}
void Nametags::onDisable()
{
    gFeatureManager->mDispatcher->deafen<RenderEvent, &Nametags::onRenderEvent>(this);
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &Nametags::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->deafen<NametagRenderEvent, &Nametags::onNametagRenderEvent>(this);

    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;
}
std::mutex bpsMutex;
std::unordered_map<Actor*, float> bpsMap;
std::unordered_map<Actor*, std::map<int64_t, float>> bpsHistory;
std::unordered_map<Actor*, float> avgBpsMap;

void Nametags::onBaseTickEvent(BaseTickEvent& event)
{
    std::lock_guard<std::mutex> lock(bpsMutex);

    auto actors = ActorUtils::getActorList(true, true);
    auto localPlayer = ClientInstance::get()->getLocalPlayer();
    for (auto actor : actors)
    {
        if (!actor->isPlayer()) continue;
        if (actor == localPlayer && !mRenderLocal.mValue) continue;
        auto shape = actor->getAABBShapeComponent();
        if (!shape) continue;
        auto posComp = actor->getRenderPositionComponent();
        if (!posComp) continue;

        static std::map<Actor*, glm::vec3> prevPosMap;
        glm::vec3 p = *actor->getPos();
        glm::vec3 prevPos = prevPosMap.contains(actor) ? prevPosMap[actor] : p;
        prevPosMap[actor] = p;

        glm::vec2 posxz = { p.x, p.z };
        glm::vec2 prevPosxz = { prevPos.x, prevPos.z };
        float bps = glm::distance(posxz, prevPosxz) * (ClientInstance::get()->getMinecraftSim()->getSimTimer() * ClientInstance::get()->getMinecraftSim()->getSimSpeed());
        if (!bpsHistory.contains(actor)) bpsHistory[actor] = {};

        std::map<int64_t, float>& history = bpsHistory[actor];
        history[NOW] = bps;
        // Remove entries from more than 1 second ago
        for (auto it = history.begin(); it != history.end();)
        {
            if (NOW - it->first > 1000) it = history.erase(it);
            else ++it;
        }

        float total = 0.f;
        int count = 0;
        for (auto it = history.begin(); it != history.end(); ++it)
        {
            total += it->second;
            count++;
        }
        avgBpsMap[actor] = total / count;
        bpsMap[actor] = bps;
    }
}

float getActorBps(bool avg, Actor* actor) {
    std::lock_guard<std::mutex> lock(bpsMutex);

    if (avg) return avgBpsMap.contains(actor) ? avgBpsMap[actor] : 0;
    return bpsMap.contains(actor) ? bpsMap[actor] : 0;
}

void Nametags::onRenderEvent(RenderEvent& event)
{
    auto ci = ClientInstance::get();
    if (!ci->getLevelRenderer()) return;

    auto actors = ActorUtils::getActorList(true, true);
    std::ranges::sort(actors, [&](Actor* a, Actor* b) {
        auto aPosComp = a->getRenderPositionComponent();
        auto bPosComp = b->getRenderPositionComponent();
        if (!aPosComp || !bPosComp) return false;
        auto aPos = aPosComp->mPosition;
        auto bPos = bPosComp->mPosition;
        auto origin = RenderUtils::transform.mOrigin;
        return glm::distance(origin, aPos) > glm::distance(origin, bPos);
    });

    auto drawList = ImGui::GetBackgroundDrawList();

    auto localPlayer = ci->getLocalPlayer();

    for (auto actor : actors)
    {
        if (!actor->isPlayer()) continue;
        if (actor == localPlayer && ci->getOptions()->mThirdPerson->value == 0 && !localPlayer->getFlag<RenderCameraComponent>()) continue;
        if (actor == localPlayer && !mRenderLocal.mValue) continue;
        auto shape = actor->getAABBShapeComponent();
        if (!shape) continue;
        auto posComp = actor->getRenderPositionComponent();
        if (!posComp) continue;

        float bps = getActorBps(false, actor);
        std::string formattedBps = fmt::format("{:.2f}", bps);
        float avgBps = getActorBps(true, actor);
        std::string formattedAvgBps = fmt::format("{:.2f}", avgBps);


        auto themeColor = ImColor(1.f, 1.f, 1.f, 1.f); //ColorUtils::getThemedColor(0);

        if (gFriendManager->isFriend(actor))
        {
            if (mShowFriends.mValue) themeColor = ImColor(0.0f, 1.0f, 0.0f);
            else continue;
        }

        glm::vec3 renderPos = posComp->mPosition;
        if (actor == localPlayer) renderPos = RenderUtils::transform.mPlayerPos;
        renderPos.y += 0.5f;

        glm::vec3 origin = RenderUtils::transform.mOrigin;
        glm::vec2 screen = glm::vec2(0, 0);

        if (!RenderUtils::transform.mMatrix.OWorldToScreen(origin, renderPos, screen, ci->getFov(), ci->getGuiData()->mResolution)) continue;
        if (std::isnan(screen.x) || std::isnan(screen.y)) continue;
        if (screen.x < 0 || screen.y < 0 || screen.x > ci->getGuiData()->mResolution.x * 2 || screen.y > ci->getGuiData()->mResolution.y * 2) continue;


        float fontSize = mFontSize.mValue;
        float padding = 5.f;

        if (mDistanceScaledFont.mValue)
        {
            // use distance to origin, not actor
            float distance = glm::distance(origin, renderPos) + 2.5f;
            if (distance < 0) distance = 0;
            fontSize = 1.0f / distance * 100.0f * mScalingMultiplier.mValue;
            if (fontSize < 1.0f) fontSize = 1.0f;
            if (fontSize < mMinScale.mValue) fontSize = mMinScale.mValue;
            padding = fontSize / 4;
        }

        FontHelper::pushPrefFont(true);

        std::string name = actor->getRawName();

        if (actor == localPlayer)
        {
            name = actor->getNameTag();
            // Remove everything after the first newline
            name = name.substr(0, name.find('\n'));
            name = ColorUtils::removeColorCodes(name);
        }

        if (mShowIrcUsers.mValue && IrcManager::mClient){
            auto ircUsers = IrcManager::mClient->getConnectedUsers();
            for (const auto& user : ircUsers){
                if (name.contains(user.playerName)){
                    name = user.username + " (" + user.playerName + ")";
                    break;
                }
            }
        }

        if (mShowBps.mValue)
        {
            if (mAverageBps.mValue)
            {
                name += " [" + formattedAvgBps + "]";
            }
            else
            {
                name += " [" + formattedBps + "]";
            }
        }

        ImVec2 imFontSize = ImGui::GetFont()->CalcTextSizeA(fontSize, FLT_MAX, 0, name.c_str());
        ImVec2 pos = ImVec2(screen.x - imFontSize.x / 2, screen.y - imFontSize.y - 5);

        ImVec2 rectMin = ImVec2(pos.x - padding, pos.y - padding);
        ImVec2 rectMax = ImVec2(pos.x + imFontSize.x + padding, pos.y + imFontSize.y + padding);

        if (mBlurStrength.mValue == 0.0f) {
            // Only draw the background if blur strength is 0
            drawList->AddRectFilled(rectMin, rectMax, ImColor(0.0f, 0.0f, 0.0f, 0.5f), 10.f);
        }

        ImRenderUtils::addBlur(ImVec4(rectMin.x, rectMin.y, rectMax.x, rectMax.y), mBlurStrength.mValue, 10, drawList, true);
        drawList->AddText(ImGui::GetFont(), fontSize, pos, themeColor, name.c_str());

        FontHelper::popPrefFont();
    }
}

void Nametags::onNametagRenderEvent(NametagRenderEvent& event)
{
    auto actor = event.mActor;
    auto localPlayer = ClientInstance::get()->getLocalPlayer();
    auto ci = ClientInstance::get();

    if (ActorUtils::isBot(actor)) return;
    if (!actor->isPlayer()) return;
    if (actor == localPlayer && ci->getOptions()->mThirdPerson->value == 0 && !localPlayer->getFlag<RenderCameraComponent>()) return;
    if (actor == localPlayer && !mRenderLocal.mValue) return;
    auto shape = actor->getAABBShapeComponent();
    if (!shape) return;
    auto posComp = actor->getRenderPositionComponent();
    if (!posComp) return;

    glm::vec3 renderPos = posComp->mPosition;
    if (actor == localPlayer) renderPos = RenderUtils::transform.mPlayerPos;
    renderPos.y += 0.5f;

    glm::vec3 origin = RenderUtils::transform.mOrigin;
    glm::vec2 screen = glm::vec2(0, 0);

    if (!RenderUtils::transform.mMatrix.OWorldToScreen(origin, renderPos, screen, ci->getFov(), ci->getGuiData()->mResolution)) return;
    if (std::isnan(screen.x) || std::isnan(screen.y)) return;
    if (screen.x < 0 || screen.y < 0 || screen.x > ci->getGuiData()->mResolution.x * 2 || screen.y > ci->getGuiData()->mResolution.y * 2) return;

    event.cancel();
}