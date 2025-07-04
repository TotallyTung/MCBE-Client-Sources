/*#pragma once
#include "../../../../Utils/MemoryUtils.h"

#include "../../../Client.h"

class ItemUseSlowdownHook {
protected:
	using func_t = void(__fastcall*)(void* dont_care, StrictEntityContext* context, void* lol, void* lol2);
	static inline func_t func;

	static void itemUseSlowdownCallback_1_20_51_1(void* dont_care, StrictEntityContext* context, void* lol, void* lol2) {
		auto* ci = Bridge::getClientInstance();
		auto* player = ci->getClientPlayer();
		if (context->entity == player->getEntityId())
		{
			auto event = nes::make_holder<PlayerSlowedByItemEvent>(player);

			auto dispatcher = systems->getEventDispatcher();
			dispatcher->trigger(event);

			if (event->isCancelled()) {
				return;
			}



			if (_this == mc.getLocalPlayer()) client->moduleMgr->onNormalTick(_this);
			func(_this);
		}
	}
public:
	static void init() {
		uintptr_t address = findSig(Sigs::hook::ItemUseSlowdownHook);
		MemoryUtils::CreateHook("ItemUseSlowdownHook", address, (void*)&ItemUseSlowdownHook::itemUseSlowdownCallback_1_20_51_1, (void*)&func);
	}
};
*/