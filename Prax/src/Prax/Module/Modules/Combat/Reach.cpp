//
// Created by vastrakai on 1/9/2024.
//

#include "Reach.h"

float* Reach::ReachPtr = nullptr;

void Reach::OnInitialize()
{
    uintptr_t addr = SigManager::GetSignatureAddress(SignatureEnum::Reach);
    addr += 4;
    // Read this addr as an int, then add it to the addr of the next instruction
    // This is the addr of the float that stores the reach
    int offset = *reinterpret_cast<int*>(addr);
    ReachPtr = reinterpret_cast<float*>(addr + offset + 4);
    //Logger::Write("Reach", "ReachPtr: " + String::ToHex(reinterpret_cast<uintptr_t>(ReachPtr)));
}

void writeToProtectedAddr(float* addr, float value)
{
    DWORD oldProtect;
    VirtualProtect(addr, sizeof(float), PAGE_EXECUTE_READWRITE, &oldProtect);
    *addr = value;
    VirtualProtect(addr, sizeof(float), oldProtect, &oldProtect);
}

void Reach::OnTick()
{
    if (CombatReachEnabled.Enabled)
    {
        writeToProtectedAddr(ReachPtr, CombatReach.Value);
    } else
    {
        writeToProtectedAddr(ReachPtr, 3.f);
    }
}

void Reach::OnEnable()
{
    if (CombatReachEnabled.Enabled)
    {
        writeToProtectedAddr(ReachPtr, CombatReach.Value);
    } else
    {
        writeToProtectedAddr(ReachPtr, 3.f);
    }
}

void Reach::OnDisable()
{
    writeToProtectedAddr(ReachPtr, 3.f);
}
