#pragma once

#include <string>
#include "FileSystem.h"

class Logger {
public:
    enum LogType {
        Debug = -1,
        Info = 0,
        Warning,
        Error
    };
    static void Init();

    static void Write(const std::string& sender = "", const std::string& text = "", LogType type = LogType::Info, bool newline = true);
    static void Write(const std::string& sender, LogType type, const char * text, ...);
    static void WriteS(const std::string& text, bool newline = true);
    static const std::vector<std::pair<std::string, int>>& GetLogs();
    static void Shutdown();
};

