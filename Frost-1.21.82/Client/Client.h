#pragma once

class Client : public MainClient {
public:
	Client() {}
	
	static void Initialize() 
	{
		static bool mSentInjectText = false;

		Console::CreateConsole("Frost [developer]");
		FileUtil::InitializeClientFolders();
		SigManager::initialize();
		OffsetManager::initialize();
		ModuleManager::InitializeModules();
		HookManager::InitializeHooks();
		DirectXHook::StartUp();
		mCommandMgr.InitializeCommands();
		Graphics::Utils::printInformation();

		if (InstanceManager::get<ClientInstance>()->getLocalPlayer() != nullptr && mSentInjectText == false) {
			std::string mInjectMessage = "Injection successful! Let the fun begin!";

			ChatUtil::sendMessage(mInjectMessage);
			mSentInjectText = true;
		}
	}

	static void Shutdown()
	{
		kiero::shutdown();
		MH_DisableHook(MH_ALL_HOOKS);
		MH_RemoveHook(MH_ALL_HOOKS);
		ModuleManager::UninitializeMods();
		OffsetManager::deinitialize();
		SigManager::deinitialize();
	}
};