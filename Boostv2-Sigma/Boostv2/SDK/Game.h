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

class GameData {
public:
	ClientInstance* clientInstance;
	ClientInstance* getClientInstance() { return clientInstance; }
	void* keyMapPtr;
	void* keyMousePtr;
	int viewPerspectiveMode;

	bool isKeyDown(uint32_t key);
	bool isLeftClickDown();
	bool isRightClickDown();
	bool isWheelDown();
	void DisplayClientMessage(const char* fmt, ...);

	inline bool canUseMoveKeys() {
		return clientInstance->mcGame->mouseGrabbed;
	}

	inline GameMode* getGameMode() {
		return clientInstance->getLocalPlayer()->gameMode;
	}
	inline LocalPlayer* getLocalPlayer() {
		return clientInstance->getLocalPlayer();
	}
};
extern GameData Game;
