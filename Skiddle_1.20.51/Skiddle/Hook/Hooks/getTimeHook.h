#pragma once
void* __o__getTime;

float GetTimeDetour(void* a1, int a2, float a3) {
    float newTime = CallFunc<float, void*, int, float>(
        __o__getTime,
        a1, a2, a3
    );
    GetTimeofDayEvent event(newTime);
    event.modifier = &newTime;
    DispatchEvent(&event);
    return newTime;
}

class getTimeHook : public FuncHook
{
public:
    static getTimeHook& Instance() {
        static getTimeHook instance;
        return instance;
    }

    bool Initialize() override
    {
        void* getTime = findSig(xorstr_("44 8b c2 b8 ? ? ? ? f7 eA"));
        return HookFunction(getTime, (void*)&GetTimeDetour, &__o__getTime, xorstr_("GetTime"));
    }
};
