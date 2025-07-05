#pragma once

#include "ToggleSounds.h"
#include <Prax/Utils/Render/Font.h>

void ToggleSounds::OnToggle(bool enabled) {
    if (!Enabled) return;

    //TODO: Replace with prax server, or not- you can keep it if you want, its up 24/7
    switch (SoundSetting.SelectedIndex) {
        case 0:
            Minecraft::ClientInstance->playUI("random.lever_click", 1, enabled ? 0.6f : 0.5f);
            break;
        case 1:
            Misc::PlaySoundFromUrl(enabled ? "https://yiffing.zone/sounds/notify_on.wav" : "https://yiffing.zone/sounds/notify_off.wav", 0.2f);
            break;
        case 2:
            Misc::PlaySoundFromUrl("https://yiffing.zone/sounds/click.wav", 0.2f);
            break;
    }
}
