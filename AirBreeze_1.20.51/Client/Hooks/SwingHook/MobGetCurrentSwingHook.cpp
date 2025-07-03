#include "MobGetCurrentSwingHook.h"

#include "../../Manager/Module.h"
#include "../../Manager/Manager.h"

Manager* SpeedSwingMgr = nullptr;

typedef void(__thiscall* SpeedSwing) (__int64, __int64);
SpeedSwing MobGetCurrentSwingHookFC;

auto MobGetCurrentSwingHookCallback(Actor* _this) -> __int64 {

	if (SpeedSwingMgr != nullptr) {

		auto instance = Minecraft::getClientInstance();
		auto mcgame = (instance != nullptr ? instance->MinecraftGame : nullptr);

		for (auto category : SpeedSwingMgr->categories) {

			for (auto mod : category->modules) {

				if ((mod->name.rfind("SlowSwing") != std::string::npos) && mod->isEnabled) {
					return 15;
				}
			};

		};

	};

	MobGetCurrentSwingHookFC;

};

auto Hook_MobGetCurrentSwing::init(void) -> StatusData {

	SpeedSwingMgr = this->mgr;
	auto sig = Utils::findSig("48 89 5C 24 ? 57 48 83 EC 20 48 8B 15");

	if (!sig)
		return StatusData(MethodStatus::Error, "[MobGetCurrentSwing Hook] Failed to find Signature!");

	if (MH_CreateHook((void*)sig, &MobGetCurrentSwingHookCallback, reinterpret_cast<LPVOID*>(&MobGetCurrentSwingHookFC)) != MH_OK)
		return StatusData(MethodStatus::Error, "[MobGetCurrentSwing Hook] Failed to create hook!");

	if (MH_EnableHook((void*)sig) != MH_OK)
		return StatusData(MethodStatus::Error, "[MobGetCurrentSwing Hook] Failed to enable hook!");

	return StatusData(MethodStatus::Success, "[MobGetCurrentSwing Hook] Successfully hooked netskip/desync Hook!");

};