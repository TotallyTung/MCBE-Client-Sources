#pragma once
#include "../../../../Utils/MemoryUtils.h"
#include "../../../../Utils/Math.h"
#include "../../../../SDK/Classes/GameMode.h"

#include "../../../Client.h"

class GameModeStartDestroyBlock {
protected:
	using func_t = bool(__fastcall*)(GameMode*, Vec3<int>*, uint8_t, bool*);
	static inline func_t oFunc;

	static bool GameMode_startDestroyBlock(GameMode* _this, Vec3<int>* blockPos, uint8_t face, bool* a4) {
		bool result = oFunc(_this, blockPos, face, a4);
		static PacketMine* packetMineMod = (PacketMine*)client->moduleMgr->getModule("PacketMine");
		static Regen* RegenMod = (Regen*)client->moduleMgr->getModule("Regen");
		if (packetMineMod->isEnabled() && result) {
			packetMineMod->setBreakPos(*blockPos, face);
			return false;
		}
		if (RegenMod->isEnabled() && result) {
			RegenMod->setBreakPos(*blockPos, face);
			return false;
		}
		
		return result;
	}
public:
	static void init(uintptr_t address) {
		MemoryUtils::CreateHook("GameMode_startDestroyBlockHook", address, (void*)&GameModeStartDestroyBlock::GameMode_startDestroyBlock, (void*)&oFunc);
	}
};