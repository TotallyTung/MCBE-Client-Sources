//
// Created by vastrakai on 1/7/2024.
//

#include "SafeWalk.h"

uintptr_t SafeWalk::SafeWalkAddress = 0;
std::vector<char> SafeWalk::NewBytes = {(char) 0x90, (char) 0x90};
std::vector<char> SafeWalk::OgBytes = {(char) 0x32, (char) 0xC0};

void SafeWalk::OnTick()
{
    if (!SafeWalkAddress) return;
    Memory::WriteBytes(SafeWalkAddress, NewBytes.data(), NewBytes.size());
}

void SafeWalk::OnDisable()
{
    if (!SafeWalkAddress) return;
    Memory::WriteBytes(SafeWalkAddress, OgBytes.data(), OgBytes.size());
}

void SafeWalk::OnEnable()
{
    if (!SafeWalkAddress) return;
    Memory::WriteBytes(SafeWalkAddress, NewBytes.data(), NewBytes.size());
}

void SafeWalk::OnInitialize()
{
    if (!SafeWalkAddress) SafeWalkAddress = SigManager::GetSignatureAddress(SignatureEnum::SneakMovementSystem_tickSneakMovementSystem);
}

void SafeWalk::OnEject()
{
    if (!SafeWalkAddress) return;
    Memory::WriteBytes(SafeWalkAddress, OgBytes.data(), OgBytes.size());
}
