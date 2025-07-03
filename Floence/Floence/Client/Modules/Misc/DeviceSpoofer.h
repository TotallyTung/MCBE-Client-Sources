#pragma once

class DeviceSpoofer : public Module
{
public:
    DeviceSpoofer(int keybind = 86, bool enabled = false) :
        Module("DeviceSpoofer", "Misc", "Spoof the device id.", keybind, enabled)
    {}

};