#pragma once

#include "../../Utils/Math.h"
#include "../Classes/ActorRotate.h"

struct ActorHeadRotationComponent {
public:
    float headRot;
    float oldHeadRot;

    ActorHeadRotationComponent(float initialRot = 0.0f)
        : headRot(initialRot), oldHeadRot(initialRot) {
    }

    [[nodiscard]] float GetHeadRotation() const {
        return headRot;
    }

    void SetHeadRotation(float newRot) {
        oldHeadRot = headRot;
        headRot = newRot;
    }

    [[nodiscard]] float GetOldHeadRotation() const {
        return oldHeadRot;
    }


    void ApplyRotationEvent(const ActorRotateEvent& event) {
        oldHeadRot = headRot;
        headRot = event.getPitch();
    }
};
