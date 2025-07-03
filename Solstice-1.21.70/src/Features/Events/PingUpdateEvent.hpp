//
// Created by vastrakai on 7/10/2024.
//

#pragma once

#include <Features/Events/Event.hpp>

class PingUpdateEvent : public Event {
public:
    __int64 mPing;

    explicit PingUpdateEvent(__int64 ping) : mPing(ping) {}
};
