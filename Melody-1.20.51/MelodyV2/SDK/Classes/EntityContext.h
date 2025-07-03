// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "EntityId.h"
#include "../trygetcomp.h"
#include "EntityRegistry.h"
#include "WeakPtr.h"

#undef max
#include "../Libs/Entt/entity/registry.hpp"

#include <cstdint>
#include <optional>

#include "../trygetcomp.h"
#include "../trygetcomp.h"

class EntityContextBase
{
public:
	EntityRegistry* mRegistry = nullptr;
	entt::basic_registry<EntityId>* mEnttRegistry = nullptr;
	EntityId id{};
public:
	[[nodiscard]] bool empty() const
	{
		return this->mRegistry == nullptr || this->mEnttRegistry == nullptr || this->id == entt::tombstone || this->id == entt::null;
	}

	template<typename component_t>
	[[nodiscard]] bool hasComponent() const
	{
		auto& storage = Bridge::assure_for<component_t>(&this->mRegistry->mOwnedRegistry);
		return storage.contains(this->id);
	}

	template<typename component_t>
	component_t* tryGetComponent()
	{
		auto& storage = Bridge::assure_for<component_t>(&this->mRegistry->mOwnedRegistry);
		if (storage.contains(this->id))
		{
			return std::addressof(storage.get(this->id));
		}
		return nullptr;
	}

	template<typename component_t, typename... args_t>
	auto getOrAddComponent(args_t... args)
	{
		if (auto& cpool = Bridge::assure_for<component_t>(&this->mRegistry->mOwnedRegistry); cpool.contains(this->id)) {
			return cpool.get(this->id);
		}
		else {
			return cpool.emplace(this->id, std::forward<args_t>(args)...);
		}
	}

	template<typename component_t>
	void removeComponent()
	{
		auto& storage = Bridge::assure_for<component_t>(&this->mRegistry->mOwnedRegistry);
		storage.remove(this->id);
	}
};

class EntityContext : public EntityContextBase
{
public:
}; //Size: 0x0C
//char(*__kaboom)[sizeof(EntityContext)] = 1;
//static_assert(sizeof(EntityContext) == 0x18);
//static_assert(sizeof(std::optional<EntityContext>) == 0x20);