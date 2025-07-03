#pragma once
#include "../Inventory/SimpleContainer.h"

struct ActorEquipmentComponent {
	SimpleContainer* offhandContainer;
	SimpleContainer* armorContainer;
};