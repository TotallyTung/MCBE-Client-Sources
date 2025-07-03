#include "../../../Client/Modules/Visual/Animations.hpp"

#pragma once

void* onCurrentSwingDuration;

int getSwingDurationDetour(Mob* _this)
{
	if (getModuleByType<Animations>()->isEnabled()) 
	{
		return getModuleByType<Animations>()->mSwingSpeed;
	}

	return 6;

	Memory::CallFunc<void*, Mob*>(
		onCurrentSwingDuration, _this);
}

class SwingDurationHook : public FuncHook 
{
public:
	bool Initialize() override
	{
		Memory::HookFunction(SigManager::Mob_getCurrentSwingDuration, (void*)&getSwingDurationDetour, &onCurrentSwingDuration, "Mob::getCurrentSwingDuration");

		return true;
	}
};