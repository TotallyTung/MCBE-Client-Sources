#pragma once
#include <string_view>

#include "Event.hpp"
//
// Created by vastrakai on 6/29/2024.
//


class ChatEvent : public CancelableEvent {
public:
    std::string mMessage;
    bool mSpecial = false;

    explicit ChatEvent(const std::string_view message) {
        this->mMessage = message;
    }

    [[nodiscard]] const std::string& getMessage() const {
        return this->mMessage;
    }

    void setMessage(const std::string_view msg) {
        this->mMessage = msg;
    }
};