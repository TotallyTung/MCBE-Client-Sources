#pragma once

void* onContainerTick;

void* ContainerTickDetour(ContainerScreenController* _this) {
    ContainerTickEvent event{ _this };
    EventManager::registerEvent(&event);

    return Memory::CallFunc<void*, ContainerScreenController*>(onContainerTick, _this);
}

class ContainerTickHook : public FuncHook
{
public:
    bool Initialize() override
    {
        Memory::HookFunction(SigManager::ContainerScreenController_tick, (void*)&ContainerTickDetour, &onContainerTick, "ContainerScreenController::tick");

        return true;
    }
};