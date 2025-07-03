// Created by Tony on 2024-10-10 10:01:54

#include "ChestStealler.h"

ChestStealler::ChestStealler() : Module("ChestStealler", "Steals items from a chest", Category::PLAYER) {
    addSlider<int>("Delay", "Ticks between each automatic inventory action.", ValueType::INT_T, &Delay, 0, 100);
}

static int ticksEnabled = 0;

void ChestStealler::onContainerScreenControllerTickEvent(ContainerScreenControllerTickEvent& event) {
    auto* player = mc.getLocalPlayer();
    int idelay = Delay;

    if (idelay != 0 && ticksEnabled % idelay != 0) return;

    auto wpManager = player->mContainerManager;
    auto manager = wpManager.lock();

    if (!manager) return;

    ContainerType type = manager->getContainerType();

    if (type == ContainerType::CHEST) {
        ContainerScreenController* controller = static_cast<ContainerScreenController*>(event.getController());

        for (int currentSlot = 0; currentSlot < 9 * 3; currentSlot++) {
            int8_t containerId = manager->getContainerId();
            ItemStack& currentStack = manager->getSlot(currentSlot);

            if (!currentStack.item) {
                continue;
            }

            const int emptySlot = player->getPlayerInventory()->inventory->getEmptySlotFromContainer();

            if (emptySlot == -1) {
                // No empty slots available
                break;
            }

            controller->_handleAutoPlace(ItemSource::container_items, currentSlot);
            break;
        }
    }
}
