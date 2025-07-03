//
// Created by vastrakai on 9/4/2024.
//

#include "UpdateForm.hpp"

#include <build_info.h>
#include <Features/Events/KeyEvent.hpp>
#include <Features/Events/MouseEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <Utils/OAuthUtils.hpp>

void UpdateForm::getCommits()
{
    std::string latestHash = mOldHash;
    /*HttpRequest request(HttpMethod::GET, OAuthUtils::sEndpoint + "getCommitsBetween?startHash=" + latestHash + "&endHash=" + SOLSTICE_BUILD_VERSION, "", "", &UpdateForm::onHttpResponse, nullptr);
    request.sendAsync();*/
    mRequest.mMethod = HttpMethod::GET;
    mRequest.mUrl = OAuthUtils::sEndpoint + "getCommitsBetween?startHash=" + latestHash + "&endHash=" + SOLSTICE_BUILD_VERSION;
    mRequest.mCallback = &UpdateForm::onHttpResponse;
    mRequest.mSender = this;
    mRequest.sendAsync();
    mRequestingCommits = true;
}

void UpdateForm::onHttpResponse(HttpResponseEvent event)
{
    try
    {
        auto sender = reinterpret_cast<UpdateForm*>(event.mSender);
        nlohmann::json json = nlohmann::json::parse(event.mResponse);
        sender->mCommits = json.get<std::vector<std::string>>();
        sender->mGatheredCommits = true;
        spdlog::info("[UpdateForm] Gathered {} commits", sender->mCommits.size());
        sender->mRequestingCommits = false;
    } catch (const std::exception& e)
    {
        spdlog::error("[UpdateForm] Error parsing commits: {}", e.what());
    } catch (...)
    {
        spdlog::error("[UpdateForm] Unknown error parsing commits");
    }
}

void UpdateForm::onEnable()
{
    mOldHash = OAuthUtils::getLastCommitHash();
    std::string latestHash = SOLSTICE_BUILD_VERSION;
    OAuthUtils::saveCommitHash(latestHash);

    if (!mGatheredCommits)
    {
        getCommits(); // Async get commits
    }

    gFeatureManager->mDispatcher->listen<RenderEvent, &UpdateForm::onRenderEvent, nes::event_priority::ABSOLUTE_LAST>(this);
    gFeatureManager->mDispatcher->listen<KeyEvent, &UpdateForm::onKeyEvent>(this);
    gFeatureManager->mDispatcher->listen<MouseEvent, &UpdateForm::onMouseEvent>(this);

    mLastEnable = NOW;
}

void UpdateForm::onDisable()
{
    gFeatureManager->mDispatcher->deafen<RenderEvent, &UpdateForm::onRenderEvent>(this);
    gFeatureManager->mDispatcher->deafen<KeyEvent, &UpdateForm::onKeyEvent>(this);
    gFeatureManager->mDispatcher->deafen<MouseEvent, &UpdateForm::onMouseEvent>(this);

    mLastEnable = 0;
}

ImVec2 lastrectmin = ImVec2(0, 0);
ImVec2 lastrectmax = ImVec2(0, 0);

