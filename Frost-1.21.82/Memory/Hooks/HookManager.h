#pragma once

class FuncHook {
public:
	virtual bool Initialize() = 0;

	virtual ~FuncHook() = default;
};

// Actor
#include "Actor/ActorAnimationControllerHook.h"
#include "Actor/ActorRenderDispatcherHook.h"
#include "Actor/BaseTickHook.h"

// Game
#include "Game/ChatScreenControllerHook.h"
#include "Game/ContainerTickHook.h"
#include "Game/PacketReceiveHook.h"
#include "Game/PacketSendHook.h"
#include "Game/RakPeerHooks.h"

// Input
#include "Input/KeyboardHook.h"
#include "Input/MouseHook.h"

// Render
#include "Render/BobHurtHook.h"
#include "Render/SetupAndRenderHook.h"
#include "Render/SwingDurationHook.h"

namespace HookManager {
	static std::vector<std::unique_ptr<FuncHook>> hooks;

	void registerHook(std::unique_ptr<FuncHook> hook) {
		hooks.push_back(std::move(hook));
	}

	void ActivateHooks() {
		for (const auto& hook : hooks) {
			try {
				if (!hook->Initialize()) {
					std::string mFailedToInitialize = Util::combine("Failded to initialize hook: ", typeid(*hook).name());
					log(mFailedToInitialize.c_str());
				}
			}
			catch (const std::exception& e) {
				std::string mCatchedException = Util::combine("Exception during hooking process: ", e.what());
				log(mCatchedException.c_str());
			}
		}
	}

	void InitializeHooks() {
		// Dynamic Hooking
		HookManager::registerHook(std::make_unique<SetupAndRenderHook>());
		HookManager::registerHook(std::make_unique<KeyboardHook>());
		HookManager::registerHook(std::make_unique<MouseHook>());
		HookManager::registerHook(std::make_unique<BaseTickHook>());
		HookManager::registerHook(std::make_unique<SwingDurationHook>());
		HookManager::registerHook(std::make_unique<BobHurtHook>());
		HookManager::registerHook(std::make_unique<ActorRenderDispatcherHook>());
		HookManager::registerHook(std::make_unique<ActorAnimationControllerHook>());
		HookManager::registerHook(std::make_unique<PacketSendHook>());
		HookManager::registerHook(std::make_unique<PacketReceiveHook>());
		HookManager::registerHook(std::make_unique<RakPeerHooks>());
		HookManager::registerHook(std::make_unique<ChatScreenControllerHook>());
		HookManager::registerHook(std::make_unique<ContainerTickHook>());
		
		HookManager::ActivateHooks();
	}
}