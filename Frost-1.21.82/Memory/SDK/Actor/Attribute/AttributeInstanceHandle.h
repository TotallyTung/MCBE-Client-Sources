#pragma once

class BaseAttributeMap;

class AttributeInstanceHandle {
public:
	uint32_t          mAttributeID;  // 0x0
	BaseAttributeMap* mAttributeMap; // 0x8
};
static_assert(sizeof(AttributeInstanceHandle) == 0x10);