void UpdateForm::onRenderEvent(RenderEvent& event)
{
    if (ClientInstance::get()->getMouseGrabbed()) ClientInstance::get()->releaseMouse();

    auto drawlist = ImGui::GetForegroundDrawList();

    FontHelper::pushPrefFont(true);

    // Set initial width for the rectangle
    float rectWidth = ImGui::GetIO().DisplaySize.x / 2;
    float padding = 10.0f;

    // Calculate dynamic height based on the number of commits and title height
    float titleHeight = ImGui::GetFont()->CalcTextSizeA(30, FLT_MAX, 0, "New Solstice update installed!").y;
    float commitHeight = 0.0f;

    if (mGatheredCommits)
        for (const auto& commit : mCommits)
        {
            std::string finalStr = std::to_string(&commit - &mCommits[0] + 1) + ": " + commit;
            commitHeight += ImGui::GetFont()->CalcTextSizeA(20, FLT_MAX, 0, finalStr.c_str()).y + 5.0f;
        }
    else
    {
        std::string finalStr = "Gathering commits...";
        commitHeight += ImGui::GetFont()->CalcTextSizeA(20, FLT_MAX, 0, finalStr.c_str()).y + 5.0f;
    }

    // Set the render size based on the calculated heights
    float rectHeight = titleHeight + commitHeight + 70.0f; // Extra space for padding and button

    // Determine rect width based on the longest commit + padding
    float longestCommit = 0.0f;
    if (mGatheredCommits)
        for (const auto& commit : mCommits)
        {
            std::string finalStr = std::to_string(&commit - &mCommits[0] + 1) + ": " + commit;
            ImVec2 textSize = ImGui::GetFont()->CalcTextSizeA(20, FLT_MAX, 0, finalStr.c_str());
            if (textSize.x > longestCommit)
                longestCommit = textSize.x;
        }
    else
    {
        std::string finalStr = "Gathering commits...";
        ImVec2 textSize = ImGui::GetFont()->CalcTextSizeA(20, FLT_MAX, 0, finalStr.c_str());
        if (textSize.x > longestCommit)
            longestCommit = textSize.x;
    }

    // if longest commit is shorter than title, use title width
    if (longestCommit < ImGui::GetFont()->CalcTextSizeA(30, FLT_MAX, 0, "New Solstice update installed!").x)
        longestCommit = ImGui::GetFont()->CalcTextSizeA(30, FLT_MAX, 0, "New Solstice update installed!").x;

    rectWidth = longestCommit + 2 * padding;

    ImVec2 renderSize = ImVec2(rectWidth, rectHeight);
    ImVec2 renderPos = ImVec2((ImGui::GetIO().DisplaySize.x - rectWidth) / 2, (ImGui::GetIO().DisplaySize.y - rectHeight) / 2);

    lastrectmin = renderPos;
    lastrectmax = renderPos + renderSize;

    ImRenderUtils::addBlur(ImVec4(renderPos.x, renderPos.y, renderPos.x + renderSize.x, renderPos.y + renderSize.y), 4.f, 10.0f, drawlist);
    drawlist->AddRectFilled(renderPos, renderPos + renderSize, IM_COL32(0, 0, 0, 100), 10.0f);

    ImColor prefColor = ColorUtils::getThemedColor(0);
    drawlist->AddRect(renderPos, renderPos + renderSize, ImColor(prefColor.Value.x, prefColor.Value.y, prefColor.Value.z, prefColor.Value.w), 10.0f);

    // Title
    ImVec2 titlePos = ImVec2(renderPos.x + (renderSize.x - 200) / 2, renderPos.y + padding);
    ImVec2 titleSize = ImGui::GetFont()->CalcTextSizeA(30, FLT_MAX, 0, "New Solstice update installed!");
    titlePos.y += (titleHeight - titleSize.y) / 2;
    titlePos.x = renderPos.x + (renderSize.x - titleSize.x) / 2;
    drawlist->AddText(ImGui::GetFont(), 30, titlePos, IM_COL32(255, 255, 255, 255), "New Solstice update installed!");

    // Commits
    ImVec2 commitPos = ImVec2(renderPos.x + padding, renderPos.y + titleHeight + 2 * padding);

    std::string initalStr = "New commits:";
    ImVec2 initialSize = ImGui::GetFont()->CalcTextSizeA(20, FLT_MAX, 0, initalStr.c_str());
    drawlist->AddText(ImGui::GetFont(), 20, commitPos, IM_COL32(255, 255, 255, 255), initalStr.c_str());
    commitPos.y += initialSize.y + 5.0f;

    int index = 0;
    if (!mGatheredCommits)
    {
        std::string finalStr = "Gathering commits...";
        ImVec2 textSize = ImGui::GetFont()->CalcTextSizeA(20, FLT_MAX, 0, finalStr.c_str());
        drawlist->AddText(ImGui::GetFont(), 20, commitPos, IM_COL32(255, 255, 255, 255), finalStr.c_str());
        commitPos.y += textSize.y + 5.0f;
    }
    else
        for (const auto& commit : mCommits)
        {
            index++;
            std::string finalStr = std::to_string(index) + ": " + commit;
            ImVec2 textSize = ImGui::GetFont()->CalcTextSizeA(20, FLT_MAX, 0, finalStr.c_str());
            drawlist->AddText(ImGui::GetFont(), 20, commitPos, IM_COL32(255, 255, 255, 255), finalStr.c_str());
            commitPos.y += textSize.y + 5.0f;
        }

    commitPos.y += padding;
    // Okay button
    ImVec2 buttonSize = ImVec2(80, 30);
    ImVec2 buttonPos = ImVec2(renderPos.x + renderSize.x - buttonSize.x - padding, renderPos.y + renderSize.y - buttonSize.y - padding);
    drawlist->AddRectFilled(buttonPos, buttonPos + buttonSize, IM_COL32(0, 0, 0, 100));
    ImVec2 textSize = ImGui::GetFont()->CalcTextSizeA(20, FLT_MAX, 0, "Okay");
    ImVec2 textPos = ImVec2(buttonPos.x + (buttonSize.x - textSize.x) / 2, buttonPos.y + (buttonSize.y - textSize.y) / 2);
    drawlist->AddText(ImGui::GetFont(), 20, textPos, IM_COL32(255, 255, 255, 255), "Okay");

    // If isMouseClicked and the mouse is over the button, disable the module
    if (ImRenderUtils::isMouseOver(ImVec4(buttonPos.x, buttonPos.y, buttonPos.x + buttonSize.x, buttonPos.y + buttonSize.y)))
    {
        // highlight the button
        drawlist->AddRectFilled(buttonPos, buttonPos + buttonSize, IM_COL32(255, 255, 255, 50));
        if (ImGui::IsMouseClicked(0))
            setEnabled(false);
    }

    FontHelper::popPrefFont();
}

void UpdateForm::onMouseEvent(MouseEvent& event)
{
    event.cancel();
}

void UpdateForm::onKeyEvent(KeyEvent& event)
{
    if (event.mKey == VK_ESCAPE && event.mPressed && NOW - mLastEnable > 500)
    {
        setEnabled(false);
        event.cancel();
    }
}
