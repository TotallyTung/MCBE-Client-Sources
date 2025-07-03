#pragma once

struct Tick {
public:
    using Type = uint64_t;

    Type t;

    [[nodiscard]] constexpr operator Type() const { return t; } // NOLINT

    [[nodiscard]] constexpr Tick(Type v) : t(v) {}; // NOLINT

    [[nodiscard]] constexpr Tick(Tick const& v) = default;

    [[nodiscard]] constexpr Tick() : t(0) {};
};