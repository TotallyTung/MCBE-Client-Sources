// #pragma once
// void* __o__ChatLogFunc;

// using namespace std;

// void chatLogHookFunc(__int64 a1, TextHolder* msg, uint32_t a2) {
//     // Your existing code for setup
//     // auto message = *reinterpret_cast<TextHolder*>(reinterpret_cast<__int64>(msg) + 0x8);
//     // static auto player = Game::GetLocalPlayer();

//     // if (player == NULL)
//     //     return;

//     // Remove insults, autoqueue, autogg, and kills functionality

//     // Call the original chat logging function
//     // return CallFunc<void, __int64, TextHolder*, uint32_t>(
//     //     __o__ChatLogFunc,
//     //     a1,
//     //     msg,
//     //     a2
//     // );
// }

// class ChatLogHook : public FuncHook
// {
// public:
//     static ChatLogHook& Instance()
//     {
//         static ChatLogHook instance;
//         return instance;
//     }

//     bool Initialize() override
//     {
//         // void* chatlogAddr = findSig(xorstr_("40 55 57 41 56 48 83 EC ? 48 8B 81"));
//         // return HookFunction(chatlogAddr, (void*)&chatLogHookFunc, &__o__ChatLogFunc, xorstr_("ChatLogFunc"));
//     }
// };
