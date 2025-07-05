//
// Created by Flash on 10/5/2023.
//

#include <Prax/Utils/Render/D2D.h>
#include <Prax/Utils/String.h>
#include <Prax/Module/Modules/Visual/HUD.h>
#include <Prax/Module/ModuleManager.h>
#include "Notifications.h"

#define ANIMATION_SPEEDX 6.f
#define ANIMATION_SPEEDY 10.f
#define BOX_VERITICAL_OFFSET 10.f
#define BOX_HORIZONTAL_OFFSET 10.f
#define DRAW_LIST ImGui::GetForegroundDrawList()

std::vector<std::unique_ptr<Notification>> Notifications::List = {};

// This was the easiest way without repeating code
bool Notifications::CalcSize(ImVec2& boxSize, float& yOff, float& x, ImVec2 screenSize, Notification* notification) {
    // Create dimensions for the notification, base
    float beginX = screenSize.x - boxSize.x -
                   BOX_HORIZONTAL_OFFSET; // This is where the notification is supposed to stay whenever it is fully being displayed
    float endX = screenSize.x + boxSize.x; // The box slides from and to this position

    // Calculate the position of the notification
    x = Math::Lerp(endX, beginX, notification->CurrentDuration);
    yOff = Math::Lerp(yOff , yOff - boxSize.y, notification->CurrentDuration);

    // Continue if both of these positions are off the screen
    if (x > screenSize.x + boxSize.x && yOff > screenSize.y + boxSize.y) return true;

    return false;
}

