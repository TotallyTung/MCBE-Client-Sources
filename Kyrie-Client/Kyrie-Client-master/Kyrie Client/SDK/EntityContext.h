#pragma once
#include <memory>

struct EntityId {
	uint32_t id;
};

class EntityRegistry;

class EntityContext {
public:
	EntityRegistry* registry;
	EntityId id;
};
