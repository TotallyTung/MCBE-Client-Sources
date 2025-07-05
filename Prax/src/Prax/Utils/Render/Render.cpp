//
// Created by Flash on 9/13/2023.
//

#include "Render.h"

#include <imgui_internal.h>
#include <Prax/Hook/Hooks/D3D.h>
#include <Prax/Module/Modules/Visual/HUD.h>
#include <Prax/Utils/Math.h>
#include <Prax/SDK/Game/Misc/Structs.h>
#include <src/Prax/SDK/Game/Minecraft.h>
#include <Prax/SDK/Game/Render/LevelRenderer.h>

#include "D2D.h"

ImDrawList* Render::DrawList = nullptr;

void Render::PushDrawList(ImDrawList *drawList) {
    DrawList = drawList;
}

void Render::PopDrawList() {
    DrawList = ImGui::GetBackgroundDrawList();
}

/*
    // from Scaffold
    ImVec2 clipRectMin = ImVec2(pos.x, pos.y);
    ImVec2 clipRectMax = ImVec2(pos.x + numTextSize.x + 5, pos.y + numTextSize.y);

    drawList->PushClipRect(clipRectMin, clipRectMax, true);

    // Render the number
    for (int i = 0; i < numStr.size(); i++) {
        // Calc text size
        std::string num = std::string(1, numStr[i]);
        int realNum = std::stoi(num);
        ImVec2 ptextSize = ImGui::GetFont()->CalcTextSizeA(fontSize, FLT_MAX, 0, num.c_str());

        ptextSize.x = numberWidth * AnimationPerc;

        float offset = ptextSize.y * (realNum);
        // (i, offset) is the position of the number
        static std::map<int, float> indexOffsetMap;

        if (indexOffsetMap.find(i) == indexOffsetMap.end()) {
            indexOffsetMap[i] = offset;
        }

        // Scroll the number smoothly to the offset
        ImVec2 daPos = ImVec2((float)i * (float)ptextSize.x + pos.x, (float)-indexOffsetMap[i] + pos.y);

        color = HUD::GetColor(i + displayText.size());

        // Draw the number
        Render::RenderText(joinedNumbers, daPos, fontSize, ImColor(color.Value.x, color.Value.y, color.Value.z, color.Value.w * AnimationPerc), true);
        // we will cliprect this later

        // lerp da offset
        indexOffsetMap[i] = Math::Animate(indexOffsetMap[i], offset, ImGui::GetIO().DeltaTime * 10.f);
    }

    drawList->PopClipRect();*/

// for this, you will have to pass in your own static map to keep track of the offsets


void Render::RenderSmoothScrolledText(std::string str, ImVec2 pos, float size, std::vector<ImColor>& color, bool shadow, std::map<int, float>& indexOffsetMap, float animationPerc) {
    ImVec2 numTextSize = ImGui::GetFont()->CalcTextSizeA(size, FLT_MAX, 0, str.c_str());
    float numberWidth = numTextSize.x / str.size();

    // from Scaffold
    ImVec2 clipRectMin = ImVec2(pos.x, pos.y);
    ImVec2 clipRectMax = ImVec2(pos.x + numTextSize.x + 5, pos.y + numTextSize.y);

    DrawList->PushClipRect(clipRectMin, clipRectMax, true);

    // Render the number
    for (int i = 0; i < str.size(); i++) {
        // Calc text size
        std::string num = std::string(1, str[i]);
        int realNum = std::stoi(num);
        ImVec2 ptextSize = ImGui::GetFont()->CalcTextSizeA(size, FLT_MAX, 0, num.c_str());

        ptextSize.x = numberWidth * animationPerc;

        float offset = ptextSize.y * (realNum);

        // (i, offset) is the position of the number

        if (indexOffsetMap.find(i) == indexOffsetMap.end()) {
            indexOffsetMap[i] = offset;
        }

        // Scroll the number smoothly to the offset
        ImVec2 daPos = ImVec2((float)i * (float)ptextSize.x + pos.x, (float)-indexOffsetMap[i] + pos.y);

        // Draw the number
        RenderText(num, daPos, size, color[i], shadow);
        // we will cliprect this later

        // lerp da offset
        indexOffsetMap[i] = Math::Animate(indexOffsetMap[i], offset, ImGui::GetIO().DeltaTime * 10.f);
    }

    DrawList->PopClipRect();
}

