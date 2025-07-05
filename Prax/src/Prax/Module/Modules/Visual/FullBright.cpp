//
// Created by vastrakai on 12/27/2023.
//

#include "FullBright.h"

#include <Prax/Utils/Math.h>


float FullBright::OldGamma = 0;

void FullBright::OnEnable()
{
    Minecraft::ClientInstance->GetOptions()->gfx_gamma->maximum = 100;
    OldGamma = Minecraft::ClientInstance->GetOptions()->gfx_gamma->value;
    Minecraft::ClientInstance->GetOptions()->gfx_gamma->value = 100;
}

void FullBright::OnDisable()
{
    Minecraft::ClientInstance->GetOptions()->gfx_gamma->value = OldGamma;
    Minecraft::ClientInstance->GetOptions()->gfx_gamma->maximum = 1.f;
}