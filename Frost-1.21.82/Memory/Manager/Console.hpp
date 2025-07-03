#pragma once

#include <windows.h>
#include <cstdio>

class Console {
public:
    static void CreateConsole(const char* title) {
        if (!GetConsoleWindow()) {
            AllocConsole();
            freopen_s(&f, "CONOUT$", "w", stdout);
            freopen_s(&f, "CONIN$", "r", stdin);
            SetConsoleTitleA(title);
        }
        else {
            ShowWindow(GetConsoleWindow(), SW_SHOW);
        }
    }

    static void CloseConsole() {
        ShowWindow(GetConsoleWindow(), SW_HIDE);
        ClearConsole();
    }

    static void ClearConsole() {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hConsole == INVALID_HANDLE_VALUE) return;

        CONSOLE_SCREEN_BUFFER_INFO csbi;
        DWORD count;
        COORD homeCoords = { 0, 0 };

        if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return;

        FillConsoleOutputCharacter(hConsole, ' ', csbi.dwSize.X * csbi.dwSize.Y, homeCoords, &count);
        SetConsoleCursorPosition(hConsole, homeCoords);
    }

    static void debug(const std::string& message) {
        setColor(8);  // Gray
        std::cout << "[";
        setColor(11); // Cyan
        std::cout << "Frost";
        setColor(8);  // Gray
        std::cout << "] [";
        setColor(9);  // Blue
        std::cout << "Debug";
        setColor(8);  // Gray
        std::cout << "] ";
        resetColor();
        std::cout << message << std::endl;
    }

    static void info(const std::string& message) {
        setColor(8);  // Gray
        std::cout << "[";
        setColor(11); // Cyan
        std::cout << "Frost";
        setColor(8);  // Gray
        std::cout << "] [";
        setColor(11); // Cyan
        std::cout << "Info";
        setColor(8);  // Gray
        std::cout << "] ";
        resetColor();
        std::cout << message << std::endl;
    }

    static void critical(const std::string& message) {
        setColor(8);  // Gray
        std::cout << "[";
        setColor(11); // Cyan
        std::cout << "Frost";
        setColor(8);  // Gray
        std::cout << "] [";
        setColor(12); // Red
        std::cout << "Critical";
        setColor(8);  // Gray
        std::cout << "] ";
        resetColor();
        std::cout << message << std::endl;
    }
private:
    static inline FILE* f = nullptr;

    static void setColor(int color) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    }

    static void resetColor() {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);  // Reset to default color
    }
};
