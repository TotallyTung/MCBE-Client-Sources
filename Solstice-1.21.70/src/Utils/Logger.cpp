//
// Created by vastrakai on 6/24/2024.
//

#include "Logger.hpp"

#include <Solstice.hpp>
#include <string>



void Logger::initialize()
{
    if (initialized) return;
    AllocConsole();

    SetConsoleTitle("Solstice Console");

    freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
    freopen_s(reinterpret_cast<FILE**>(stdin), "CONIN$", "r", stdin);
    freopen_s(reinterpret_cast<FILE**>(stderr), "CONOUT$", "w", stderr);

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
    initialized = true;
}

void Logger::deinitialize()
{
    // Close the console
    fclose(stdout);
    fclose(stdin);
    fclose(stderr);
    // Free the console
    FreeConsole();
}

std::string Logger::getAnsiColor(float r, float g, float b)
{
    return "\033[38;2;" + std::to_string(static_cast<int>(r * 255)) + ";" + std::to_string(static_cast<int>(g * 255)) + ";" + std::to_string(static_cast<int>(b * 255)) + "m";
}

std::string Logger::getAnsiColor(int r, int g, int b)
{
    return "\033[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
}

