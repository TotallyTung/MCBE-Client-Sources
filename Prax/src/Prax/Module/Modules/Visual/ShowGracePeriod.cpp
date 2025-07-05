//
// Created by vastrakai on 10/28/2023.
//

#include <Prax/SDK/Game/Network/Other/TextPacket.h>
#include <Prax/Utils/String.h>
#include <Prax/Utils/Render/Render.h>
#include "ShowGracePeriod.h"
#include <Prax/SDK/Game/Internal/MinecraftGame.h>
#include <Prax/UI/ThemeManager.h>
#include <Prax/Utils/Math.h>


int64_t graceStartTime = 0;
float graceSeconds = 31.f;
int64_t graceTime = graceSeconds * 1000;

void ShowGracePeriod::OnTick() {

}
static std::string text = "";
void ShowGracePeriod::OnRender()
{
    if (!Minecraft::ClientInstance->mcGame->canUseKeys()) return;


    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    int64_t now = Misc::GetCurrentMs();
    static ImVec2 topMiddlePos = ImVec2(0, 0);

    // Get the time left in the grace period
    float timeLeft = graceSeconds - (float)abs(float(graceStartTime - now)) / 1000.f;

    // Slowly animate up for the next 2 seconds

    bool render = false;

    if (now - graceStartTime < graceTime)
    {
        // Get the text to display

        std::string timeLeftStr = std::to_string(timeLeft);

        // make the timeLeft only display 2 decimal places after the period
        if (timeLeftStr.find(".") != std::string::npos) {
            timeLeftStr = timeLeftStr.substr(0, timeLeftStr.find(".") + 2);
        }

        text = Style.SelectedIndex == Style::Circle ? "     " : "";
        text += "Grace period: " + timeLeftStr + "s";

        render = true;
    } else if (now - graceStartTime < graceTime + 3000) {
        // Get the text to display
        text = "Grace period ended!";
        render = true;

        if (now - graceStartTime > graceTime + 2000) {
            render = false;
        }
    }




    // Render text at the top of the screen with ImGui


    if (!render)
        return;
    ImGui::PushFont(Font::Fonts["greycliff_demibold_large"]);

    const float padding = 2;
    const float fontSize = 20;


    // If it was just enabled lerp the topMiddlePos.y to 50px above the y pos
    topMiddlePos.y = Math::Animate(topMiddlePos.y, render ? 0 : -50, ImGui::GetIO().DeltaTime * 5.f);
    topMiddlePos.x = ImGui::GetIO().DisplaySize.x / 2;
    ImVec2 textSize = ImGui::GetFont()->CalcTextSizeA(fontSize, FLT_MAX, 0, text.c_str());

    float width = textSize.x + padding * 4;
    float height = textSize.y + padding * 4;


    ImVec2 rectStart = ImVec2(topMiddlePos.x - width / 2, topMiddlePos.y - height);
    ImVec2 rectEnd = ImVec2(topMiddlePos.x + width / 2, topMiddlePos.y + height);

    // Make sure the sizex is at least 200
    if (width < 200) {
        rectStart.x = topMiddlePos.x - 100;
        rectEnd.x = topMiddlePos.x + 100;
    }


    ImGui::GetBackgroundDrawList()->AddRectFilled(rectStart, rectEnd, ImGui::GetColorU32(ImVec4(0, 0, 0, 0.7f)), 5, 15);
    // Add shadow
    ImGui::GetBackgroundDrawList()->AddShadowRect(rectStart, rectEnd, ImGui::GetColorU32(ImVec4(0, 0, 0, 0.7f)), 125.0f, ImVec2(), ImDrawFlags_ShadowCutOutShapeBackground, 5);

    // Draw the text
    TextRenderInfo info;
    info.Text = text;
    info.Pos = ImVec2(topMiddlePos.x - textSize.x / 2, topMiddlePos.y + padding);
    info.Size = fontSize;
    info.Color = ImColor(255, 255, 255, 255);
    info.RenderShadow = true;
    Render::DrawShadowText(info, false);

    if (Style.SelectedIndex == Style::Line)
    {
        float percent = timeLeft / graceSeconds;
        if (percent < 0) percent = 0;
        ImVec2 start = ImVec2(rectStart.x, rectEnd.y);
        ImVec2 end = ImVec2(rectStart.x + 5 + (width - 10) * percent, rectEnd.y + 5);

        Render::DrawList->AddRectFilled(start, end, ThemeManager::CurrentTheme->primaryColor, 5, 15);
    } else
    {
        Render::DrawCircleLoader(ImVec2(rectStart.x + 12, topMiddlePos.y + height / 2 - 3), ThemeManager::CurrentTheme->primaryColor.GetShadowColor(), 7, 50, timeLeft, graceSeconds, 2.f);
        Render::DrawCircleLoader(ImVec2(rectStart.x + 12, topMiddlePos.y + height / 2 - 3), ThemeManager::CurrentTheme->primaryColor, 7, 50, timeLeft, graceSeconds, 2.f);
    }
    ImGui::PopFont();
}

void ShowGracePeriod::OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) {
    if (packet->getId() == (int)PacketID::Text) {
        // Replace the § with a & so we can use the Minecraft::Text formatting
        auto textPacket = std::reinterpret_pointer_cast<TextPacket>(packet);
        std::string text = textPacket->message.getText();

        // Check if the text packet is a game start message
        if (String::ContainsIgnoreCase(text, "§d§l» §r§b§lSkyWars: ") && String::ContainsIgnoreCase(text, "Lucky Ores")) {
            graceStartTime = Misc::GetCurrentMs();
            graceSeconds = 31.f;
            graceTime = graceSeconds * 1000;

            // §a§l» §r§bThere is a §f30§b second grace period
        } else if (String::ContainsIgnoreCase(text, "§a§l» §r§bThere is a §f") && String::ContainsIgnoreCase(text, "§b second grace period")) {
            // Get the time left in the grace period
            graceStartTime = Misc::GetCurrentMs();
            graceSeconds = 2.f;
            graceTime = graceSeconds * 1000;
        }
        text = String::ReplaceAll(text, "§", "&");
    } // Cancel the timer if the ChangeDimension packet is received
    else if (packet->getId() == (int)PacketID::ChangeDimension) {
        graceStartTime = 0;
    }
}