void Render::RenderSmoothScrolledText(std::string str, ImVec2 pos, float size, ImColor color, bool shadow, std::map<int, float>& indexOffsetMap, float animationPerc) {
    static std::vector<std::string> numbers = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
    static std::string joinedNumbers = String::Join(numbers, "\n");

    ImVec2 numTextSize = ImGui::GetFont()->CalcTextSizeA(size, FLT_MAX, 0, str.c_str());
    float numberWidth = numTextSize.x / str.size() + 1;

    // from Scaffold
    ImVec2 clipRectMin = ImVec2(pos.x, pos.y);
    ImVec2 clipRectMax = ImVec2(pos.x + numTextSize.x + 5, pos.y + numTextSize.y);

    DrawList->PushClipRect(clipRectMin, clipRectMax, true);

    // Render the number
    for (int i = 0; i < str.size(); i++) {
        // Calc text size
        std::string num = std::string(1, str[i]);
        int realNum = std::stoi(num);
        ImVec2 ptextSize = ImGui::GetFont()->CalcTextSizeA(size, FLT_MAX, 0, num.c_str());

        ptextSize.x = numberWidth * animationPerc;

        float offset = ptextSize.y * (realNum);

        // (i, offset) is the position of the number

        if (indexOffsetMap.find(i) == indexOffsetMap.end()) {
            indexOffsetMap[i] = offset;
        }

        // Scroll the number smoothly to the offset
        ImVec2 daPos = ImVec2((float)i * (float)ptextSize.x + pos.x, (float)-indexOffsetMap[i] + pos.y);

        // Draw the number
        RenderText(joinedNumbers, daPos, size, color, shadow);
        // we will cliprect this later

        // lerp da offset
        indexOffsetMap[i] = Math::Animate(indexOffsetMap[i], offset, ImGui::GetIO().DeltaTime * 10.f);
    }

    DrawList->PopClipRect();
}

TextRenderInfo Render::DrawRainbowText(std::string str, ImVec2 pos, float size, float timeInSeconds, float saturation, float value, float separation, bool shadow) {
    for (int i = 0; i < str.length(); i++) {
        char c = str[i];
        ImColor color = ColorUtil::GetDefaultRainbow(i * separation, timeInSeconds, saturation, value);

        if (shadow) {
            TextRenderInfo charInfo;
            charInfo.Text = c;
            charInfo.Pos = pos;
            charInfo.Size = size;
            charInfo.Color = color;

            DrawShadowText(charInfo);
        }


        DrawList->AddText(ImGui::GetFont(), size, pos, color, std::string(1, c).c_str());

        pos.x += ImGui::GetFont()->CalcTextSizeA(size, FLT_MAX, 0.0f, std::string(1, c).c_str()).x;
    }

    // Generate TextRenderInfo
    TextRenderInfo info;
    info.Text = str;
    info.Pos = pos;
    info.Size = size;
    info.UsesRGB = true;
    info.RenderShadow = shadow;
    info.RGBInfo.Time = timeInSeconds;
    info.RGBInfo.Saturation = saturation;
    info.RGBInfo.Value = value;
    info.RGBInfo.Separation = separation;


    return info;
}

TextRenderInfo Render::DrawRainbowText(const TextRenderInfo& info) {
    return DrawRainbowText(info.Text,
                           info.Pos,
                           info.Size,
                           info.RGBInfo.Time,
                           info.RGBInfo.Saturation,
                           info.RGBInfo.Value,
                           info.RGBInfo.Separation,
                           info.RenderShadow);
}

TextRenderInfo Render::DrawGradientText(std::string str, ImVec2 pos, float size, float timeInSeconds, ImColor color1, ImColor color2, bool shadow) {
    for (int i = 0; i < str.length(); i++) {
        char c = str[i];
        ImColor color = ColorUtil::LerpColors(timeInSeconds, i * 100, color1, color2);
        ImVec2 charSize = ImGui::GetFont()->CalcTextSizeA(size, FLT_MAX, 0.0f, std::string(1, c).c_str());


        if (shadow) {
            // Generate TextRenderInfo
            TextRenderInfo charInfo;
            charInfo.Text = c;
            charInfo.Pos = pos;
            charInfo.Size = size;
            charInfo.Color = color;

            DrawShadowText(charInfo);
        }

        DrawList->AddText(ImGui::GetFont(), size, pos, color, std::string(1, c).c_str());

        pos.x += charSize.x;
    }

    // Generate TextRenderInfo
    TextRenderInfo info;
    info.Text = str;
    info.Pos = pos;
    info.Size = size;
    info.UsesGradient = true;
    info.RenderShadow = shadow;
    info.Color = color1;
    info.Color2 = color2;

    return info;
}

