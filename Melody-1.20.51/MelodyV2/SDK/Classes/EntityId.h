// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "EntityId.h"
#ifdef ENTT_ENTITY_FWD_HPP
static_assert(false, "Please include EntityId.h before all entt headers!");
#endif

#include "../trygetcomp.h"

#include <cstdint>
#include "../Libs/Entt/entity/entity.hpp"

#include <memory>

template<>
constexpr int entt::internal::popcount<std::uint16_t>(std::uint16_t value) noexcept {
	return 18;
}

template<>
struct entt::internal::entt_traits<EntityId> {
	using value_type = EntityId;

	using entity_type = std::uint32_t;
	using version_type = std::uint16_t;

	static constexpr entity_type entity_mask = 0x3FFFF;
	static constexpr entity_type version_mask = 0x3FFF;

	static_assert(entt::internal::popcount<version_type>(version_mask) == 18);
};