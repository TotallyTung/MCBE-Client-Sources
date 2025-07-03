#include "HookGameMode.h"

#include "../../Manager/Manager.h"
#include "../../Manager/Module.h"
#include "../../Manager/Category.h"
#include "../../Utils/Utils.h"
#include "HookGameMode.h"
#include "../../SDK/Classes/GameMode.h"


Manager* gmMgr = nullptr;



typedef void(__thiscall* tick)(GameMode* GM);
tick _tick;

Manager* g_manager = nullptr;

void tick_Callback(GameMode* GM) {

	if (gmMgr != nullptr) {
		for (auto category : gmMgr->categories) {
			for (auto mod : category->modules) {
				if (mod->isEnabled) {
					mod->onTick();
				}
			}
		}
	}

	_tick(GM);
}

auto Hook_GameMode::init(void) -> StatusData {


	gmMgr = this->mgr;
	auto sig = Utils::findSig("48 8B 89 ? ? ? ? 48 8B 01 48 8B 40 18 FF 15 ? ? ? ? 8B 43 24 89 43 20 48 83 C4 20 5B");

	if (!sig)
		return StatusData(MethodStatus::Error, "[GameMode Hook] Failed to find Signature!");

	if (MH_CreateHook((void*)sig, &tick_Callback, reinterpret_cast<LPVOID*>(&_tick)) != MH_OK)
		return StatusData(MethodStatus::Error, "[GameMode Hook] Failed to create hook!");

	if (MH_EnableHook((void*)sig) != MH_OK)
		return StatusData(MethodStatus::Error, "[GameMode Hook] Failed to enable hook!");


	return StatusData(MethodStatus::Success, "[GameMode Hook] Successfully hooked!");



};