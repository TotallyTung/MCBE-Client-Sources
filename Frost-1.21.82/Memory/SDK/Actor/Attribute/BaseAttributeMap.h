#pragma once

#include <unordered_map>

class BaseAttributeMap
{
public:
    std::unordered_map<int, AttributeInstance> mAttributes;
    std::vector<uint64_t> mDirtyAttributes;
};

//static_assert(sizeof(BaseAttributeMap) == 0x58);