//
// Created by Flash on 8/10/2023.
//
#pragma once

#include <map>

class KeyManager {
public:



    static inline std::map<std::string, uint64_t> KeyMap = {//I am sad this exists
            { "a", 'A'},
            { "b", 'B'},
            { "c", 'C'},
            { "d", 'D'},
            { "e", 'E'},
            { "f", 'F'},
            { "g", 'G'},
            { "h", 'H'},
            { "i", 'I'},
            { "j", 'J'},
            { "k", 'K'},
            { "l", 'L'},
            { "m", 'M'},
            { "n", 'N'},
            { "o", 'O'},
            { "p", 'P'},
            { "q", 'Q'},
            { "r", 'R'},
            { "s", 'S'},
            { "t", 'T'},
            { "u", 'U'},
            { "v", 'V'},
            { "w", 'W'},
            { "x", 'X'},
            { "y", 'Y'},
            { "z", 'Z'},
            { "alt", VK_MENU},
            { "insert", VK_INSERT},
            { "num0", VK_NUMPAD0},
            { "num1", VK_NUMPAD1},
            { "num2", VK_NUMPAD2},
            { "num3", VK_NUMPAD3},
            { "num4", VK_NUMPAD4},
            { "num5", VK_NUMPAD5},
            { "num6", VK_NUMPAD6},
            { "num7", VK_NUMPAD7},
            { "num8", VK_NUMPAD8},
            { "num9", VK_NUMPAD9},
            { "f1", VK_F1},
            { "f2", VK_F2},
            { "f3", VK_F3},
            { "f4", VK_F4},
            { "f5", VK_F5},
            { "f6", VK_F6},
            { "f7", VK_F7},
            { "f8", VK_F8},
            { "f9", VK_F9},
            { "f10", VK_F10},
            { "f11", VK_F11},
            { "f12", VK_F12},
            { "`", VK_OEM_7},
            { "[", VK_OEM_4},
            { "]", VK_OEM_6},
            { "/", VK_OEM_2},
            { R"("\")", VK_OEM_5},
            { "tab", VK_TAB},
            { "delete", VK_DELETE},
            { "home", VK_HOME},
            { "end", VK_END},
            { "none", 0}
    };

    static inline std::map<std::string, uint64_t>::iterator it;

    static std::map<uint64_t, bool> Keys;

    static uint64_t GetFirstDown();
};
