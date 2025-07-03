// Created by Tony on 2024-10-10 10:01:54

#pragma once

#include "../Utils/MemoryUtils.h"
#include "Classes/EntityId.h"
#include "../Libs/Entt/entity/registry.hpp"
//#include <entt/entity/registry.hpp>

#include <iomanip>
#include <iostream>
#include <sstream>

#include "Classes/EntityId.h"
#include "Classes/EntityContext.h"

namespace Bridge
{
	
	template<typename component_t>
	auto& assure_for(entt::basic_registry<EntityId>* registry)
	{
		return registry->storage<component_t>();
		/*using assure_t = entt::basic_storage<component_t, EntityId>* (__fastcall *)(const entt::basic_registry<EntityId>*, uint32_t);
		static auto assureFunc = reinterpret_cast<assure_t>(resolveAssureFunc<component_t>());

		return assureFunc(registry, entt::type_hash<component_t>::value());*/
	}
}
