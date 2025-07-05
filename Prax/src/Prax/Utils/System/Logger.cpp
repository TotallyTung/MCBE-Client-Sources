#pragma once

#include <iostream>
#include <fstream>
#include <imgui.h>

#include "Internet.h"

#ifdef __DEBUG__
#define INCLUDE_LOGGER 1
#endif


void Logger::Init() {

#ifdef INCLUDE_LOGGER
    // If the log directory doesn't exist, create it.
    FileSystem::CreateDirectoryP(FileSystem::Directory);
    FileSystem::CreateDirectoryP(FileSystem::LogDirectory);

    // Allocate a new console for logging.
    AllocConsole();
    // Set da console title
    SetConsoleTitle("Catgirl Console");
    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);

    // Set the console to enable virtual terminal processing.
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#endif


}

void SetConsoleColor(WORD color) {
#ifdef INCLUDE_LOGGER
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
#endif
}

// Method to get a color escape code from rgb floats
std::string rgb(float r, float g, float b) {

    return "\x1b[38;2;" + std::to_string((int)(r * 255)) + ";" + std::to_string((int)(g * 255)) + ";" + std::to_string((int)(b * 255)) + "m";
}

// Method to get a color reset escape code
std::string reset() {
    return "\x1b[0m";
}

static auto logs = std::vector<std::pair<std::string, int>>();

void Logger::Write(const std::string& sender, const std::string& text, LogType type, bool newline) {
#ifdef INCLUDE_LOGGER
    std::string output = "[" + sender + "] ";

    std::string processedText{output + text};

    logs.emplace_back(processedText, type);

    switch (type) {
        case LogType::Info:
            // Prepend the escape code for green to the output
            output = rgb(0, 0.85f, 0) + output;
            output += "[INFO] " + text;
            break;
        case LogType::Warning:
            // Prepend the escape code for yellow to the output
            output = rgb(0.85f, 0.85f, 0) + output;
            output += "[WARN] " + text;
            break;
        case LogType::Error:
            // Prepend the escape code for red to the output
            output = rgb(0.85f, 0, 0) + output;
            output += "[ERROR] " + text;
            break;
        case LogType::Debug:
            // Prepend the escape code for cyan to the output
            output = rgb(0, 0.70f, 0.85f) + output;
            output += "[DEBUG] " + text;
            break;
    }


    WriteS(output + reset(), newline);

#endif
}

void Logger::Write(const std::string& sender, LogType type, char const* const text, ...) {
#ifdef INCLUDE_LOGGER
    std::string output = "[" + sender + "] ";

    std::string textS = text;

    std::string processedText{output + textS};

    logs.emplace_back(processedText, type);

    switch (type) {
    case LogType::Info:
        // Prepend the escape code for green to the output
        output = rgb(0, 0.85f, 0) + output;
        output += "[INFO] " + textS;
        break;
    case LogType::Warning:
        // Prepend the escape code for yellow to the output
        output = rgb(0.85f, 0.85f, 0) + output;
        output += "[WARN] " + textS;
        break;
    case LogType::Error:
        // Prepend the escape code for red to the output
        output = rgb(0.85f, 0, 0) + output;
        output += "[ERROR] " + textS;
        break;
    case LogType::Debug:
        // Prepend the escape code for cyan to the output
        output = rgb(0, 0.70f, 0.85f) + output;
        output += "[DEBUG] " + textS;
        break;
    }

    WriteS(output + reset());
#endif
}

void Logger::WriteS(const std::string& text, bool newline) {
    // Write to file
    std::ofstream file;
    file.open(FileSystem::LogDirectory + "\\client.log", std::ios::app);
    file << text << std::endl;
    file.close();

#ifdef INCLUDE_LOGGER
    // Write to console
if (newline)
    std::cout << text << std::endl;
else
    std::cout << text;
#endif
}

const std::vector<std::pair<std::string, int>>& Logger::GetLogs() {
    return logs;
}

void Logger::Shutdown()
{
#ifdef INCLUDE_LOGGER
    Logger::Write("Logger", "You may now close this window.");
    FreeConsole();
#endif
}