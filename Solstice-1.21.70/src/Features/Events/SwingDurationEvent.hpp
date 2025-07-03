//
// Created by vastrakai on 7/18/2024.
//

#pragma once

#include "Event.hpp"

class SwingDurationEvent : public Event {
public:
    int mSwingDuration;

    explicit SwingDurationEvent(int swingDuration) : mSwingDuration(swingDuration) {}
};
