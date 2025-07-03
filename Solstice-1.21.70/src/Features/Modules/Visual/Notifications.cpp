//
// Created by vastrakai on 7/4/2024.
//

#include "Notifications.hpp"

#include <Features/Events/ConnectionRequestEvent.hpp>
#include <Features/Events/NotifyEvent.hpp>
#include <Features/Events/RenderEvent.hpp>

void Notifications::onEnable()
{
    gFeatureManager->mDispatcher->listen<NotifyEvent, &Notifications::onNotifyEvent>(this);
    gFeatureManager->mDispatcher->listen<ModuleStateChangeEvent, &Notifications::onModuleStateChange>(this);
    gFeatureManager->mDispatcher->listen<ModuleScriptStateChangeEvent, &Notifications::onModuleScriptStateChange>(this);
    gFeatureManager->mDispatcher->listen<ConnectionRequestEvent, &Notifications::onConnectionRequestEvent>(this);
}

void Notifications::onDisable()
{
    gFeatureManager->mDispatcher->deafen<NotifyEvent, &Notifications::onNotifyEvent>(this);
    gFeatureManager->mDispatcher->deafen<ModuleStateChangeEvent, &Notifications::onModuleStateChange>(this);
    gFeatureManager->mDispatcher->deafen<ModuleScriptStateChangeEvent, &Notifications::onModuleScriptStateChange>(this);
    gFeatureManager->mDispatcher->deafen<ConnectionRequestEvent, &Notifications::onConnectionRequestEvent>(this);
}

bool CalcSize(ImVec2& boxSize, float& yOff, float& x, ImVec2 screenSize, Notification* notification) {
    // skidding, i don't even care i just want this to work :sob:
    float beginX = screenSize.x - boxSize.x - 10.f;
    float endX = screenSize.x + boxSize.x;

    x = MathUtils::lerp(endX, beginX, notification->mCurrentDuration);
    yOff = MathUtils::lerp(yOff , yOff - boxSize.y, notification->mCurrentDuration);

    if (x > screenSize.x + boxSize.x && yOff > screenSize.y + boxSize.y) return true;

    return false;
}

void Notifications::onRenderEvent(RenderEvent& event)
{
    FontHelper::pushPrefFont(true);
    ImVec2 displaySize = ImGui::GetIO().DisplaySize;
    auto drawList = ImGui::GetBackgroundDrawList();
    float y = displaySize.y - 10.0f;
    float x;
    float delta = ImGui::GetIO().DeltaTime;

    // First, remove any notifications that have expired (only if 10 seconds have passed since the notification was shown)
    std::erase_if(mNotifications, [](const Notification& notification) { return notification.mIsTimeUp && notification.mTimeShown > notification.mDuration + 3.0f; });

    int colIndex = mNotifications.size() - 1;
    int i = 0;
    for (auto& notification : mNotifications)
    {
        if (i > mMaxNotifications.mValue && mLimitNotifications.mValue) break;
        notification.mTimeShown += delta;
        notification.mIsTimeUp = notification.mTimeShown >= notification.mDuration;
        notification.mCurrentDuration = MathUtils::lerp(notification.mCurrentDuration,  notification.mIsTimeUp ? 0.0f : 1.0f, delta * 5.0f);

        float percentDone = notification.mTimeShown / notification.mDuration;
        // Clamp the percentage done to 0-1
        percentDone = std::clamp(percentDone, 0.0f, 1.0f);

        constexpr float fontSize = 20.0f;

        const auto size = ImGui::GetFont()->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, notification.mMessage.c_str()).x;
        float sizey = ImGui::GetFont()->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, notification.mMessage.c_str()).y;
        // make sure the boxSize accounts for the text's height
        auto boxSize = ImVec2(fmax(200.0f, 50 + size), sizey + 30.0f);


        if (CalcSize(boxSize, y, x, displaySize, &notification)) continue;

        // Draw a rect
        ImColor themeColor = ColorUtils::getThemedColor(y * 2);

        if (notification.mType == Notification::Type::Warning)
            themeColor = ImColor(1.f, 0.8f, 0.f, 1.f);
        else if (notification.mType == Notification::Type::Error)
            themeColor = ImColor(1.f, 0.f, 0.f, 1.f);

        themeColor.Value.w = 0.7f;
        float max = x + boxSize.x;
        ImVec2 progMax = ImVec2(x + (boxSize.x * percentDone + 6.f), y + (boxSize.y - 10.f));
        progMax.x = std::clamp(progMax.x, x, max);
        ImVec2 bgMin = ImVec2(x + boxSize.x * percentDone, y);
        ImVec2 bgMax = ImVec2(x + boxSize.x, y + (boxSize.y - 10.f));

        drawList->AddShadowRect(ImVec2(x, y), progMax, ImColor(themeColor.Value.x, themeColor.Value.y, themeColor.Value.z, 1.f), 50.f, ImVec2(), 0, 5.0f);
        drawList->PushClipRect(ImVec2(x, y), ImVec2(x + (boxSize.x * percentDone), y + (boxSize.y - 10.f)));

        if (!mColorGradient.mValue)
        {
            drawList->AddRectFilled(ImVec2(x, y), progMax,
                                             themeColor, 5.0f);
        } else
        {
            // uhhh theres no rounding on this..... 😭
            ImColor rgb2 = ColorUtils::getThemedColor(y * 2 + ((x - progMax.x) * 1.2));
            ImRenderUtils::fillGradientOpaqueRectangle(ImVec4(x, y, progMax.x, progMax.y), themeColor, rgb2, 1.f, 1.f);
        }
        drawList->PopClipRect();

        drawList->PushClipRect(bgMin, bgMax);
        drawList->AddRectFilled(ImVec2(x + boxSize.x * percentDone - 6, y), bgMax,
                                ImColor(0.f, 0.f, 0.f, 0.7f), 5.0f);
        drawList->PopClipRect();

        // Draw text
        drawList->AddText(ImGui::GetFont(), fontSize, ImVec2(x + 10, y + 10), ImColor(255, 255, 255, 255), notification.mMessage.c_str());
        colIndex--;
        if (!notification.mIsTimeUp) i++;
    }

    ImGui::PopFont();
}

void Notifications::onModuleStateChange(ModuleStateChangeEvent& event)
{
    if (event.isCancelled()) return;
    const auto notification = Notification(event.mModule->getName() + " was " + (event.mEnabled ? "enabled" : "disabled"), Notification::Type::Info, 3.0f);
    mNotifications.push_back(notification);
}

void Notifications::onModuleScriptStateChange(ModuleScriptStateChangeEvent& event)
{
    if (event.isCancelled()) return;
    const auto notification = Notification(event.mModule->moduleName + " was " + (event.mEnabled ? "enabled" : "disabled"), Notification::Type::Info, 3.0f);
    mNotifications.push_back(notification);
}

void Notifications::onConnectionRequestEvent(ConnectionRequestEvent& event)
{
    const auto notification = Notification("Connecting to " + *event.mServerAddress + "...", Notification::Type::Info, 6.0f);
    mNotifications.push_back(notification);
}

void Notifications::onNotifyEvent(NotifyEvent& event)
{
    mNotifications.push_back(event.mNotification);
}
