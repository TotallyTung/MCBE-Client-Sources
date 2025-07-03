#pragma once
#include "NotificationBase.h"

class Notification {
public:
    static void renderNotifications() {
        static NotificationBase noti;
        auto& notifications = noti.getList();
        float yOffset = Game::Core::ClientInstance->getGuiData()->WindowResolution2.y - 50;
        const float textSize = 1.2f;

        for (auto& notif : notifications) {
            if (notif->title.back() == ':') {
                notif->title = notif->title.substr(0, notif->title.length() - 1);
            }
            float elapsedTime = notif->getElapsedTime();
            float duration = notif->duration;
            const float firstPos = Game::Core::ClientInstance->getGuiData()->WindowResolution2.x - 200 - 8.f;
            const float lastPos = Game::Core::ClientInstance->getGuiData()->WindowResolution2.x + 8.f;
            const float speed = 1.f;
            static float fade = 0;

            EasingUtil easing;
            easing.percentage = (elapsedTime > duration) ? 1.0f - ((elapsedTime - duration) / speed) : elapsedTime / speed;
            float posX = (easing.percentage < 1) ? Math::lerp(lastPos, firstPos, (elapsedTime > duration) ? easing.easeInExpo() : easing.easeOutExpo()) : ((elapsedTime > duration) ? lastPos : firstPos);
            fade = (easing.percentage < 1) ? Math::lerp(0, 1.f, (elapsedTime > duration) ? easing.easeInExpo() : easing.easeOutExpo()) : 1.f;

            if (elapsedTime > duration && easing.percentage <= 0) notif->isOpen = false;

            Vector4<float> rect(posX, yOffset - 30, posX + 200, yOffset + 30);
            ImRenderUtil::fillRectangle(rect, UIColor(0, 0, 0), 0.5f, 10.f);
            {
                float textWidth = ImRenderUtil::getTextWidth(&notif->title, textSize);
                ImRenderUtil::drawText(Vector2<float>(posX + 100 - textWidth / 2, yOffset - 20), &notif->title, UIColor(255, 255, 255), textSize, fade, true);
            }
            {
                float textWidth = ImRenderUtil::getTextWidth(&notif->message, textSize);
                ImRenderUtil::drawText(Vector2<float>(posX + 100 - textWidth / 2, yOffset), &notif->message, UIColor(255, 255, 255), textSize, fade, true);
            }

            // Calculate progress bar percentage
            float progressPercentage = (elapsedTime > duration) ? 0.0f : (elapsedTime / duration);

            // Draw progress bar
            Vector4<float> progressBarRect(posX, yOffset + 30, posX + 200 * progressPercentage, yOffset + 32);
            ImRenderUtil::fillRectangle(progressBarRect, UIColor(253, 223, 231), 1.0f, 0.0f);

            yOffset = Math::lerp(yOffset, yOffset - 50 - 16, (elapsedTime > duration) ? easing.easeInExpo() : easing.easeOutExpo());
        }
    }
};
