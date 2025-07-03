// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../../../../Utils/MemoryUtils.h"
#include "../../../Client.h"

class KeyMapHook {
protected:
	using func_t = __int64(__fastcall*)(uint64_t, bool);
	static inline func_t func;

	static __int64 KeyPress(uint64_t key, bool isDown) {
		static ClickGui* clickGuiMod = (ClickGui*)client->moduleMgr->getModule("ClickGui");
		if (clickGuiMod->isEnabled()) {
			clickGuiMod->onKey((int)key, isDown);
			return 0;
		}

		if (mc.canUseMoveKeys() || mc.getLocalPlayer() == nullptr) client->moduleMgr->onKeyUpdate((int)key, isDown);
		return func(key, isDown);
	}
public:
	static void init() {
		uintptr_t address = findSig(Sigs::hook::KeyMapHook);
		MemoryUtils::CreateHook("KeyMapHook", address, (void*)&KeyMapHook::KeyPress, (void*)&func);
	}
};
