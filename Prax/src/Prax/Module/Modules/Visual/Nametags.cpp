//
// Created by Flash on 1/3/2024.
//

#include "Nametags.h"

#include <mutex>
#include <Prax/Module/ModuleManager.h>


#include <Prax/Module/Modules/Combat/Antibot.h>
#include <Prax/Module/Modules/Player/Friends.h>
#include <Prax/Utils/Math.h>
#include <Prax/Utils/Render/ColorUtil.h>
#include <Prax/Utils/Render/Render.h>
#include <Prax/Utils/String.h>
#include <Prax/SDK/Game/Render/LevelRenderer.h>

#include "ShowNametag.h"


Actor* getActorFromPlayerlist(std::string name) {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return nullptr;

    // use getRuntimeActorList for entitylist
    for (auto& actor : player->level->getRuntimeActorList()) {
        if (String::ContainsIgnoreCase(actor->_getNameTag(), name)) {
            return actor;
        }
    }

    return nullptr;
}


std::unordered_map<Actor*, std::string> Nametags::CachedXuids = std::unordered_map<Actor*, std::string>();
void Nametags::OnRender()
{
    auto player = Minecraft::ClientInstance->ThePlayer();

    if (!player) {
        return;
    }
    ImGui::PushFont(Font::Fonts["greycliff_demibold_large"]);

    std::vector<Actor*> actors = Antibot::GetActors(ShowTeamMembers.Enabled);

    static ShowNametag* showNametag = ModuleManager::GetModule<ShowNametag>();

    for (auto actor : actors) {
        Vector3* dpos = actor->GetRenderPositionComponent();
        if (!dpos) continue;
        Vector3 pos = *dpos;
        AABB aabb = actor->GetLerpedAABB();
        if (actor == player)
        {
            // Check if we are in first person
            if (Minecraft::ClientInstance->GetOptions()->game_thirdperson->value == 0) continue;
            // If ShowNametags module is disabled, continue
            if (!showNametag->Enabled) continue;
            pos = Render::Transform.playerPos;
            aabb = player->GetAABBForPos(pos);
        }
        auto color = ImColor(255, 255, 255, 255);

        if (ShowFriends.Enabled && Friends::IsFriend(actor)) color = ImColor(0, 255, 0, 255);
        else if (!ShowFriends.Enabled && Friends::IsFriend(actor)) continue;


        ImVec4 pos2d = Math::Get2dDrawPoints(aabb.lower, aabb.upper);

        if (pos2d.x <= 0 && pos2d.y <= 0 && pos2d.z <= 0 && pos2d.w <= 0)
            continue;

        std::string name = actor->_getNameTag();

        if (name.empty())
            continue;

        // if the name contains a newline, remove it and everything after it
        if (name.find('\n') != std::string::npos)
            name = name.substr(0, name.find('\n'));

        // make sure the player is on the screen
        if (pos2d.x < 0 || pos2d.y < 0) continue;

        Vector3 cameraPos = Minecraft::ClientInstance->levelRenderer->levelRendererPlayer->cameraPos;

        float fontSize = Size.Value;

        if (Scaling.Enabled)
        {
            // scale the size down based how large the distance is from the camera
            float distance = cameraPos.Distance(*pos);
            float scale = abs((distance / 2.5f) + 1.f);
            fontSize /= scale;

            // make sure the font size is not smaller than the minimum
            if (fontSize < MinimumScale.Value)
                fontSize = MinimumScale.Value;
        }

        // remove any non ascii characters

        // Replace § with &
        ImVec2 textSize = ImGui::GetFont()->CalcTextSizeA(fontSize, FLT_MAX, 0, String::RemoveColorCodes(name).c_str());

        // Get the position of the text thats centered above the 2d startpos
        ImVec2 textPos = ImVec2(pos2d.x + (pos2d.z - pos2d.x) / 2 - textSize.x / 2, pos2d.y - textSize.y - 5);

        // Get the position of the background
        ImVec2 bgPos = ImVec2(textPos.x - 5, textPos.y - 5);
        ImVec2 bgEndPos = ImVec2(bgPos.x + textSize.x + 10, bgPos.y + textSize.y + 10);

        // Render outline of the background using bg draw list


        // Render the background using bg draw list
        Render::DrawList->AddRectFilled(bgPos, bgEndPos, ImColor(0, 0, 0, 100), 5.f);

        // Render the text using fg draw list
        auto info = Render::RenderTextColored(name, textPos, fontSize, color, false);

        //Render::DrawList->AddRect(bgPos, bgEndPos, info.Color, 5.f);


    }

    ImGui::PopFont();
}

bool _bytePatch = false;


void Nametags::OnTick()
{
    auto player = Minecraft::ClientInstance->ThePlayer();

    if (!player) return;

    static uintptr_t addr = SigManager::GetSignatureAddress(SignatureEnum::Actor_shouldRenderNametagBp);
    if (HideMinecraftNametags.Enabled) {
        std::vector<char> bytes = {(char)0x90, (char)0xE9};
        Memory::WriteBytes(addr, bytes.data(), bytes.size());
        _bytePatch = true;
    } else {
        std::vector<char> bytes = {(char)0x0F, (char)0x84};
        Memory::WriteBytes(addr, bytes.data(), bytes.size());
        _bytePatch = false;
    }
}


void Nametags::OnEject()
{
    if (!_bytePatch) return;
    static uintptr_t addr = SigManager::GetSignatureAddress(SignatureEnum::Actor_shouldRenderNametagBp);
    std::vector<char> bytes = {(char)0x0F, (char)0x84};
    Memory::WriteBytes(addr, bytes.data(), bytes.size());
}

void Nametags::OnEnable()
{
    if (!HideMinecraftNametags.Enabled) return;

    static uintptr_t addr = SigManager::GetSignatureAddress(SignatureEnum::Actor_shouldRenderNametagBp);
    std::vector<char> bytes = {(char)0x90, (char)0xE9};
    Memory::WriteBytes(addr, bytes.data(), bytes.size());

    _bytePatch = true;
}

void Nametags::OnDisable()
{
    if (!_bytePatch) return;
    static uintptr_t addr = SigManager::GetSignatureAddress(SignatureEnum::Actor_shouldRenderNametagBp);
    std::vector<char> bytes = {(char)0x0F, (char)0x84};
    Memory::WriteBytes(addr, bytes.data(), bytes.size());


}