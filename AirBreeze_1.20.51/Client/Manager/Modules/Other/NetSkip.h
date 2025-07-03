#pragma once

#include "../../Module.h"
#include "../../Manager.h"
#include "../../Client.h"
#include "../../../Include/imgui/imgui.h"
#include "../../../Utils/Render/ImGuiUtils.h"
#include "../../../Include/imgui/imgui_notify.h"

Manager* NetSkipMgr = nullptr;

class NetSkip : public Module {
public:
    NetSkip(Category* c) : Module(c, "NetSkip", "Testing RakNet::RakPeer::RunUpdateCycle(RakNet::BitStream &).") {}

	auto onEnable() -> void {
		NetSkipMgr->skipTicks = true;
	}


    auto onTick() -> void {
		auto i = Minecraft::getClientInstance();
		if (i == nullptr) return;

		auto player = i->LocalPlayer;
		if (player == nullptr) return;

		static auto lastTime = std::chrono::high_resolution_clock::now();
		auto currentTime = std::chrono::high_resolution_clock::now();
		if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count() < 500) { // 500 is the delay value
			NetSkipMgr->skipTicks = true;
		}
		else {
			NetSkipMgr->skipTicks = false;
			lastTime = std::chrono::high_resolution_clock::now();
		}
    }

	auto onDisable() -> void {
		NetSkipMgr->skipTicks = false;
	}
};
