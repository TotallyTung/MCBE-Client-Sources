//
// Created by vastrakai on 8/9/2024.
//

#pragma once

#include "Event.hpp"

class SendImmediateEvent : public CancelableEvent {
public:
    uint64_t a1;
    char* send;
    uint64_t a2;
    uint64_t a3;
    uint64_t a4;
    uint64_t a5;
    uint64_t a6;
    uint64_t a7;
    uint64_t a8;
    uint64_t a9;
    uint64_t a10;
    bool mModified = false;

    explicit SendImmediateEvent(uint64_t a1, char* send, uint64_t a2, uint64_t a3, uint64_t a4, uint64_t a5, uint64_t a6, uint64_t a7, uint64_t a8, uint64_t a9, uint64_t a10) : a1(a1), send(send), a2(a2), a3(a3), a4(a4), a5(a5), a6(a6), a7(a7), a8(a8), a9(a9), a10(a10) {}
};