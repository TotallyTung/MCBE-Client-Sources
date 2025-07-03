//
// Created by vastrakai on 7/10/2024.
//

#pragma once

#include <Features/Events/Event.hpp>

class RunUpdateCycleEvent : public CancelableEvent {
public:
    bool mApplied = false;
};