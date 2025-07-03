#pragma once
#include <string>
#include <string_view>
#include <cstdint>
#include <functional>

class HashedString {
public:
    uint64_t hash;
    std::string str;
    mutable HashedString const* lastMatch;

    constexpr static uint64_t computeHash(std::string_view str) {
        if (str.empty()) return 0;
        uint64_t hash = 0xCBF29CE484222325ULL;
        for (char s : str) {
            hash = s ^ (0x100000001B3ULL * hash);
        }
        return hash;
    }

    HashedString() : hash(0), str(""), lastMatch(nullptr) {}

    HashedString(std::string_view strView) {
        hash = computeHash(strView);
        str = strView;
        lastMatch = nullptr;
    }

    bool operator==(const HashedString& other) const noexcept {
        return hash == other.hash && str == other.str;
    }
};

namespace std {
    template <>
    struct hash<HashedString> {
        std::size_t operator()(const HashedString& h) const noexcept {
            return static_cast<std::size_t>(h.hash);
        }
    };
}