TextRenderInfo Render::DrawGradientText(const TextRenderInfo& info) {
    return DrawGradientText(info.Text,
                            info.Pos,
                            info.Size,
                            info.RGBInfo.Time,
                            info.Color,
                            info.Color2,
                            info.RenderShadow);
}

TextRenderInfo Render::DrawShadowText(const std::string& str, ImVec2 pos, float size, ImColor color, bool shadowOnly) {
    DrawList->AddText(ImGui::GetFont(), size, {pos.x + 2, pos.y + 2},
                                            ImColor(color.Value.x / 3, color.Value.y / 3, color.Value.z / 3,
                                                    color.Value.w), str.c_str());

    // Basically if shadowOnly is true, we don't want to render the original text
    if (!shadowOnly)
        DrawList->AddText(ImGui::GetFont(), size, pos, color, str.c_str());

    // Generate TextRenderInfo
    TextRenderInfo info;
    info.Text = str;
    info.Pos = pos;
    info.Size = size;
    info.Color = color;
    info.RenderShadow = true;
    return info;
}

TextRenderInfo Render::DrawShadowText(const TextRenderInfo& info, bool shadowOnly) {
    return DrawShadowText(info.Text,
                          info.Pos,
                          info.Size,
                          info.Color,
                          shadowOnly);
}

TextRenderInfo Render::RenderText(std::string str, ImVec2 pos, float size, ImColor color, bool shadow) {

    // Generate TextRenderInfo
    TextRenderInfo info;
    info.Text = str;
    info.Pos = pos;
    info.Size = size;
    info.Color = color;
    info.RenderShadow = shadow;

    if (shadow)
        DrawShadowText(info);

    DrawList->AddText(ImGui::GetFont(), size, pos, color, str.c_str());


    return info;
}

TextRenderInfo Render::RenderText(const TextRenderInfo& info) {
    return RenderText(info.Text,
                      info.Pos,
                      info.Size,
                      info.Color,
                      info.RenderShadow);
}

// this func is like RenderText, but it changes colors according to minecraft's color codes
TextRenderInfo Render::RenderTextColored(std::string str, ImVec2 pos, float size, ImColor color, bool shadow)
{
    // Use TextUtils::ImChatColors["§0"] to get the ImColor for black and so on
    // Use a regex to find the § symbol and then get the color code after it
    // Also use String::RemoveColorcodes when rendering the string

    // Replace the § with &
    /*str = String::ReplaceAll(str, "§", "&");
    str = String::ReplaceAll(str, "&l", "");
    str = String::ReplaceAll(str, "&r", "");*/
    str = String::ReplaceAll(str, {{"§", "&"}, {"&l", ""}, {"&r", ""}});
    str = String::RemoveNonAscii(str);
    str = String::Trim(str);


    std::vector<std::pair<std::string, ImColor>> strings;

    ImColor currentColor = color;
    bool firstColorSet = false;
    ImColor firstColor = color;

    // Seperate the string into parts seperated by the § symbol and the next character
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] == '&')
        {
            // Make sure this is a valid color code with a character after it
            if (i + 1 >= str.length()) continue;
            if (!TextUtils::ImChatColors.contains(str[i + 1])) continue;

            // Get the color code
            char colorCode = str[i + 1];

            // Make sure the color code is valid
            if (!TextUtils::ImChatColors.contains(colorCode)) continue;

            // Get the color
            color = TextUtils::ImChatColors[colorCode];

            // Skip the next two characters
            i += 2;

            if (!firstColorSet) {
                firstColor = color;
                firstColorSet = true;
            }

            // Add the string to the vector
            strings.emplace_back(std::make_pair(std::string(1, str[i]), color));
        }
        else
        {
            strings.emplace_back(std::make_pair(std::string(1, str[i]), color));
        }
    }

    // Render the string

    for (auto& [str, color] : strings)
    {
        DrawList->AddText(ImGui::GetFont(), size, pos, color, str.c_str());
        pos.x += ImGui::GetFont()->CalcTextSizeA(size, FLT_MAX, 0.0f, str.c_str()).x;
    }

    TextRenderInfo info;
    info.Color = firstColor;

    return info;
}


