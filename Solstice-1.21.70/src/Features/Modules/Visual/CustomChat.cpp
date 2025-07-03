//
// Created by vastrakai on 9/21/2024.
//

#include "CustomChat.hpp"

#include <Features/Events/ChatEvent.hpp>
#include <Features/Events/PacketInEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Network/Packets/TextPacket.hpp>
#include <Utils/MiscUtils/MathUtils.hpp>

void CustomChat::onEnable()
{
    gFeatureManager->mDispatcher->listen<PacketInEvent, &CustomChat::onPacketInEvent, nes::event_priority::ABSOLUTE_LAST>(this);
    gFeatureManager->mDispatcher->listen<RenderEvent, &CustomChat::onRenderEvent>(this);
}

void CustomChat::onDisable()
{
    gFeatureManager->mDispatcher->deafen<PacketInEvent, &CustomChat::onPacketInEvent>(this);
    gFeatureManager->mDispatcher->deafen<RenderEvent, &CustomChat::onRenderEvent>(this);
}



struct ParsedText {
    std::string text;
    ImVec2 pos;
    ImU32 color;
};



// Define color codes
enum class ChatColor : char {
    BLACK = '0',
    DARK_BLUE = '1',
    DARK_GREEN = '2',
    DARK_AQUA = '3',
    DARK_RED = '4',
    DARK_PURPLE = '5',
    GOLD = '6',
    GRAY = '7',
    DARK_GRAY = '8',
    BLUE = '9',
    GREEN = 'a',
    AQUA = 'b',
    RED = 'c',
    LIGHT_PURPLE = 'd',
    YELLOW = 'e',
    WHITE = 'f'
};

// Mapping of color codes to ImGui colors
static const std::unordered_map<char, ImU32> colorMap = {
    {'0', IM_COL32(0, 0, 0, 255)},         // BLACK
    {'1', IM_COL32(0, 0, 170, 255)},       // DARK_BLUE
    {'2', IM_COL32(0, 170, 0, 255)},       // DARK_GREEN
    {'3', IM_COL32(0, 170, 170, 255)},     // DARK_AQUA
    {'4', IM_COL32(170, 0, 0, 255)},       // DARK_RED
    {'5', IM_COL32(170, 0, 170, 255)},     // DARK_PURPLE
    {'6', IM_COL32(255, 170, 0, 255)},     // GOLD
    {'7', IM_COL32(170, 170, 170, 255)},   // GRAY
    {'8', IM_COL32(85, 85, 85, 255)},      // DARK_GRAY
    {'9', IM_COL32(85, 85, 255, 255)},     // BLUE
    {'a', IM_COL32(85, 255, 85, 255)},     // GREEN
    {'b', IM_COL32(85, 255, 255, 255)},    // AQUA
    {'c', IM_COL32(255, 85, 85, 255)},     // RED
    {'d', IM_COL32(255, 85, 255, 255)},    // LIGHT_PURPLE
    {'e', IM_COL32(255, 255, 85, 255)},    // YELLOW
    {'f', IM_COL32(255, 255, 255, 255)},   // WHITE
    {'r', IM_COL32(255, 255, 255, 255)},   // RESET
};

// Function to get color value from map
template <typename T>
ImU32 getColorValue(const std::unordered_map<char, ImU32>& map, char code) {
    auto it = map.find(code);
    if (it != map.end()) {
        return it->second;
    }
    return IM_COL32(255, 255, 255, 255); // Default color: white
}

struct ParsedMessage {

};

std::vector<ParsedText> parseMessage(const std::string& message) {
    std::vector<ParsedText> parsedText;
    ImU32 currentColor = IM_COL32(255, 255, 255, 255); // Default color: white
    std::string currentSegment;

    for (size_t i = 0; i < message.length(); ++i) {
        if (message[i] == '§') {
            if (!currentSegment.empty()) {
                ParsedText p;
                p.text = currentSegment;
                p.color = currentColor;
                parsedText.emplace_back(p);
                currentSegment.clear();
            }

            // Check if there's a valid color code
            if (i + 1 < message.length()) {
                char codeChar = std::tolower(static_cast<unsigned char>(message[i + 1]));
                auto it = colorMap.find(codeChar);
                if (it != colorMap.end()) {
                    currentColor = it->second;
                    i++; // Skip the color code character
                } else {
                    // Invalid color code, include '§' and the character in the text
                    currentSegment += '§';
                    currentSegment += message[i + 1];
                    i++; // Skip the character
                }
            } else {
                // '§' at the end of the string, include it in the text
                currentSegment += '§';
            }
        } else {
            currentSegment += message[i];
        }
    }

    if (!currentSegment.empty()) {
        ParsedText p;
        p.text = currentSegment;
        p.color = currentColor;
        parsedText.emplace_back(p);
    }

    return parsedText;
}
void CustomChat::addMessage(std::string message) {
    ChatMessage chatMessage;
    chatMessage.mText = message;
    chatMessage.mLifeTime = mMaxLifeTime.as<float>();
    chatMessage.mTime = std::chrono::system_clock::now();
    chatMessage.mPercent = 0.f;
    mMessages.push_back(chatMessage);
}

