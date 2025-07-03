#pragma once
#include <string>
#include <memory>

class HashedString {
public:
	uint64_t hash;
	std::string str;
	mutable HashedString const* lastMatch;

    [[nodiscard]] constexpr static uint64_t computeHash(std::string_view str) {
        if (str.empty()) return 0;
        uint64_t hash = 0xCBF29CE484222325ULL;
        for (char s : str) {
            hash = s ^ (0x100000001B3ULL * hash);
        }
        return hash;
    }

    HashedString(std::string_view strView) {
        hash = computeHash(strView);
        str = strView;
        lastMatch = nullptr;
    }
};