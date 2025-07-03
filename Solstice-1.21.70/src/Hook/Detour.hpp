#pragma once
//
// Created by vastrakai on 6/24/2024.
//


#include <string>


class Detour {
public:
    void* mFunc{};
    void* mOriginalFunc{};

    std::string mName;

    ~Detour();
    Detour(const std::string& name, void* addr, void* detour, bool silent = false);

    void enable(bool silent = false) const;
    void restore() const; // provided just in case, you should NOT use this

    template <auto T>
    auto getOriginal() {
        return reinterpret_cast<decltype(T)>(mOriginalFunc);
    }

};
