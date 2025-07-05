#pragma once

#include "Memory.h"
#include <windows.h>
#include <Psapi.h>
#include <vector>
#include <thread>
#include <Prax/SDK/Game/Entity/Actor.h>



uintptr_t Memory::SearchAOB(const std::string& signature)
{
    hat::signature sigParsed = hat::parse_signature(signature).value();
    hat::signature_view sigView = hat::signature_view(sigParsed);
    hat::scan_result result = hat::find_pattern<hat::scan_alignment::X1>(sigView, hat::process::get_process_module());

    if (result.has_result()) return reinterpret_cast<uintptr_t>(result.get());
#ifdef __DEBUG__
    Logger::Write("Memory", "Failed to find signature: " + signature, Logger::LogType::Debug);
#endif
    return 0;
}

uintptr_t Memory::ResolveRef(uintptr_t ptr, int offset) {
    if (!ptr)
        return 0;

    uintptr_t newAddr = ptr + *reinterpret_cast<int*>(ptr + offset) + (offset + 4);
    return newAddr;
}

uintptr_t Memory::GetAddressByIndex(uintptr_t vtable, int index) {
    return (*reinterpret_cast<uintptr_t*>(vtable + 8 * index));
}

void Memory::WriteBytes(uintptr_t address, uint32_t *bytes, int length) {
    DWORD oldProtect;
    VirtualProtect((LPVOID)address, length, PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy((LPVOID)address, bytes, length);
    VirtualProtect((LPVOID)address, length, oldProtect, &oldProtect);
}

void Memory::WriteBytes(uintptr_t address, int32_t *bytes, int length, bool) {
    DWORD oldProtect;
    VirtualProtect((LPVOID)address, length, PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy((LPVOID)address, bytes, length);
    VirtualProtect((LPVOID)address, length, oldProtect, &oldProtect);
}



void Memory::WriteBytes(uintptr_t address, std::string bytes, int length){
    DWORD oldProtect;
    VirtualProtect((LPVOID)address, length, PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy((LPVOID)address, bytes.c_str(), length);
    VirtualProtect((LPVOID)address, length, oldProtect, &oldProtect);
}
// ReadBytes(uintptr_t address, int size) func
// example:
void Memory::ReadBytes(void* address, void* buffer, size_t size) {
    DWORD oldProtect;
    VirtualProtect(address, size, PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy(buffer, address, size);
    VirtualProtect(address, size, oldProtect, &oldProtect);
}

std::vector<uint8_t> Memory::ReadBytes(uintptr_t address, int size) {
    std::vector<uint8_t> buffer(size);
    ReadBytes((void*)address, buffer.data(), size);
    return buffer;
}

void Memory::WriteBytes(uintptr_t addr, const std::vector<unsigned char>& bytes, int length)
{
    DWORD oldProtect;
    VirtualProtect((LPVOID)addr, length, PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy((LPVOID)addr, bytes.data(), length);
    VirtualProtect((LPVOID)addr, length, oldProtect, &oldProtect);
}

int32_t Memory::GetRelativeAddress(uintptr_t address, uintptr_t target) {
    return (uint32_t)((uintptr_t)target - (uintptr_t)address - 4);
}

uintptr_t Memory::GetDirectAddress(uintptr_t intAddress) {
    return intAddress + *reinterpret_cast<int*>(intAddress - 4);
}

void Memory::SetProtection(uintptr_t addr, size_t size, DWORD protect) {
    DWORD oldProtect;
    VirtualProtect((LPVOID)addr, size, protect, &oldProtect);
}

uintptr_t Memory::ScanSig(const char* sig, int offset)
{
    auto parsed_sig = hat::parse_signature(sig);
    const auto result = hat::find_pattern(parsed_sig.value(), ".text");
    if (result.has_result())
        return reinterpret_cast<uintptr_t>(offset > 0 ? result.rel(offset) : result.get());
    else
        return 0;
}


