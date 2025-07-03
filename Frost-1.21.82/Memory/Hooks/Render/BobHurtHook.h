#pragma once

void* onBobHurt;

void BobHurtDetour(uint64_t _this, glm::mat4* matrix)
{
	BobHurtEvent event{ matrix };
	EventManager::registerEvent(&event);

	return Memory::CallFunc<void, uint64_t, glm::mat4*>(
		onBobHurt, _this, matrix);
}

class BobHurtHook : public FuncHook 
{
public:
	bool Initialize() override
	{
		Memory::HookFunction(SigManager::BobHurt, (void*)&BobHurtDetour, &onBobHurt, "BobHurt");

		return true;
	}
};