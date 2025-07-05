//
// Created by vastrakai on 9/5/2023.
//

#include "ShowNametag.h"
#include <Prax/Utils/String.h>


uintptr_t ShowNametag::NameTagRenderFunc = 0;
std::vector<uint8_t> ShowNametag::NameTagRenderFuncOriginalBytes;

void ShowNametag::OnInitialize() {
    if (NameTagRenderFunc == 0){
        // should be compatible with McPatch
        NameTagRenderFunc = SigManager::GetSignatureAddress(SignatureEnum::Actor_shouldRenderNametag);
        //Logger::Write("ShowNametag", "Found NameTagRenderFunc at " + Misc::GetModuleOfAddress(NameTagRenderFunc));
        if (NameTagRenderFunc != 0){
            NameTagRenderFuncOriginalBytes = Memory::ReadBytes(NameTagRenderFunc, 6);
        }
    }
}

void ShowNametag::OnEnable(){
    if (NameTagRenderFunc != 0){
        Memory::WriteBytes(NameTagRenderFunc, "\x90\x90\x90\x90\x90\x90", 6);
    } else {
        auto player = Minecraft::ClientInstance->ThePlayer();
        if (player) {
            player->addChatMessage("§cUnable to find NameTagRenderFunc§e!§r");
        }
        Toggle();
    }
}

void ShowNametag::OnDisable() {
    if (NameTagRenderFunc != 0){ // 0F 84 69 03 00 00
        Memory::WriteBytes(NameTagRenderFunc, NameTagRenderFuncOriginalBytes, 6);
    }
}
