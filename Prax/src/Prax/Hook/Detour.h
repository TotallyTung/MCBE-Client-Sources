#pragma once

#include <string>

class Detour {
public:
    void* FuncPtr;
    void* OFunc{};

    std::string Name;

    ~Detour();
    Detour(const std::string& name, void* addr, void* detour);
    Detour(const std::string& name, void* addr, void* detour, bool silent);
    Detour(std::string name, uintptr_t addr, void* detour);
    Detour(std::string name, uintptr_t** addr, void* detour);
    void Enable(bool silent = false) const;
    void Restore() const;

    template <typename TRet, typename... TArgs>
    inline auto* GetFastcall() {
        using Fn = TRet(__fastcall*)(TArgs...);
        return reinterpret_cast<Fn>(OFunc);
    }
};