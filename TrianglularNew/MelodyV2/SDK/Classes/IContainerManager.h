#pragma once

#include "Container.h"
#include "ItemStack.h"


#include <stdint.h>
#include <vector>

class IContainerManager
{
public:
	virtual ~IContainerManager();
	virtual int8_t getContainerId() const;
	virtual void setContainerId(int8_t containerId);
	virtual ContainerType getContainerType() const;
	virtual void setContainerType(ContainerType containerType);
	virtual void serverInitItemStackIds(void);
	virtual std::vector<ItemStack> getItemCopies() const;
	virtual void setSlot(int, const ItemStack&, bool);
	virtual ItemStack& getSlot(int);
	virtual void broadcastChanges();
};