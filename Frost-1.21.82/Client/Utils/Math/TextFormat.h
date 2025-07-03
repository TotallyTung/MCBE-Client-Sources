#pragma once

class TextFormat {
public:
    enum class Color {
        BLACK, DARK_BLUE, DARK_GREEN, DARK_AQUA, DARK_RED, DARK_PURPLE, GOLD, GRAY,
        DARK_GRAY, BLUE, GREEN, AQUA, RED, LIGHT_PURPLE, YELLOW, WHITE
    };

    enum class Style {
        OBFUSCATED, BOLD, STRIKETHROUGH, UNDERLINE, ITALIC, RESET
    };

    static std::string getSymbol() {
        return "\xC2\xA7"; // Minecraft color code symbol.
    }

    static std::string getColorCode(Color color) {
        static const std::unordered_map<Color, std::string> colorCodes = {
            {Color::BLACK, "0"}, {Color::DARK_BLUE, "1"}, {Color::DARK_GREEN, "2"},
            {Color::DARK_AQUA, "3"}, {Color::DARK_RED, "4"}, {Color::DARK_PURPLE, "5"},
            {Color::GOLD, "6"}, {Color::GRAY, "7"}, {Color::DARK_GRAY, "8"},
            {Color::BLUE, "9"}, {Color::GREEN, "a"}, {Color::AQUA, "b"},
            {Color::RED, "c"}, {Color::LIGHT_PURPLE, "d"}, {Color::YELLOW, "e"},
            {Color::WHITE, "f"}
        };
        auto it = colorCodes.find(color);
        if (it != colorCodes.end()) {
            return getSymbol() + it->second;
        }
        throw std::invalid_argument("Invalid color");
    }

    static std::string getStyleCode(Style style) {
        static const std::unordered_map<Style, std::string> styleCodes = {
            {Style::OBFUSCATED, "k"}, {Style::BOLD, "l"}, {Style::STRIKETHROUGH, "m"},
            {Style::UNDERLINE, "n"}, {Style::ITALIC, "o"}, {Style::RESET, "r"}
        };
        auto it = styleCodes.find(style);
        if (it != styleCodes.end()) {
            return getSymbol() + it->second;
        }
        throw std::invalid_argument("Invalid style");
    }

    static std::string applyFormat(const std::string& text, Color color, const std::initializer_list<Style>& styles = {}) {
        std::string formattedText = getColorCode(color);
        for (const auto& style : styles) {
            formattedText += getStyleCode(style);
        }
        formattedText += text + getStyleCode(Style::RESET);
        return formattedText;
    }

    static std::string removeFormat(const std::string& text) {
        std::string result;
        size_t i = 0;
        while (i < text.size()) {
            if (text[i] == '\xC2' && i + 1 < text.size() && text[i + 1] == '\xA7') {
                i += 3; // Skip \xC2\xA7 and the following format character.
            }
            else {
                result += text[i++];
            }
        }
        return result;
    }
};