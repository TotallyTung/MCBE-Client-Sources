//
// Created by vastrakai on 6/24/2024.
//

#include "BedrockPlatformUWP.hpp"

#include <SDK/OffsetProvider.hpp>

MinecraftGame* BedrockPlatformUWP::getMinecraftGame()
{
    return hat::member_at<MinecraftGame*>(this, OffsetProvider::BedrockPlatformUWP_mcGame);
}
