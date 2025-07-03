// Created by Tony on 2024-10-10 10:01:54

#include "../../Utils/MemoryUtils.h"
#include "../../Utils/Math.h"

/*ADD_OFFSET("MoveInputComponent::forward", 0x0A);
    ADD_OFFSET("MoveInputComponent::backward", 0x0B);
    ADD_OFFSET("MoveInputComponent::left", 0x0C);
    ADD_OFFSET("MoveInputComponent::right", 0x0D);
    ADD_OFFSET("MoveInputComponent::sneaking", 0x20);
    ADD_OFFSET("MoveInputComponent::jumping", 0x26);
    ADD_OFFSET("MoveInputComponent::sprinting", 0x27);*/
struct MoveInputComponent {
    // ? calculateMoveVector@PlayerMovement
    BUILD_ACCESS(this, bool, forward, 0x0A);
    BUILD_ACCESS(this, bool, backward, 0x0B);
    BUILD_ACCESS(this, bool, left, 0x0C);
    BUILD_ACCESS(this, bool, right, 0x0D);

    BUILD_ACCESS(this, bool, sneaking, 0x20); // ?setSneaking@Actor ?
    BUILD_ACCESS(this, bool, jumping, 0x26);
    BUILD_ACCESS(this, bool, sprinting, 0x27));

};