#pragma once

#include <vector>
#include <memory>
#include <string>
#include <chrono>

struct NotificationData {
    std::string title;
    std::string message;
    float duration = 3;
    bool isOpen = true;

    std::chrono::time_point<std::chrono::steady_clock> startTime;

    NotificationData(std::string title, std::string message)
        : title(std::move(title)), message(std::move(message)), startTime(std::chrono::steady_clock::now()) {}

    float getElapsedTime() const {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
        return elapsedTime / 1000.0f;
    }
};

class NotificationBase {
public:
    inline static std::vector<std::shared_ptr<NotificationData>> notificationQueue;

    std::vector<std::shared_ptr<NotificationData>>& getList() {
        // Are we populated?
        while (!notificationQueue.empty()) {
            auto& first = notificationQueue.front();
            if (!first->isOpen) {
                notificationQueue.erase(notificationQueue.begin());
                continue;
            }
            break;
        }
        return notificationQueue;
    }

    std::shared_ptr<NotificationData> addNotification(std::string title, std::string message, const float duration = 3.f) {
        auto notification = std::make_shared<NotificationData>(std::move(title), std::move(message));
        notification->duration = duration;
        notificationQueue.push_back(notification);
        return notification;
    }
};
