//
// Created by vastrakai on 7/4/2024.
//

#include "NotifyUtils.hpp"

#include <Features/FeatureManager.hpp>
#include <utility>

#include "spdlog/spdlog.h"

void NotifyUtils::notify(std::string message, float duration, Notification::Type type)
{
    const auto notification = Notification(std::move(message), type, duration);
    spdlog::info("[NotifyUtils] Notified: {}", notification.mMessage);
    auto holder = nes::make_holder<NotifyEvent>(notification);
    gFeatureManager->mDispatcher->trigger<NotifyEvent>(holder);
}
