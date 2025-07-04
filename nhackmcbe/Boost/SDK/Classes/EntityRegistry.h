#pragma once
#include "EntityId.h"
#include "../trygetcomp.h"
#include "../Libs/Entt/entt.hpp"
enum class EntityId : std::uint32_t;

class EntityRegistryBase
{
public:
	entt::basic_registry<EntityId>* mEnttRegistry = nullptr;
	EntityRegistryBase* mRegistry = nullptr;
};

class EntityRegistry : public EntityRegistryBase {
public:
	std::string mName;
	entt::basic_registry<EntityId> mOwnedRegistry;
};