void Notifications::Render() {
    static HUD* hud = ModuleManager::GetModule<HUD>();

    ImVec2 screenSize =  ImGui::GetIO().DisplaySize;
    if (screenSize.x == 0) return;

    // Loop through all notifications checking if they should be removed
    // This is needed because we can't remove them while looping through them normally
    for (int i = 0; i < List.size(); i++) {
        auto notification = List.at(i).get();
        bool isTimeUp = true;

        if (notification)
        {
            isTimeUp = notification->TimeShown >= notification->Duration + 10.f;
        }

        if (isTimeUp)
        {
            List.erase(List.begin() + i);
        }
    }

    float delta = ImGui::GetIO().DeltaTime;
    float yOff = screenSize.y;

    // Remove notifications where ptr.get() == nullptr
    for (const auto & i : List)
    {
        auto notification = i.get();
        if (!notification) continue;

        // Moved some things up here because they can be used by other notification styles
        notification->TimeShown += delta;
        notification->IsTimeUp = notification->TimeShown >= notification->Duration;
        notification->CurrentDuration = Math::Lerp(notification->CurrentDuration,
                                                   notification->IsTimeUp ? 0.0f : 1.0f,
                                                   ANIMATION_SPEEDX * delta);

        float percentDone = notification->TimeShown / notification->Duration;

        std::string icon = Font::IconFontToString(IconFont::ManyGears);
        switch (notification->Type) {
            case NotificationType::Info: // Gear icon
                icon = Font::IconFontToString(IconFont::InfoI);
                break;
            case NotificationType::Warning: // Flag icon
                icon = Font::IconFontToString(IconFont::Flag);
                break;
            case NotificationType::Error: // Flag
                icon = Font::IconFontToString(IconFont::Flag);
                break;
            case NotificationType::Enable: // Checkmark
                icon = Font::IconFontToString(IconFont::Checkmark);
                break;
            case NotificationType::Disable: // X
                icon = Font::IconFontToString(IconFont::Cross);
                break;
        }

        // stupid cpp compiler
        std::string timeLeft;
        ImVec2 boxSize;
        switch (hud->NotificationStyle) {
            case 0: // Flash
                // time left text down to the hundredth
                timeLeft = std::to_string((int) (notification->Duration - notification->TimeShown)) + "." +
                                       std::to_string((int) ((notification->Duration - notification->TimeShown) * 10) % 10) + "s";

                float x;
                boxSize = ImVec2(300, 100);
                if (Notifications::CalcSize(boxSize, yOff, x, screenSize, notification)) continue;

                // Add Shadow
                DRAW_LIST->AddShadowRect(ImVec2(x, yOff),
                                         ImVec2(x + boxSize.x, yOff + boxSize.y),
                                         ImColor(0.f,0.f,0.f,0.6f),
                                         50.0f,
                                         ImVec2(1.0f, 1.0f), 0.0f);

                DRAW_LIST->AddRectFilled(ImVec2(x, yOff), ImVec2(x + boxSize.x, yOff + (boxSize.y - BOX_VERITICAL_OFFSET)),
                                         ImColor(0, 0, 0, 200), 5.0f);


                // Draw the progress bar
                DRAW_LIST->AddRectFilled(ImVec2(x, yOff + (boxSize.y - BOX_VERITICAL_OFFSET)),
                                         ImVec2(x + boxSize.x * percentDone, yOff + (boxSize.y - BOX_VERITICAL_OFFSET)),
                                         notification->Color, 5.0f);

                // Render time left above progress bar
                DRAW_LIST->AddText(Font::Fonts["greycliff_medium_large"],
                                   15, ImVec2(x + 10, yOff + 70),
                                   ImColor(255, 255, 255, 255),
                                   timeLeft.c_str(),
                                   nullptr,
                                   boxSize.x - 20);

                // Draw the title
                DRAW_LIST->AddText(Font::Fonts["greycliff_bold_large"],
                                   20, ImVec2(x + 10, yOff + 10),
                                   ImColor(255, 255, 255, 255),
                                   notification->Title.c_str(),
                                   nullptr,
                                   boxSize.x - 20);

                // Draw the message
                DRAW_LIST->AddText(Font::Fonts["greycliff_medium_large"],
                                   15, ImVec2(x + 10, yOff + 40),
                                   ImColor(255, 255, 255, 255),
                                   notification->Message.c_str(),
                                   nullptr,
                                   boxSize.x - 20);

                // Add icon in top right
                DRAW_LIST->AddText(Font::Fonts["icons_v1_large"],
                                   25, ImVec2(x + boxSize.x - 35, yOff + 10),
                                   ImColor(255, 255, 255, 255),
                                   icon.c_str(),
                                   nullptr,
                                   boxSize.x - 20);
                break;
            case 1: // Aether
                std::string notifMessage = notification->Message.contains(notification->Title) ? notification->Message : (notification->Title + " - " + notification->Message);
                const char* notifMessageSimple = notifMessage.c_str();

                auto cock = Font::Fonts["greycliff_medium_large"]->CalcTextSizeA(16, screenSize.x, 0.0f, notifMessageSimple,nullptr).x;

                boxSize = ImVec2(fmax(200.0f, 50 + cock), 50);

                if (Notifications::CalcSize(boxSize, yOff, x, screenSize, notification)) continue;

                // BACKGROUND
                DRAW_LIST->AddRectFilled(ImVec2(x, yOff), ImVec2(x + boxSize.x, yOff + (boxSize.y - BOX_VERITICAL_OFFSET)),
                                         ImColor(0, 0, 0, 200), 5.0f);

                //DEBUG HORIZ CENTERLINE
                /*DRAW_LIST->AddLine(ImVec2(x, yOff + (boxSize.y - BOX_VERITICAL_OFFSET) / 2),
                                  ImVec2(x + boxSize.x, yOff + (boxSize.y - BOX_VERITICAL_OFFSET) / 2),
                                  ImColor(255, 255, 255, 255), 1.0f);*/

                // PROGRESS BAR
                DRAW_LIST->AddRectFilled(ImVec2(x + 2, yOff + (boxSize.y - BOX_VERITICAL_OFFSET) - 1),
                                         ImVec2(x + boxSize.x * percentDone, yOff + (boxSize.y - BOX_VERITICAL_OFFSET) - 1),
                                         notification->Color, 5.f);
                // ICON
                auto iconSizeX = Font::Fonts["icons_v1_large"]->CalcTextSizeA(24, boxSize.x, 0.0f, icon.c_str(),nullptr).x;
                DRAW_LIST->AddText(Font::Fonts["icons_v1_large"],
                                   24, ImVec2((x - 2) + 20 - (iconSizeX / 2), yOff + BOX_VERITICAL_OFFSET - 3),
                                   ImColor(255, 255, 255, 255),
                                   icon.c_str(),
                                   nullptr,
                                   0.0f);

                // TEXT
                DRAW_LIST->AddText(Font::Fonts["greycliff_medium_large"],
                                   16, ImVec2(x + 40, yOff + BOX_VERITICAL_OFFSET + 2),
                                   ImColor(255, 255, 255, 255),
                                   notifMessageSimple,
                                   nullptr,
                                   0.0f);
                break;
        }
    }
}

void Notifications::Notify(std::string title, std::string message, float duration) {
    // Create a new notification and add it to the list
    //Notification* notification = new Notification();
    std::unique_ptr<Notification> notification = std::make_unique<Notification>();
    notification->Title = title;
    notification->Message = message;
    notification->Duration = duration;
    notification->Color = ImColor(255, 255, 255, 255);

    List.emplace_back(std::move(notification));
}

void Notifications::Notify(std::string title, std::string message, NotificationType type, float duration) {
    // Create a new notification and add it to the list
    //Notification* notification = new Notification();
    auto notification = std::make_unique<Notification>();
    notification->Title = title;
    notification->Message = message;
    notification->Duration = duration;

    switch (type) {
        case NotificationType::Info: // White
            notification->Color = ImColor(255, 255, 255, 255);
            break;
        case NotificationType::Warning: // Yellow
            notification->Color = ImColor(224, 221, 49, 255);
            break;
        case NotificationType::Error: // Red
            notification->Color = ImColor(255, 50, 70, 255);
            break;
        case NotificationType::Enable: // Green
            notification->Color = ImColor(60, 255, 50, 255);
            break;
        case NotificationType::Disable: // Red
            notification->Color = ImColor(255, 50, 70, 255);
            break;
    }
    notification->Type = type;

    List.emplace_back(std::move(notification));
}