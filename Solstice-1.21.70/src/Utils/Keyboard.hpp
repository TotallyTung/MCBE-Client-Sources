#pragma once
#include <unordered_map>
#include <string>
#include <Windows.h>
//
// Created by vastrakai on 6/29/2024.
//


class Keyboard {
public:
    static inline std::unordered_map<std::string, int> mKeyMap = {
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
        { "f13", VK_F13},
        { "f14", VK_F14},
        { "f15", VK_F15},
        { "f16", VK_F16},
        { "f17", VK_F17},
        { "f18", VK_F18},
        { "f19", VK_F19},
        { "f20", VK_F20},
        { "f21", VK_F21},
        { "f22", VK_F22},
        { "f23", VK_F23},
        { "f24", VK_F24},
        { "`", VK_OEM_7},
        { "[", VK_OEM_4},
        { "]", VK_OEM_6},
        { "/", VK_OEM_2},
        { R"("\")", VK_OEM_5},
        { "tab", VK_TAB},
        { "delete", VK_DELETE},
        { "home", VK_HOME},
        { "end", VK_END},
        // tilda/grave
        { "grave", VK_OEM_3},
        { "backspace", VK_BACK},
        { "enter", VK_RETURN},
        { "space", VK_SPACE},
        { "shift", VK_SHIFT},
        { "ctrl", VK_CONTROL},
        { "up", VK_UP},
        { "down", VK_DOWN},
        { "left", VK_LEFT},
        { "right", VK_RIGHT},
        { "pageup", VK_PRIOR},
        { "pagedown", VK_NEXT},
        { "capslock", VK_CAPITAL},
        { "none", 0}
    };

    static inline std::unordered_map<int, bool> mPressedKeys;

    static int getKeyId(const std::string& str);
    static bool isUsingMoveKeys(bool includeSpaceShift = false);
    static bool isStrafing();
    static std::string getKey(int keyId);
};