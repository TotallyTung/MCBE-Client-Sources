//
// Created by vastrakai on 12/27/2023.
//

#include "Zoom.h"

#include <Prax/SDK/Game/Internal/MinecraftGame.h>
#include <Prax/Utils/Math.h>

float Zoom::OldFOV = 0;
bool Zoom::Rendering = false;
float Zoom::TargetFOV = 0;

void Zoom::OnEnable()
{
    TargetFOV = ZoomAmount.Value;
    Minecraft::ClientInstance->GetOptions()->gfx_field_of_view->minimum = 0;
    if (!Rendering) OldFOV = Minecraft::ClientInstance->GetOptions()->gfx_field_of_view->value;
    Rendering = true;
}

void Zoom::OnDisable()
{
    if (!Smooth.Enabled) Minecraft::ClientInstance->GetOptions()->gfx_field_of_view->value = OldFOV;
}

void Zoom::OnTick()
{
}

void Zoom::OnRender()
{
    float deltaTime = ImGui::GetIO().DeltaTime;
    float fov = Minecraft::ClientInstance->GetOptions()->gfx_field_of_view->value;
    if (!Smooth.Enabled && Enabled) {
        Minecraft::ClientInstance->GetOptions()->gfx_field_of_view->value = ZoomAmount.Value;
        Rendering = true;
    }
    else if (Smooth.Enabled && Enabled) {
        Minecraft::ClientInstance->GetOptions()->gfx_field_of_view->value = Math::Animate(fov, TargetFOV, deltaTime * SmoothSpeed.Value);
        Rendering = true;
    } else if (!Enabled && Rendering) {
        Minecraft::ClientInstance->GetOptions()->gfx_field_of_view->value = Math::Animate(fov, OldFOV, deltaTime * SmoothSpeed.Value);
        if (fov == OldFOV) Rendering = false;

    }

}

void Zoom::OnEject()
{
    // set all values back to normal
    if (!Enabled) return;
    Minecraft::ClientInstance->GetOptions()->gfx_field_of_view->minimum = 30;
    Minecraft::ClientInstance->GetOptions()->gfx_field_of_view->value = OldFOV;
    Rendering = false;
}

void Zoom::OnMouseInput(void* Instance, int mouseButton, int isDown, short mouseX, short mouseY, long long a1,
    long long a2, long long a3, bool* cancel)
{
    if (!ScrollZoom.Enabled) return;
    if (!Minecraft::ClientInstance->mcGame->canUseKeys()) return;

    float incAmount = ScrollZoomAmount.Value;
    if (mouseButton == 4) {
        if (isDown == 0x78 || isDown == 0x7F) {
            *cancel = true;
            if (TargetFOV - incAmount <= Minecraft::ClientInstance->GetOptions()->gfx_field_of_view->minimum) return;
            TargetFOV -= incAmount;
        }
        else if (isDown == 0x88 || isDown == 0x80) {
            *cancel = true;
            if (TargetFOV + incAmount >= OldFOV) return;
            TargetFOV += incAmount;
        }
    }
}