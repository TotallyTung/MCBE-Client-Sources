//
// Created by Flash on 10/5/2023.
//
#pragma once

#include <string>
#include <vector>
#include <imgui.h>
#include <Prax/Utils/Math.h>
#include <Prax/Utils/Render/Font.h>
#include <src/Prax/SDK/Game/Minecraft.h>

enum NotificationType {
    Info,
    Warning,
    Error,
    Enable,
    Disable
};

struct Notification {
    std::string Title = "";
    std::string Message = "";
    NotificationType Type = NotificationType::Info;
    float CurrentDuration = 0.0f;
    bool IsTimeUp = false;
    float y = 0.0f;
    float TimeShown = 0.0f;
    float Duration = 0.0f;
    ImColor Color = ImColor(1, 1, 1,1);
};


class Notifications {
    static std::vector<std::unique_ptr<Notification>> List;
public:

    static void Render();
    static void Notify(std::string title, std::string message, float duration = 2.5f);
    static void Notify(std::string title, std::string message, NotificationType = NotificationType::Info, float duration = 2.5f);

    static bool CalcSize(ImVec2 &boxSize, float &yOff, float &x, ImVec2 screenSize, Notification *notification);
};