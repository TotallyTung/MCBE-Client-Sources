#pragma once
//
// Created by vastrakai on 10/15/2023.
//


#include <Prax/Module/Module.h>

class DeviceIDSpoof : public ModuleBase<DeviceIDSpoof> {
public:
    static inline std::string DeviceID = "";
    DeviceIDSpoof() : ModuleBase("DeviceIdSpoof", "Spoofs your device id", "Misc", 0, false) {}

    void OnInitialize() override;
    void OnEnable() override;
    void OnDisable() override;
};