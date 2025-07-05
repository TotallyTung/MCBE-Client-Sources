//
// Created by Flash on 9/15/2023.
//

#include "Statistics.h"
#include <Prax/Utils/System/Internet.h>
#include <Prax/Utils/String.h>
#include <Prax/Utils/Render/D2D.h>
#include <nlohmann/json.hpp>
#include <Prax/Hook/Hooks/D3D.h>
#include <Prax/Hook/Hooks/Social.h>
#include <Prax/SDK/Game/Internal/MinecraftGame.h>
#include <Prax/SDK/Game/Network/Other/TextPacket.h>
#include <Prax/Utils/Render/Render.h>

#include "HUD.h"

// Expected response {"UUID":"86fa704b-33fd-339f-9c3c-df76978fafab","xp":842,"played":13,"victories":4,"first_played":1694747267,"kills":33,"ores_mined":171,"spells_used":1,"deaths":4}

int Statistics::Kills = 0;
int Statistics::Deaths = 0;
int Statistics::Played = 0;


void queueStatCheck(uint64_t delay)
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) {
        return;
    }

    std::string playerName;

    auto xuid = player->getXuid();

    for (auto& listing : player->level->playerList | std::views::values) {
        if (listing.XUID == xuid) {
            playerName = listing.name;
            break;
        }
    }

    // Create a new thread to get the stats
    std::thread([playerName, delay]() {

        // sleep for the delay
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));

        static std::vector<std::string> gamemodesToGetStatsFor = { "sky", "sky-duos", "sky-squads", "sky-mega", "ctf", "bed", "bed-duos", "bed-squads", "sg", "sg-duos" };

        Logger::Write("Statistics", "Getting stats for " + playerName + " in " + std::to_string(gamemodesToGetStatsFor.size()) + " gamemodes");

        // set stats to 0
        Statistics::Kills = 0;
        Statistics::Deaths = 0;
        Statistics::Played = 0;

        for (auto& gamemode : gamemodesToGetStatsFor)
        {
            auto response = Internet::GetHiveStatsJson(gamemode, playerName);

            if (response.empty()) {
                Logger::Write("Statistics", "Failed to get stats for " + playerName);
                continue;
            }

            try {
                nlohmann::json json = nlohmann::json::parse(response);
                int played = 0;
                int kills = 0;
                int deaths = 0;

                if (json.contains("played")) {
                    played = json["played"];
                }
                if (json.contains("kills")) {
                    kills = json["kills"];
                }
                if (json.contains("deaths")) {
                    deaths = json["deaths"];
                }

                Statistics::Kills += kills;
                Statistics::Deaths += deaths;
                Statistics::Played += played;

                Logger::Write("Statistics", "Got stats for " + playerName + " in " + gamemode + " with " + std::to_string(played) + " games played, " + std::to_string(kills) + " kills, and " + std::to_string(deaths) + " deaths");
            }
            catch (nlohmann::json::exception& e) {
                Logger::Write("Statistics", "Failed to parse json: " + std::string(e.what()));
            }
        }

    }).detach();
}

void Statistics::OnEnable() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) {
        Enabled = false;
        OnDisable();
        return;
    }


    queueStatCheck(0);
}


void Statistics::OnDisable() {

}

struct StructuredPlaytime {
    int days;
    int hours;
    int minutes;
    int seconds;
};

StructuredPlaytime getPlayTimeFormatted()
{
    // format: 1d 2h 3m 4s
    int64_t playTime = Misc::GetCurrentMs() / 1000 - Prax::InjectTime / 1000;
    int days = playTime / 86400;
    int hours = (playTime % 86400) / 3600;
    int minutes = (playTime % 3600) / 60;
    int seconds = playTime % 60;

    return { days, hours, minutes, seconds };
}

