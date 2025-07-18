#pragma once

class MovementUtils
{
public:
    static Vector2<float> getMoveVec()
    {
        auto player = Address::getLocalPlayer();

        Vector2<float> moveVec = { 0.0f, 0.0f };

        if (player->getMoveInputHandler()->mInputState.mUp)
            moveVec.x += 1.0f;

        if (player->getMoveInputHandler()->mInputState.mDown)
            moveVec.x -= 1.0f;

        if (player->getMoveInputHandler()->mInputState.mLeft)
            moveVec.y -= 1.0f;

        if (player->getMoveInputHandler()->mInputState.mRight)
            moveVec.y += 1.0f;

        return moveVec;
    }

    static void setSpeed(float speed) {
        auto player = Address::getLocalPlayer();
        if (!player)
            return;

        auto velocity = player->getStateVector();
        if (!velocity) {
            return;
        }

        float calcYaw = (player->getMovementInterpolator()->Rotations.y + 90) * (PI / 180);
        float c = cos(calcYaw);
        float s = sin(calcYaw);

        Vector2<float> moveVec = getMoveVec().normalize();
        moveVec = { moveVec.x * c - moveVec.y * s, moveVec.x * s + moveVec.y * c };

        Vector3<float> movement;
        movement.x = moveVec.x * speed;
        movement.y = velocity->Velocity.y;
        movement.z = moveVec.y * speed;

        StateVectorComponent* state = player->getStateVector();

        state->Velocity.z = movement.z;
        state->Velocity.x = movement.x;
    }

    static bool isMoving() {
        auto player = Address::getLocalPlayer();
        if (!player)
            return false;

        StateVectorComponent* velocity = player->getStateVector();
        if (!velocity && !player->getStateVector())
            return false;

        return (velocity->Velocity.magnitudexz() >= 0.045f);
    }
};