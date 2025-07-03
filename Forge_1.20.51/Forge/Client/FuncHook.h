#pragma once
#include "SlimMem.h"

class FuncHook {
public:
	virtual bool Initialize() = 0;
};

// include hooks here
#include "./Hooks/KeymapHook.h"
#include "./Hooks/RakPeerHook.h"
#include "./Hooks/PacketHook.h"
#include "./Hooks/SendChatMessage.h"
#include "./Hooks/RenderContextHook.h"
#include "./Hooks/UISceneHook.h"
#include "./Hooks/OpenURLHook.h"
#include "./Hooks/SwingHook.h"
#include "./Hooks/UIControl.h"
#include "./Hooks/onUpdateTickHook.h"

void InitHooks() {
	// initialize hooks here
	static FuncHook* hooks[] = {
		// include hooks here
		&KeymapHook::Get(),
		//&PacketObserverHook::Get(),
		//&RakPeerHooks::Get(),
		&SendChatMessage::Get(),
		&RenderContextHook::Get(),
		&UISceneHook::Get(),
		&OpenURLHook::Get(),
		&ActorHook::Instance(),
		&UIControlHook::Instance(),
		&onUpdateHook::Instance()
	};

	for (std::size_t i = 0; i < std::size(hooks); ++i)
	{
		if (not hooks[i]->Initialize())
		{
			//error handling
		}
		Game::initProgress = (i + 1.f) / std::size(hooks) * 100.f;
		Sleep(20);
		karnelLogF("Progress: %d%%\n", (int)std::floor(Game::initProgress));
		Sleep(20);
	}
}

void InitGameData() {
	DWORD procId = GetCurrentProcessId();
	if (!Game::Core::slimMem.Open(procId, SlimUtils::ProcessAccess::Full)) {
		logF("Failed to open process, error-code: %i\n", GetLastError());
	}
	Game::Core::gameModule = Game::Core::slimMem.GetModule(L"Minecraft.Windows.exe");
	Game::Core::clientInstance = reinterpret_cast<ClientInstance*>(Game::Core::slimMem.ReadPtr<uintptr_t*>(
		Game::Core::gameModule->ptrBase + 0x057A8150, { 0x0,0x0,0x60,0x10 }
	));

	logF("[ %sGame%s ] ClientInstance: %llX\n", GOLD, WHITE, Game::GetInstance());
}