TextRenderInfo Render::DrawGlowText(std::string str, ImVec2 pos, float size, ImColor color, bool shadow) {
    for (char c : str) {
        ImVec2 charSize = ImGui::GetFont()->CalcTextSizeA(size, FLT_MAX, 0.0f, std::string(1, c).c_str());

        // Add Glow
        DrawList->AddShadowCircle(ImVec2(pos.x + (charSize.x / 2), pos.y + (charSize.y / 2)),
                                                        size / 3, color, 100, ImVec2(0.f, 0.f), 0, 12);

        pos.x += charSize.x;
    }

    // Generate TextRenderInfo
    TextRenderInfo info;
    info.Text = str;
    info.Pos = pos;
    info.Size = size;
    info.Color = color;
    info.RenderShadow = shadow;
    return info;
}

TextRenderInfo Render::DrawGlowText(std::string str, ImVec2 pos, float size, ImColor color, int thickness, bool shadow) {
    for (char c : str) {
        ImVec2 charSize = ImGui::GetFont()->CalcTextSizeA(size, FLT_MAX, 0.0f, std::string(1, c).c_str());

        // Add Glow
        DrawList->AddShadowCircle(ImVec2(pos.x + (charSize.x / 2), pos.y + (charSize.y / 2)),
                                  size / 3, color, thickness, ImVec2(0.f, 0.f), 0, 12);

        pos.x += charSize.x;
    }

    // Generate TextRenderInfo
    TextRenderInfo info;
    info.Text = str;
    info.Pos = pos;
    info.Size = size;
    info.Color = color;
    info.RenderShadow = shadow;
    return info;
}

TextRenderInfo Render::DrawGlowText(const TextRenderInfo& info) {
    return DrawGlowText(info.Text,
                        info.Pos,
                        info.Size,
                        info.Color,
                        info.RenderShadow);
}

void Render::FillScreenWithColor(ImColor color) {
    DrawList->AddRectFilled(ImVec2(0, 0), ImGui::GetIO().DisplaySize, color);
}

void Render::FillScreenWithBlur(float i) {
    D2D::AddBlur(DrawList, i, ImVec4{ 0, 0, ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y });
}

void Render::FillAreaWithBlur(float i, ImVec4 area) {
    if (i <= 0) return;

    D2D::AddBlur(DrawList, i, area);
}

void Render::DrawRect(ImVec2 pos1, ImVec2 pos2, ImColor color) {
    DrawList->AddRectFilled(pos1, pos2, color);
}

void Render::DrawImageFromURL(const std::string& url, ImVec2 pos, ImVec2 size)
{
    // Get the file name from the url
    std::string fileName = url.substr(url.find_last_of('/') + 1);
    std::string filePath = FileSystem::AssetDirectory + "\\" + fileName;

    // Download the file if it doesn't exist
    if (!FileSystem::FileExists(filePath)) {
        static std::vector<std::string> attemptedDownloads;
        if (std::ranges::find(attemptedDownloads, url) != attemptedDownloads.end()) return;

        Logger::Write("Render", "Downloading image from " + url, Logger::LogType::Info);
        auto file = Internet::DownloadFile(url, filePath);
        if (!file) {
            Logger::Write("Render", "Failed to download image from " + url, Logger::LogType::Error);
            attemptedDownloads.push_back(url);
            return;
        }
    }

    static std::unordered_map<std::string, ID3D11ShaderResourceView*> textures;
    if (textures.find(filePath) == textures.end()) {
        ID3D11ShaderResourceView* srv;
        int width, height;
        bool success = D3D::LoadTextureFromFile(filePath.c_str(), &srv, &width, &height);
        if (!success) {
            Logger::Write("Render", "Failed to load image from " + filePath, Logger::LogType::Error);
            return;
        }
        textures[filePath] = srv;
    }


    DrawList->AddImage(textures[filePath], pos, ImVec2(pos.x + size.x, pos.y + size.y));

}

