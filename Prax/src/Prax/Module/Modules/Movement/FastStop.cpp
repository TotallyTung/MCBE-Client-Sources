

#include <Prax/Utils/Math.h>
#include "FastStop.h"


bool LastKeyState = false;
void FastStop::OnTick() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    bool keyState = Misc::IsUsingMovementKeys();
    if (!keyState && LastKeyState) {
        player->GetStateVectorComponent()->velocity = Vector3(0.f,player->GetStateVectorComponent()->velocity.y,0.f);
    }

    LastKeyState = keyState;
}
