//
// Created by vastrakai on 7/19/2024.
//

#pragma once

#include "Event.hpp"


class BobHurtEvent : public Event {
public:
    BobHurtEvent(void* _this, glm::mat4* matrix) : Event(), mThis(_this), mMatrix(matrix) {

    }

    void* mThis;
    glm::mat4* mMatrix;
    bool mDoBlockAnimation = false;
};