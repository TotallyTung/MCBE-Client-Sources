#pragma once

namespace mce {
	class UUID {
    public:
        uint64_t mostSigs, leastSigs; // a, b

        [[nodiscard]] constexpr UUID(uint64_t a = 0, uint64_t b = 0) : mostSigs(a), leastSigs(b) {}

        [[nodiscard]] inline UUID(std::string const& uuidStr) : UUID(fromString(uuidStr)) {} // NOLINT

        [[nodiscard]] inline UUID(std::string_view uuidStr) : UUID(fromString(std::string{ uuidStr })) {} // NOLINT

        //LLNDAPI static mce::UUID random();

        /* [[nodiscard]] inline mce::UUID fromStringHash(std::string_view sv) {
            return { ll::hash_utils::doHash(sv), ll::hash_utils::doHash2(sv) };
        } */

        [[nodiscard]] inline explicit operator bool() const { return !isEmpty(); }

        [[nodiscard]] inline explicit operator std::string() const { return asString(); }

        [[nodiscard]] constexpr bool operator==(UUID const& other) const { return (mostSigs == other.mostSigs) && (leastSigs == other.leastSigs); }

        [[nodiscard]] constexpr std::strong_ordering operator<=>(UUID const& other) const {
            if (mostSigs != other.mostSigs) {
                return mostSigs <=> other.mostSigs;
            }
            return leastSigs <=> other.leastSigs;
        }

    public:
        // NOLINTBEGIN
        std::string asString() const;

        bool isEmpty() const;

        bool operator<(class mce::UUID const&) const;

        static bool canParse(std::string const& in);

        static class mce::UUID fromString(std::string const& in);

        static class mce::UUID EMPTY;

        static uint64_t STRING_LENGTH;

        // NOLINTEND
    };
};