// Minecraft: Java edition styled chat
void CustomChat::onRenderEvent(RenderEvent& event)
{
    FontHelper::pushPrefFont();
    auto drawList = ImGui::GetBackgroundDrawList();
    const auto delta = ImGui::GetIO().DeltaTime;

    ImVec2 displaySize = ImGui::GetIO().DisplaySize;
    ImVec2 windowSize = {600, 450};
    ImVec2 windowPos = {10, displaySize.y - 85.f};
    float rounding = 15.f;
    float totalHeight = 0.f;
    float fontSize = 20.0f;

    static float easedHeight = 0.f;
    static float maxHeight = 0.f;
    ImRect rect = ImVec4(windowPos.x, windowPos.y - 10.f, windowPos.x + windowSize.x, windowPos.y - 10 - easedHeight);
    ImRect flipped = ImVec4(rect.Min.x, rect.Max.y, rect.Max.x, rect.Min.y);

    if (rect.Min.y - rect.Max.y >= 1)
    {
        ImRenderUtils::addBlur(rect.ToVec4(), 3, rounding);
        drawList->AddRectFilled(flipped.Min, flipped.Max, IM_COL32(0, 0, 0, 200), rounding);
        drawList->PushClipRect({rect.Min.x, rect.Max.y}, {rect.Max.x, rect.Min.y});

        auto fontHeight = ImGui::GetFont()->CalcTextSizeA(
                    fontSize,
                    FLT_MAX,
                    0,
                    ""
                ).y;

        ImVec2 cursorPos = {windowPos.x + 10, windowPos.y - 10.f};
        auto now = std::chrono::system_clock::now();
        auto isInGyat = ClientInstance::get()->getScreenName() == "chat_screen";

        for (auto it = mMessages.rbegin(); it != mMessages.rend(); ) {
            float elapsed = std::chrono::duration<float>(now - it->mTime).count();
            bool hasElapsed = elapsed >= it->mLifeTime;

            if (hasElapsed) {
                it->mPercent -= delta * 2.5f;
                if (easedHeight > cursorPos.y) {
                    it = std::reverse_iterator(mMessages.erase((++it).base()));
                    continue;
                }
            }else {
                it->mPercent = MathUtils::lerp(it->mPercent, 1.f, delta * 8.f);
            }
            it->mPercent = std::clamp(it->mPercent, 0.f, 1.f);

            if (it->mPercent > 0.0f) {
                cursorPos.y = MathUtils::lerp(cursorPos.y, cursorPos.y - fontHeight - 5.0f, hasElapsed ? 1.f : it->mPercent);
                int alpha = static_cast<int>(255 * (!hasElapsed ? it->mPercent : 1.f)); // fade in no fae out :twerk:

                drawList->AddText(
                    ImGui::GetFont(),
                    fontSize,
                    cursorPos,
                    IM_COL32(255, 255, 255, alpha),
                    it->mText.c_str()
                );

                // visualize repeated messages
                if (it->mCount > 1) {
                    std::string countText = " x" + std::to_string(it->mCount);
                    drawList->AddText(
                        ImGui::GetFont(),
                        fontSize,
                        {cursorPos.x + ImGui::CalcTextSize(it->mText.c_str()).x + 5.0f, cursorPos.y},
                        IM_COL32(170, 170, 170, alpha),
                        countText.c_str()
                    );
                }
            }
            if(!hasElapsed) {
                if (mMessages.size() < 12) {
                    totalHeight += (fontHeight + 5.0f);
                    maxHeight = totalHeight;
                } else {
                    totalHeight = maxHeight;
                }
            }
            ++it;
        }

        easedHeight = MathUtils::lerp(easedHeight, isInGyat ?  (fontHeight + 5.0f) * 12 : totalHeight, delta * 8.f);
        drawList->PopClipRect();
        FontHelper::popPrefFont();
    } else {
        bool isInChatScreen = ClientInstance::get()->getScreenName() == "chat_screen";
        auto fontHeight = ImGui::GetFont()->CalcTextSizeA(
                    fontSize,
                    FLT_MAX,
                    0,
                    ""
                ).y;

        easedHeight = MathUtils::lerp(easedHeight, isInChatScreen ?  (fontHeight + 5.0f) * 12 : totalHeight, delta * 8.f);
        FontHelper::popPrefFont();
    }
}


void CustomChat::onPacketInEvent(PacketInEvent& event)
{
    if (event.isCancelled()) return;
    if (event.mPacket->getId() != PacketID::Text) return;
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    auto textPacket = event.getPacket<TextPacket>();
    std::string message = textPacket->mMessage;

    if (textPacket->mType == TextPacketType::Chat) {
        //message = "<" + textPacket->mAuthor + "> " + message;
        message = textPacket->mAuthor.empty() ? message : "<" + textPacket->mAuthor + "> " + message;
    }

    if (!mMessages.empty() && mMessages.back().mText == message) {
        mMessages.back().mCount++;
        mMessages.back().mCount++;
        mMessages.back().mTime = std::chrono::system_clock::now();
        mMessages.back().mLifeTime = mMaxLifeTime.as<float>();
        mMessages.back().mPercent = 0.f;
        mCachedMessages.push_back(mMessages.back());
        return;
    }

    ChatMessage chatMessage;
    chatMessage.mText = message;
    chatMessage.mLifeTime = mMaxLifeTime.as<float>();
    chatMessage.mTime = std::chrono::system_clock::now();
    chatMessage.mPercent = 0.f;
    chatMessage.mCount = 1;

    mMessages.push_back(chatMessage);
    mCachedMessages.push_back(chatMessage);
}
