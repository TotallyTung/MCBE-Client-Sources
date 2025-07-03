#pragma once

#include <vector>

#include "../SDK/Entity.h"

class Target {
public:
	static bool containsOnlyASCII(const std::string& string);
	static bool isValidTarget(Entity* ent);
};
