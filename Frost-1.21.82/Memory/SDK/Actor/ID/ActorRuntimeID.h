#pragma once

class ActorRuntimeID {
public:
    uint64_t id;

    [[nodiscard]] constexpr ActorRuntimeID() : id(0) {}
    [[nodiscard]] constexpr ActorRuntimeID(uint64_t x) : id(x) {} // NOLINT

    [[nodiscard]] constexpr operator uint64_t() const { return id; } // NOLINT
};

namespace std {
    template <>
    class hash<ActorRuntimeID> {
    public:
        size_t operator()(ActorRuntimeID const& id) const { return std::hash<int64_t>()(id.id); }
    };
}