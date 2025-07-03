// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../../../../SDK/Classes/ContainerScreenControllerTickEventContainerScreenControllerTickEvent.h"
#include "../../../../SDK/Classes/Actor.h"
#include "../../../../Utils/MemoryUtils.h"
#include "../../../../SDK/Classes/Options.h"

#include "../../../Client.h"

class onContainerScreenControllerTickEventHook {
protected:
	using func_t = void(__fastcall*)(ContainerScreenControllerTickEvent*);
	static inline func_t oFunc;

	static void onContainerScreenControllerTickEvent_(ContainerScreenControllerTickEvent* event) {
		

		client->moduleMgr->onContainerScreenControllerTickEvent(event);
		oFunc(event);
	}
public:
	static void init() {
		uintptr_t address = findSig(Sigs::ContainerScreenController::ContainerScreenController_tick);
		MemoryUtils::CreateHook("onContainerScreenControllerTickEventHook", address, (void*)&onContainerScreenControllerTickEventHook::onContainerScreenControllerTickEvent_, (void*)&oFunc);
	}
};