//
// Created by vastrakai on 12/23/2023.
//

#include "NoFilter.h"

void NoFilter::OnEnable()
{
    wasEnabled = Minecraft::ClientInstance->GetProfanityContext()->enabled;
    Minecraft::ClientInstance->GetProfanityContext()->enabled = false;
}

void NoFilter::OnTick()
{
    Minecraft::ClientInstance->GetProfanityContext()->enabled = false;
}

void NoFilter::OnDisable()
{
    Minecraft::ClientInstance->GetProfanityContext()->enabled = wasEnabled;
}
