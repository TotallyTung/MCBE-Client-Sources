//
// Created by vastrakai on 10/15/2023.
//

#include "DeviceIDSpoof.h"

void DeviceIDSpoof::OnInitialize()
{
    Toggle(); // So on enable is called
}

void DeviceIDSpoof::OnEnable()
{
    DeviceID = String::GenerateUUID();
    Logger::Write("Privacy", "Spoofing Device ID to " + DeviceID + "...");
}

void DeviceIDSpoof::OnDisable()
{
    DeviceID = "";
    Logger::Write("Privacy", "No longer spoofing.");
}
