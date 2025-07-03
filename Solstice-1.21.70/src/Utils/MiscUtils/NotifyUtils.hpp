#pragma once
//
// Created by vastrakai on 7/4/2024.
//

#include <Solstice.hpp>
#include <Features/Events/NotifyEvent.hpp>

class NotifyUtils {
public:
    static void notify(std::string message, float duration, Notification::Type type);
};