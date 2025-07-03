#pragma once

void* onAddStaticScreenVars;
void* onChatScreenControllerTick;

void* ChatScreenControllerTickDetour(ChatScreenController* _this) {

    return Memory::CallFunc<void*, ChatScreenController*>(
        onChatScreenControllerTick, _this);
}

void* AddStaticScreenVarsDetour(ChatScreenController* _this, Json::Value* globalVars) {
    auto VTable = *(uintptr_t**)_this;

    static bool mHookedVtables = false;

    if (!mHookedVtables) {
        Memory::HookFunction((void*)VTable[1], (void*)&ChatScreenControllerTickDetour, &onChatScreenControllerTick, "ChatScreenController::tick");
        mHookedVtables = true;
    }

    return Memory::CallFunc<void*, ChatScreenController*, Json::Value*>(
        onAddStaticScreenVars, _this, globalVars);
}

class ChatScreenControllerHook : public FuncHook
{
public:
    bool Initialize() override
    {
        Memory::HookFunction(SigManager::ChatScreenController_addStaticScreenVars, (void*)&AddStaticScreenVarsDetour, &onAddStaticScreenVars, "ChatScreenController::addStaticScreenVars");

        return true;
    }
};