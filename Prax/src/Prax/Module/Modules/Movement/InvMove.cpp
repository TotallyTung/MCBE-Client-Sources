//
// Created by vastrakai on 2/9/2024.
//

#include "InvMove.h"

Vector2 getMovement() {
    Vector2 ret = Vector2(0, 0);
    float forward = 0.0f;
    float side = 0.0f;

    bool w = KeyManager::Keys['W'];
    bool a = KeyManager::Keys['A'];
    bool s = KeyManager::Keys['S'];
    bool d = KeyManager::Keys['D'];

    if (!w && !a && !s && !d)
        return ret;

    static constexpr float forwardF = 1;
    static constexpr float sideF = 0.7071067691f;

    if (w) {
        if (!a && !d)
            forward = forwardF;
        if (a) {
            forward = sideF;
            side = sideF;
        }
        else if (d) {
            forward = sideF;
            side = -sideF;
        }
    }
    else if (s) {
        if (!a && !d)
            forward = -forwardF;
        if (a) {
            forward = -sideF;
            side = sideF;
        }
        else if (d) {
            forward = -sideF;
            side = -sideF;
        }
    }
    else if (!w && !s) {
        if (!a && d) side = -forwardF;
        else side = forwardF;
    }

    ret.x = side;
    ret.y = forward;
    return ret;
}

void patchFunc(bool enable)
{
    static uintptr_t clearInputStateFunc = 0;

    if (clearInputStateFunc == 0) clearInputStateFunc = Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::PlayerMovement_clearInputState));
    if (clearInputStateFunc == 0) return;

    std::vector<uint8_t> ret = { 0xC3 };

    if (enable) ret = { 0xC3 };
    else ret = { 0x80 };

    Memory::WriteBytes(clearInputStateFunc, ret, 1);
}

void InvMove::OnEnable()
{
    patchFunc(true);
}

void InvMove::OnDisable()
{
    patchFunc(false);
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (player)
    {
        auto input = player->GetMoveInputComponent();
        input->moveLocked = false;
    }
}


void InvMove::OnTick()
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    auto input = player->GetMoveInputComponent();

    std::string screenName = Minecraft::ClientInstance->GetScreenName();
    if (screenName == "chat_screen")
    {
        input->forward = false;
        input->backward = false;
        input->left = false;
        input->right = false;
        if (!JumpOverride) input->isJumping = false;
        input->moveVector = Vector2(0.f, 0.f);
        input->moveLocked = false;
        input->isJumping2 = false;
        input->isSneakDown = false;
        patchFunc(false);
        return;
    } else patchFunc(true);

    bool w = KeyManager::Keys['W'];
    bool a = KeyManager::Keys['A'];
    bool s = KeyManager::Keys['S'];
    bool d = KeyManager::Keys['D'];
    bool space = KeyManager::Keys[VK_SPACE];

    bool shift = KeyManager::Keys[VK_SHIFT];
    bool pressed = w || a || s || d || space || shift;



    if (!pressed)
    {

        input->forward = false;
        input->backward = false;
        input->left = false;
        input->right = false;
        if (!JumpOverride) input->isJumping = false;
        input->moveVector = Vector2(0.f, 0.f);
        input->isJumping2 = false;
        input->isSneakDown = false;
        return;
    }

    if (screenName != "hud_screen")
    {
        input->moveLocked = true;
    }
    else input->moveLocked = false;

    input->forward = w;
    input->backward = s;
    input->left = a;
    input->right = d;
    if (!JumpOverride) input->isJumping = space;
    input->isJumping2 = space;
    input->isSneakDown = shift;

    input->moveVector = getMovement();
}
