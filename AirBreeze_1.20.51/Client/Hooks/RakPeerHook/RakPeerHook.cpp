#include "RakPeerHook.h"

#include "../../Manager/Module.h"
#include "../../Manager/Manager.h"

Manager* PeerMgr = nullptr;

typedef void(__thiscall* RakPeer) (__int64, __int64);
RakPeer RakPeerHookFC;

auto RakPeerHookCallback(__int64 _this, __int64 a1) -> __int64 {

	if (PeerMgr != nullptr) {

		auto instance = Minecraft::getClientInstance();
		auto mcgame = (instance != nullptr ? instance->MinecraftGame : nullptr);

		for (auto category : PeerMgr->categories) {

			for (auto mod : category->modules) {

				//if ((mod->name.rfind("NetSkip") != std::string::npos) && mod->isEnabled) {
					//return 0;
				//}

				if (PeerMgr->skipTicks) {
					return 0;
				}

			};

		};

	};

	RakPeerHookFC(_this, a1);

};

auto Hook_RakPeer::init(void) -> StatusData {

	PeerMgr = this->mgr;
	auto sig = Utils::findSig("48 8B C4 48 89 58 18 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 F8");

	if (!sig)
		return StatusData(MethodStatus::Error, "[Rakpeer Hook] Failed to find Signature!");

	if (MH_CreateHook((void*)sig, &RakPeerHookCallback, reinterpret_cast<LPVOID*>(&RakPeerHookFC)) != MH_OK)
		return StatusData(MethodStatus::Error, "[Rakpeer Hook] Failed to create hook!");

	if (MH_EnableHook((void*)sig) != MH_OK)
		return StatusData(MethodStatus::Error, "[Rakpeer Hook] Failed to enable hook!");

	return StatusData(MethodStatus::Success, "[Rakpeer Hook] Successfully hooked netskip/desync Hook!");

};