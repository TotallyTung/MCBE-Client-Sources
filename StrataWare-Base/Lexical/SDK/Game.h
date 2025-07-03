#pragma once
#include "../Utils/MemoryUtil.h"

#include "Core/ClientInstance.h"
#include "Network/MinecraftPackets.h"
#include "Network/PacketSender.h"
#include "World/Inventory/Transaction/ComplexInventoryTransaction.h"
#include "World/Inventory/Transaction/ItemUseInventoryTransaction.h"
#include "World/Inventory/Transaction/ItemUseOnActorInventoryTransaction.h"
#include "Render/MinecraftUIRenderContext.h"
#include "Render/ScreenView.h"
#include "MCTextFormat.h"

namespace Game {
	extern ClientInstance* clientInstance;
	inline ClientInstance* getClientInstance() { return clientInstance; }
	extern void* keyMapPtr;
	extern void* keyMousePtr;
	extern int viewPerspectiveMode;

	bool isKeyDown(uint32_t key);
	bool isLeftClickDown();
	bool isRightClickDown();
	void DisplayClientMessage(const char* fmt, ...);

	inline bool canUseMoveKeys() {
		return clientInstance->mcGame->mouseGrabbed;
	}

	inline LocalPlayer* ourPlayer() {
		return clientInstance->ourPlayer();
	}
}
