#pragma once
#include "../../../../Utils/MemoryUtils.h"
#include "../../../../Utils/Math.h"
#include "../../../../SDK/Classes/GameMode.h"

#include "../../../Client.h"

class GameModeStopDestroyBlock {
protected:
	using func_t = void(__fastcall*)(GameMode*, Vec3<int>*);
	static inline func_t oFunc;

	static void GameMode_stopDestroyBlock(GameMode* _this, Vec3<int>* blockPos) {
		static PacketMine* packetMineMod = (PacketMine*)client->moduleMgr->getModule("PacketMine");
		static Regen* RegenMod = (Regen*)client->moduleMgr->getModule("Regen");
		if (packetMineMod->isEnabled()) return;
		if (RegenMod->isEnabled()) return;
		oFunc(_this, blockPos);
	}
public:
	static void init(uintptr_t address) {
		MemoryUtils::CreateHook("GameMode_stopDestroyBlockHook", address, (void*)&GameModeStopDestroyBlock::GameMode_stopDestroyBlock, (void*)&oFunc);
	}
};