// drawImageFromUrl (rounded)
void Render::DrawRoundedImageFromURL(const std::string& url, ImVec2 pos, ImVec2 size, float rounding, ImColor color)
{
    // Get the file name from the url
    std::string fileName = url.substr(url.find_last_of('/') + 1);
    std::string filePath = FileSystem::AssetDirectory + "\\" + fileName;

    // Download the file if it doesn't exist
    if (!FileSystem::FileExists(filePath)) {
        static std::vector<std::string> attemptedDownloads;
        if (std::ranges::find(attemptedDownloads, url) != attemptedDownloads.end()) return;

        Logger::Write("Render", "Downloading image from " + url, Logger::LogType::Info);
        auto file = Internet::DownloadFile(url, filePath);
        if (!file) {
            Logger::Write("Render", "Failed to download image from " + url, Logger::LogType::Error);
            attemptedDownloads.push_back(url);
            return;
        }
    }

    static std::unordered_map<std::string, ID3D11ShaderResourceView*> textures;
    if (textures.find(filePath) == textures.end()) {
        ID3D11ShaderResourceView* srv;
        int width, height;
        bool success = D3D::LoadTextureFromFile(filePath.c_str(), &srv, &width, &height);
        if (!success) {
            Logger::Write("Render", "Failed to load image from " + filePath, Logger::LogType::Error);
            return;
        }
        textures[filePath] = srv;
    }
    ImGui::GetForegroundDrawList()->AddImageRounded(textures[filePath], pos, ImVec2(pos.x + size.x, pos.y + size.y), ImVec2(0, 0), ImVec2(1, 1), color, rounding, ImDrawFlags_RoundCornersAll);

}

void Render::DrawLine(ImVec2 pos1, ImVec2 pos2, ImColor color, float thickness) {
    DrawList->AddLine(pos1, pos2, color, thickness);
}

void Render::DrawGlowLine(ImVec2 pos1, ImVec2 pos2, ImColor color, float thickness) {
    bool isX = pos1.x == pos2.x;

    if (isX) {
        DrawList->AddShadowRect(ImVec2(pos1.x - thickness / 2, pos1.y), ImVec2(pos1.x + thickness / 2, pos2.y), color, thickness*3, ImVec2(0, 0));
    } else {
        DrawList->AddShadowRect(ImVec2(pos1.x, pos1.y - thickness / 2), ImVec2(pos2.x, pos1.y + thickness / 2), color, thickness*3, ImVec2(0, 0));
    }

    DrawList->AddLine(pos1, pos2, color, thickness);
}

void Render::RenderTextWithBG(const std::string& str, ImVec2 pos, float size, ImColor color, ImColor bgColor, bool shadow, float textPadding) {
    ImVec2 textSize = ImGui::GetFont()->CalcTextSizeA(size, FLT_MAX, 0.0f, str.c_str());

    ImVec2 bgTopLeft = {pos.x, pos.y};
    ImVec2 bgBottomRight = ImVec2(bgTopLeft.x + textSize.x + textPadding * 2, bgTopLeft.y + textSize.y + textPadding * 2);
    ImVec2 textPos = {pos.x + textPadding, pos.y + textPadding};



    DrawList->AddShadowRect(bgTopLeft, bgBottomRight, color, 130, ImVec2(1, 1));
    DrawList->AddRectFilled(bgTopLeft, bgBottomRight, bgColor);



    // Create text render info
    TextRenderInfo textInfo;
    textInfo.Text = str;
    textInfo.Pos = textPos;
    textInfo.Size = size;
    textInfo.Color = color;

    if (shadow)
        DrawShadowText(textInfo);

    RenderText(textInfo);

}

void Render::DrawBoxAroundBlockOutline(Vector3 blockPos, ImColor color)
{
    if (!Minecraft::ClientInstance->levelRenderer->levelRendererPlayer) return;
    AABB aabb = AABB(Vector3(blockPos.x, blockPos.y, blockPos.z), Vector3(1, 1, 1));
    Math::DrawOutlinedBox(aabb.lower, aabb.upper, 1.0f, color);
}

void Render::DrawBoxAroundBlock(Vector3 blockPos, ImColor borderColor, ImColor filledColor)
{
    if (!Minecraft::ClientInstance->levelRenderer->levelRendererPlayer) return;
    AABB aabb = AABB(Vector3(blockPos.x, blockPos.y, blockPos.z), Vector3(1, 1, 1));
    Math::DrawBoxFilled(aabb.lower, aabb.upper, 1.0f, borderColor, filledColor);
}

