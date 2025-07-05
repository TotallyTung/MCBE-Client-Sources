//
// Created by vastrakai on 9/23/2023.
//

#include "Swing.h"

uintptr_t fluxSwingAddr = 0x0;
float* tapSwingVal = nullptr;
// og bytes
void* ogBytes[5] = { 0 };

void patchAnimFunc(bool patch = true)
{
    static uintptr_t addr = SigManager::GetSignatureAddress(SignatureEnum::ItemInHandRenderer_renderBp);
    if (addr == 0) return;

    std::vector<uint8_t> bytes = {};

    if (patch) {
        bytes = { 0x90, 0x90, 0x90, 0x90 };
    }
    else {
        bytes = { 0xF3, 0x0F, 0x5C, 0xD0 };
    }

    Memory::WriteBytes(addr, bytes, bytes.size());
}

void Swing::OnEnable() {
    if (fluxSwingAddr == 0) fluxSwingAddr = SigManager::GetSignatureAddress(SignatureEnum::fluxSwing);
    if (fluxSwingAddr == 0) return;
    if (tapSwingVal == nullptr){
        tapSwingVal = reinterpret_cast<float*>(Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::TapSwingAnim), 5));
        // set memory protection
        Memory::SetProtection(reinterpret_cast<uintptr_t>(tapSwingVal), 4, PAGE_READWRITE);
    }

    // write 5 og bytes to the ogBytes variable
    // void Memory::ReadBytes(uintptr_t address, uint32_t *bytes, int length) {
    Memory::ReadBytes((void*)fluxSwingAddr, ogBytes, 5);
    // write the new bytes
    if (FluxSwing.Enabled) Memory::WriteBytes(fluxSwingAddr, "\x90\x90\x90\x90\x90", 5);
    patchAnimFunc(NoSwitchAnimation.Enabled);

}

void Swing::OnTick() {
    if (fluxSwingAddr == 0) return;
    if (FluxSwing.Enabled) Memory::WriteBytes(fluxSwingAddr, "\x90\x90\x90\x90\x90", 5);
    else Memory::WriteBytes(fluxSwingAddr, (char*)ogBytes, 5);

    if (CustomSwingAngle.Enabled) {
        *tapSwingVal = CustomSwingAngleValue.Value;
    }
    else {
        *tapSwingVal = -80.f;
    }

    patchAnimFunc(NoSwitchAnimation.Enabled);

}

void Swing::OnDisable() {
    if (fluxSwingAddr == 0) return;
    // write the og bytes
    if (FluxSwing.Enabled) Memory::WriteBytes(fluxSwingAddr, (char*)ogBytes, 5);
    patchAnimFunc(false);
    *tapSwingVal = -80.f;
}

uint64_t Swing::LastSwingTime = 0;

void Swing::OnActorSwing(bool *cancel) {
    auto player = Minecraft::ClientInstance->ThePlayer();
    uint64_t delay = (uint64_t)(SwingDelay.Value) * 1000;
    uint64_t now = Misc::GetCurrentMs();
    // make sure to use player->addChatMessage for logging and log out value n stuff
    if (now - LastSwingTime < delay) {
        *cancel = true;
        return;
    }
    LastSwingTime = now;

}