//
// Created by vastrakai on 9/5/2023.
//

#include "NoCameraClip.h"
#include <Prax/Utils/String.h>


uintptr_t NoCameraClip::CameraClipFunc = 0;

void NoCameraClip::OnInitialize() {
    if (CameraClipFunc == 0) {
        // Alternative sig: ? ? ? ? ? ? 56 48 8? ? ? ? 48 8? ? ? ? ? ? 48 8? ? ? ? ? ?  48 3? C4 48 8? ? ? 49 8? ? ? 49 8?
        CameraClipFunc = SigManager::GetSignatureAddress(SignatureEnum::CameraClipFunc);
        //Logger::Write("NoCameraClip", "Found CameraClipFunc at " + Misc::GetModuleOfAddress(CameraClipFunc));
    }

}

void NoCameraClip::OnEnable() {
    // Write C3 90 to the func
    if (CameraClipFunc != 0) {
        Memory::WriteBytes(CameraClipFunc, "\xC3\x90", 2);
    } else {
        auto player = Minecraft::ClientInstance->ThePlayer();
        if (player) {
            player->addChatMessage("§cUnable to find CameraClipFunc§e!§r");
        }
        Toggle();
    }
}

void NoCameraClip::OnDisable() {
    // Write 40 55 to the func
    if (CameraClipFunc != 0) {
        Memory::WriteBytes(CameraClipFunc, "\x40\x55", 2);
    }
}
