//
// Created by vastrakai on 7/5/2024.
//

#pragma once

#include "Event.hpp"

class ContainerScreenTickEvent : public Event {
public:
    class ContainerScreenController* mController{};

    explicit ContainerScreenTickEvent(class ContainerScreenController* controller) : mController(controller) {}
};