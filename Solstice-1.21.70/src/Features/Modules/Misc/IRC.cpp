//
// Created by vastrakai on 8/24/2024.
//

#include "IRC.hpp"

#include <Features/IRC/IrcClient.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <Utils/OAuthUtils.hpp>

void IRC::onEnable()
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player)
    {
        NotifyUtils::notify("Please don't enable IRC until you're in game!", 10.0f, Notification::Type::Error);
        setEnabled(false);
        return;
    }

    /*if (!OAuthUtils::hasValidToken())
    {
        ChatUtils::displayClientMessageRaw("§7[§dirc§7] §eFailed to authenticate with Discord. Please authenticate with the injector.");
        NotifyUtils::notify("Failed to authenticate with Discord. Please authenticate with the injector.", 10.0f, Notification::Type::Error);
        setEnabled(false);
        return;
    }*/

    IrcManager::init();
    IrcManager::setShowNamesInChat(mShowNamesInChat.mValue);
    IrcManager::setAlwaysSendToIrc(mAlwaysSendToIrc.mValue);
    IrcManager::mLastConnectAttempt = NOW;



}

void IRC::onDisable()
{
    auto player = ClientInstance::get()->getLocalPlayer();
    bool isConnected = IrcManager::mClient && IrcManager::mClient->mConnectionState == ConnectionState::Connected;
    // if we're not in game AND not connected, don't deinit
    if (!player && !isConnected) return;

    IrcManager::deinit();

}

void IRC::onModuleStateChangeEvent(ModuleStateChangeEvent& event)
{
    if (event.mModule != this) return;

    if (IrcManager::mClient && IrcManager::mClient->mConnectionState == ConnectionState::Connecting)
    {
        ChatUtils::displayClientMessageRaw("§7[§dirc§7] §eYou can't disable IRC while it's connecting!");
        event.cancel();
    }
}

void IRC::onBaseTickEvent(BaseTickEvent& event)
{
    if (!mEnabled) return;

    IrcManager::setShowNamesInChat(mShowNamesInChat.mValue);
    IrcManager::setAlwaysSendToIrc(mAlwaysSendToIrc.mValue);
    if (IrcManager::mClient && IrcManager::mLastConnectAttempt + 5000 < NOW && IrcManager::mClient->mConnectionState == ConnectionState::Disconnected)
    {
        ChatUtils::displayClientMessageRaw("§7[§dirc§7] §eAttempting to reconnect to IRC...");
        IrcManager::init();
    }

}