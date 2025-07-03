//
// Created by vastrakai on 6/24/2024.
//

#include "MinecraftGame.hpp"
#include <map>
#include <memory>
#include <libhat/Access.hpp>
#include <SDK/OffsetProvider.hpp>
#include <Utils/MemUtils.hpp>

#include "UWP/BedrockPlatformUWP.hpp"
#include "UWP/MainView.hpp"

MinecraftGame* MinecraftGame::getInstance()
{
    if (!MainView::getInstance() || !MainView::getInstance()->getBedrockPlatform() || !MainView::getInstance()->getBedrockPlatform()->getMinecraftGame())
        return nullptr;
    return MainView::getInstance()->getBedrockPlatform()->getMinecraftGame();
}

ClientInstance* MinecraftGame::getPrimaryClientInstance()
{
    auto helpme = hat::member_at<std::map<unsigned char, std::shared_ptr<class ClientInstance>>, MinecraftGame>(this, OffsetProvider::MinecraftGame_mClientInstances);

    for (auto& clientInstance : helpme)
    {
        return clientInstance.second.get();
    }

    return nullptr;
}

UIProfanityContext* MinecraftGame::getProfanityContext()
{
    return hat::member_at<UIProfanityContext*>(this, OffsetProvider::MinecraftGame_mProfanityContext);
}

bool MinecraftGame::getMouseGrabbed()
{
    return hat::member_at<bool>(this, OffsetProvider::MinecraftGame_mMouseGrabbed);
}

void MinecraftGame::setMouseGrabbed(bool grabbed)
{
    hat::member_at<bool>(this, OffsetProvider::MinecraftGame_mMouseGrabbed) = grabbed;
}

void MinecraftGame::playUi(const std::string& soundName, float volume, float pitch)
{
    int index = OffsetProvider::MinecraftGame_playUi;
    MemUtils::callVirtualFunc<void, const std::string&, float, float>(index, this, soundName, volume, pitch);
}
