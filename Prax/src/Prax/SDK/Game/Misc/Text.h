#pragma once

#include <imgui.h>
#include <string>

class TextUtils {
public:
    enum ChatColor
    {
        Black = '0',
        DarkBlue = '1',
        DarkGreen = '2',
        DarkAqua = '3',
        DarkRed = '4',
        DarkPurple = '5',
        Gold = '6',
        Gray = '7',
        DarkGray = '8',
        Blue = '9',
        Green = 'a',
        Aqua = 'b',
        Red = 'c',
        LightPurple = 'd',
        Yellow = 'e',
        White = 'f',
        LightGold = 'g',
        Obfuscated = 'k',
        Bold = 'l',
        Strikethrough = 'm',
        Underline = 'n',
        Italic = 'o',
        Reset = 'r'
    };

    // ImChatColors (map of § to ImColor)
    static inline std::unordered_map<char, ImColor> ImChatColors = {
        { '0', ImColor(0, 0, 0, 255) },
        { '1', ImColor(0, 0, 170, 255) },
        { '2', ImColor(0, 170, 0, 255) },
        { '3', ImColor(0, 170, 170, 255) },
        { '4', ImColor(170, 0, 0, 255) },
        { '5', ImColor(170, 0, 170, 255) },
        { '6', ImColor(255, 170, 0, 255) },
        { '7', ImColor(170, 170, 170, 255) },
        { '8', ImColor(85, 85, 85, 255) },
        { '9', ImColor(85, 85, 255, 255) },
        { 'a', ImColor(85, 255, 85, 255) },
        { 'b', ImColor(85, 255, 255, 255) },
        { 'c', ImColor(255, 85, 85, 255) },
        { 'd', ImColor(255, 85, 255, 255) },
        { 'e', ImColor(255, 255, 85, 255) },
        { 'f', ImColor(255, 255, 255, 255) },
        { 'g', ImColor(255, 255, 85, 255) },
        { 'k', ImColor(0, 0, 0, 255) },
        { 'l', ImColor(255, 255, 255, 255) },
        { 'm', ImColor(255, 255, 255, 255) },
        { 'n', ImColor(255, 255, 255, 255) },
        { 'o', ImColor(255, 255, 255, 255) },
        { 'r', ImColor(255, 255, 255, 255) }};

    static std::string AddColorsToString(std::string str)
    {
        std::string newStr = "";
        for (int i = 0; i < str.length(); i++)
        {
            //logr::write("Checking: " + std::to_string(i) + " " + str[i]);
            if (std::tolower(str[i]) == '&')
            {
                //logr::write("ok: " + std::to_string(i) + " " + str[i + 1]);
                const unsigned char ccc = str[i + 1];
                unsigned int ci = ccc;
                char ca = ci;
                const auto color = static_cast<ChatColor>(ci);
                //logr::write("Color: " + GetColor(color));
                newStr += "\xC2\xA7";
                newStr += color;
                i++;
            }
            else
            {
                newStr += str[i];
            }
        }
        return newStr;
    }
    static std::string GetColor(ChatColor color)
    {
        std::string empty = "";
        empty += "\xC2\xA7";//
        empty += color;
        return empty;
    }
};

class TextHolder
{
public:

    union {
        char inlineText[16]; //0x0000
        char* pText; //0x0000
    };

    size_t textLength; //0x0010
    size_t alignedTextLength; //0x0018

    TextHolder() {
        memset(this, 0, sizeof(TextHolder));
    }

    TextHolder(int) {
        memset(this, 0, sizeof(TextHolder));
    }

    TextHolder(std::string str) {
        memset(this, 0, sizeof(TextHolder));
        textLength = str.size();
        alignedTextLength = textLength | 0xF;
        if (str.size() < 16)
            strcpy_s(inlineText, 16, str.c_str());
        else {
            size_t size = str.size();
            pText = reinterpret_cast<char*>(malloc(alignedTextLength + 1));
            if (pText != 0x0) {
                strcpy_s(pText, size + 1, str.c_str());
            }
        }
    }

    TextHolder(void* ptr, size_t sizeOfData) {
        memset(this, 0, sizeof(TextHolder));
        textLength = sizeOfData;
        alignedTextLength = sizeOfData;
        if (textLength < 16)
            memcpy(inlineText, ptr, sizeOfData);
        else
            pText = reinterpret_cast<char*>(ptr);
    }

    ~TextHolder() {
        /*if (textLength >= 16 && pText != nullptr)
            free(pText);*/
    }

    char* getText() {
        if (alignedTextLength < 16)
            return this->inlineText;
        else
            return this->pText;
    }

    size_t getTextLength() {
        return textLength;
    }

    void setText(std::string str) {
        memset(this, 0, sizeof(TextHolder));
        textLength = str.size();
        alignedTextLength = textLength | 0xF;
        if (str.size() < 16)
            strcpy_s(inlineText, 16, str.c_str());
        else {
            size_t size = str.size();
            char* ptr = reinterpret_cast<char*>(malloc(alignedTextLength + 1));
            if (ptr != 0x0) {
                strcpy_s(ptr, size + 1, str.c_str());
            }

            pText = ptr;
        }
    }

    void resetWithoutDelete() {
        memset(this, 0, sizeof(TextHolder));
    }

};