void Render::Spinner(const char* label, ImVec2 pos, float radius, int thickness, const ImU32& color)
{
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    ImVec2 size((radius )*2, (radius + style.FramePadding.y)*2);

    // Render
    DrawList->PathClear();

    int num_segments = 30;
    int start = abs(ImSin(g.Time*1.8f)*(num_segments-5));

    const float a_min = IM_PI*2.0f * ((float)start) / (float)num_segments;
    const float a_max = IM_PI*2.0f * ((float)num_segments-3) / (float)num_segments;

    const ImVec2 centre = ImVec2(pos.x+radius, pos.y+radius+style.FramePadding.y);

    for (int i = 0; i < num_segments; i++) {
        const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
        DrawList->PathLineTo(ImVec2(centre.x + ImCos(a+g.Time*8) * radius,
                                            centre.y + ImSin(a+g.Time*8) * radius));
    }

    DrawList->PathStroke(color, false, thickness);
}


void Render::LoadingSpinner(float radius, ImVec2 position, float duration) {
    static float startTime = 0.0f;
    if (startTime == 0.0f) {
        startTime = ImGui::GetTime();
    }

    float currentTime = ImGui::GetTime();
    float progress = std::min((currentTime - startTime) / duration, 1.0f);

    ImVec2 center(position.x + radius, position.y + radius);


    // Draw the unfilled circle
    DrawList->AddCircle(center, radius, IM_COL32(255, 255, 255, 255), radius, 2.0f);

    // Calculate the angles to fill based on progress
    float startAngle = 0.0f;
    float endAngle = progress * 6.28318f; // 2 * Pi

    // Draw the filled part of the circle
    DrawList->PathLineTo(center);
    DrawList->PathArcTo(center, radius, startAngle, endAngle, radius);
    DrawList->PathFillConvex(IM_COL32(255, 255, 255, 255));
}

void Render::DrawCircleLoader(ImVec2 pos, ImColor color, float radius, int segments, float timeLeft, float total, float thickness)
{
    float countdownFraction = timeLeft / total;
    float M_PI = 3.14159265358979323846f;

    for (int i = 0; i < segments; ++i) {
        float start = (i / (float)segments) * 2 * M_PI;
        float end = ((i + 1) / (float)segments) * 2 * M_PI;
        if ((i / (float)segments) > countdownFraction) {
            continue;
        }
        ImVec2 startPoint = ImVec2(
            pos.x + cos(start) * radius,
            pos.y + sin(start) * radius
        );
        ImVec2 endPoint = ImVec2(
            pos.x + cos(end) * radius,
            pos.y + sin(end) * radius
        );

        ImGui::GetBackgroundDrawList()->AddLine(startPoint, endPoint, color, thickness);

    }
}

void Render::DrawGradientText(std::string str, ImVec2 pos, float size, float timeInSeconds, std::vector<ImColor>* colors, bool shadow) {
    for (int i = 0; i < str.length(); i++) {
        char c = str[i];
        ImColor color = HUD::GetColor(i);
        colors->push_back(color);
        ImVec2 charSize = ImGui::GetFont()->CalcTextSizeA(size, FLT_MAX, 0.0f, std::string(1, c).c_str());


        if (shadow) {
            // Generate TextRenderInfo
            TextRenderInfo charInfo;
            charInfo.Text = c;
            charInfo.Pos = pos;
            charInfo.Size = size;
            charInfo.Color = color;

            DrawShadowText(charInfo);
        }

        DrawList->AddText(ImGui::GetFont(), size, pos, color, std::string(1, c).c_str());

        pos.x += charSize.x;
    }
}

void Render::DrawGlowText(std::string str, ImVec2 pos, float size, std::vector<ImColor> color, bool shadow)
{
    int i = 0;
    for (char c : str) {
        ImVec2 charSize = ImGui::GetFont()->CalcTextSizeA(size, FLT_MAX, 0.0f, std::string(1, c).c_str());

        // Add Glow
        DrawList->AddShadowCircle(ImVec2(pos.x + (charSize.x / 2), pos.y + (charSize.y / 2)),
                                                        size / 3, color[i], 100, ImVec2(0.f, 0.f), 0, 12);

        pos.x += charSize.x;
        i++;
    }
}