void Statistics::OnRender()
{
    if (!Minecraft::ClientInstance->ThePlayer() || Minecraft::ClientInstance->GetScreenName() != "hud_screen" || !Enabled || !Minecraft::ClientInstance->mcGame->canUseKeys()) {
        return;
    }

    ImVec2 pos = ImGui::GetIO().DisplaySize;
    pos.x = 50;
    pos.y /= 2;
    pos.x -= 50;

    ImGui::PushFont(Font::Fonts["greycliff_demibold_large"]);

    auto drawList = ImGui::GetForegroundDrawList();
    Render::PushDrawList(drawList);
    // draw a shadow rect

    ImVec4 area = ImVec4(pos.x, pos.y, pos.x + 200, pos.y + 110);
    D2D::AddBlur(drawList, 10.f, area, 10.f);
    drawList->AddRectFilled(pos, ImVec2(area.z, area.w), ImColor(0.f, 0.f, 0.f, 0.45f), 10.0f);
    ImVec2 titlePos = pos;
    ImVec2 textSize = ImGui::GetFont()->CalcTextSizeA(20.0f, FLT_MAX, 0, "Statistics");
    titlePos.x += (200 - textSize.x) / 2;
    titlePos.y += (25 - textSize.y) / 2;
    Render::RenderText("Statistics", titlePos, 20.0f, ImColor(255, 255, 255, 255), false);


    // Draw a divider
    ImVec2 lineStart = ImVec2(pos.x, pos.y + 25);
    ImVec2 lineEnd = ImVec2(pos.x + 200, pos.y + 26);

    //drawList->AddLine(ImVec2(pos.x, pos.y + 25), ImVec2(pos.x + 200, pos.y + 25), ImColor(255, 255, 255, 255));
    // Get the color for each point in the line using HUD::GetColor(index)
    int lineLength = 20;
    int lengthPerLine = 200 / lineLength;
    for (int i = 0; i < lineLength; i++) {
        ImVec2 point = ImVec2(lineStart.x + (lineEnd.x - lineStart.x) * i / lineLength, lineStart.y);
        //drawList->AddLine(point, ImVec2(point.x + lengthPerLine, point.y), HUD::GetColor(i));
        // add a rect instead to make the line thicker
        drawList->AddRectFilled(point, ImVec2(point.x + lengthPerLine, lineEnd.y), HUD::GetColor(i));
    }

    // Draw the stats

    int startPadding = 38;


    Render::RenderText("Play Time: ", ImVec2(pos.x + 10, pos.y + startPadding), 20.0f, ImColor(255, 255, 255, 255), false);
    ImVec2 playTimeTextSize = ImGui::GetFont()->CalcTextSizeA(20.0f, FLT_MAX, 0, "Play Time: ");
    ImVec2 playTimePos = ImVec2(pos.x + 10 + playTimeTextSize.x, pos.y + startPadding);
    StructuredPlaytime playTime = getPlayTimeFormatted();
    static std::map<int, std::map<int, float>> indexOffsetStorage; // AIDS

    // If empty, initialize it
    if (indexOffsetStorage.empty()) {
        for (int i = 0; i < 6; i++)
        {
            indexOffsetStorage[i] = { {} };
        }
    }

    for (int i = 0; i < 4; i++) {
        std::string time;
        std::string timeText;
        switch (i) {
        case 0:
            time = std::to_string(playTime.days) + "";
            timeText = "d";
            break;
        case 1:
            time = std::to_string(playTime.hours) + "";
            timeText = "h";
            break;
        case 2:
            time = std::to_string(playTime.minutes) + "";
            timeText = "m";
            break;
        case 3:
            time = std::to_string(playTime.seconds) + "";
            timeText = "s";
            break;
        }

        // Continue if the time is a h or d and the time is 0
        if ((timeText == "h" || timeText == "d") && time == "0") {
            continue;
        }

        // if the time is less than 10, add a 0 in front of it
        if (time.size() == 1 && i == 3) {
            time = "0" + time;
        }

        timeText += " ";

        // std::string str, ImVec2 pos, float size, ImColor color, bool shadow, std::map<int, float>& indexOffsetMap, float animationPerc) {
        Render::RenderSmoothScrolledText(time, playTimePos, 20.0f, ImColor(255, 255, 255, 255), false, indexOffsetStorage[i], 1.f);
        playTimePos.x += ImGui::GetFont()->CalcTextSizeA(20.0f, FLT_MAX, 0, time.c_str()).x + 1;
        // Render the time text
        Render::RenderText(timeText, playTimePos, 20.0f, ImColor(255, 255, 255, 255), false);
        playTimePos.x += ImGui::GetFont()->CalcTextSizeA(20.0f, FLT_MAX, 0, timeText.c_str()).x;

    }

    Render::RenderText("Games Played: ", ImVec2(pos.x + 10, pos.y + startPadding + 20), 20.0f, ImColor(255, 255, 255, 255), false);
    // Render smooth scrolled text
    Render::RenderSmoothScrolledText(std::to_string(Statistics::Played), ImVec2(pos.x + 10 + ImGui::GetFont()->CalcTextSizeA(20.0f, FLT_MAX, 0, "Games Played: ").x, pos.y + startPadding + 20), 20.0f, ImColor(255, 255, 255, 255), false, indexOffsetStorage[4], 1.f);

    Render::RenderText("Kills: ", ImVec2(pos.x + 10, pos.y + startPadding + 40), 20.0f, ImColor(255, 255, 255, 255), false);
    // Render smooth scrolled text
    Render::RenderSmoothScrolledText(std::to_string(Statistics::Kills), ImVec2(pos.x + 10 + ImGui::GetFont()->CalcTextSizeA(20.0f, FLT_MAX, 0, "Kills: ").x, pos.y + startPadding + 40), 20.0f, ImColor(255, 255, 255, 255), false, indexOffsetStorage[5], 1.f);


    // make sure the left over padding between the first text and start of the line is the same as the end of the last text and the bottom of the box
    // this will make it look nice and centered


    Render::PopDrawList();

    ImGui::PopFont();




}

void Statistics::OnPacketReceive(std::shared_ptr<Packet> packet, bool* cancel) {

    if (packet->getId() != (int)PacketID::Text) return;

    auto textPacket = std::reinterpret_pointer_cast<TextPacket2>(packet);

    std::string text = textPacket->message;

    if (String::ContainsIgnoreCase(text, "§c§l» §r§c§lGame OVER!")) {
        Played++;
        queueStatCheck(7500); // 7.5 seconds
    }
}