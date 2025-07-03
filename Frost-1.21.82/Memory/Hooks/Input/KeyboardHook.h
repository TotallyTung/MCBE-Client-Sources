#pragma once

void* onKeyboardDevice;

void KeyboardDetour(uint64_t key, bool held)
{
	GlobalVariables::mKey[key] = held;

	bool mCancelled = false;
	KeyboardEvent event{ &key, &held };
	event.mCancelled = &mCancelled;
	EventManager::registerEvent(&event);

	for (const auto& mod : mModules)
	{
		if (held == true) 
		{
			if (mod->getKeybind() == key) 
			{
				if (mod->mToggleInGameOnly)
				{
					if (InstanceManager::isAllowedToUseKeys())
					{
						mod->toggle();
					}
				}
				else 
				{
					mod->toggle();
				}
			}
		}
	}

	if (mCancelled) return;

	Memory::CallFunc<void*, __int32, bool>(
		onKeyboardDevice, key, held);
}

class KeyboardHook : public FuncHook 
{
public:
	bool Initialize() override
	{
		Memory::HookFunction(SigManager::Keyboard_feed, (void*)&KeyboardDetour, &onKeyboardDevice, "Keyboard::feed");

		return true;
	}
};