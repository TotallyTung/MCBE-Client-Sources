//
// Created by vastrakai on 2/10/2024.
//

#include "SelectionColor.h"

// Store the 10 bytes
std::vector<uint8_t> SelectionColor::OgBytes;

void SelectionColor::OnEnable()
{
    static uint64_t func = SigManager::GetSignatureAddress(SignatureEnum::LevelRendererPlayer_renderOutlineSelectionOpcode);
    if (func == 0) {
        auto player = Minecraft::ClientInstance->ThePlayer();
        if (player) {
            player->addChatMessage("§cUnable to find LevelRendererPlayer_renderOutlineSelectionOpcode§e!§r");
        }
        Toggle();
        return;
    }

    // Nop 10 bytes here
    OgBytes = Memory::ReadBytes(func, 10);
    Memory::WriteBytes(func + 7, "\x90\x90\x90", 3);
    // \x0F\x10\x05\xE2\x5D\x27\x04\x0F\x11\x00
}

void SelectionColor::OnDisable()
{
    static uint64_t func = SigManager::GetSignatureAddress(SignatureEnum::LevelRendererPlayer_renderOutlineSelectionOpcode);
    if (func == 0) return;
    Memory::WriteBytes(func, OgBytes, 10);
}
