//
// Created by vastrakai on 6/24/2024.
//

#include "ClientInstance.hpp"

#include <libhat/Access.hpp>
#include <SDK/OffsetProvider.hpp>
#include <SDK/SigManager.hpp>

#include "MinecraftGame.hpp"

#include <Utils/Structs.hpp>

#include "lualib.h"
#include "Actor/Actor.hpp"

#include "KeyboardMouseSettings.hpp"

ClientInstance* ClientInstance::get()
{
    static MinecraftGame* game = MinecraftGame::getInstance();
    if (game == nullptr)
    {
        game = MinecraftGame::getInstance();
        return nullptr;
    }
    static ClientInstance* instance = game->getPrimaryClientInstance();
    if (instance == nullptr) instance = game->getPrimaryClientInstance();
    return instance;
}

glmatrixf ClientInstance::getViewMatrix()
{
    return hat::member_at<glmatrixf>(this, OffsetProvider::ClientInstance_mViewMatrix);
}

glm::vec2 ClientInstance::getFov()
{
    float x = hat::member_at<float>(this, OffsetProvider::ClientInstance_mFovX); // 0x6F0
    float y = hat::member_at<float>(this, OffsetProvider::ClientInstance_mFovY); // 0x704
    return { x, y };
}

MinecraftSim* ClientInstance::getMinecraftSim()
{
    return hat::member_at<MinecraftSim*>(this, OffsetProvider::ClientInstance_mMinecraftSim);
}

LevelRenderer* ClientInstance::getLevelRenderer()
{
    return hat::member_at<LevelRenderer*>(this, OffsetProvider::ClientInstance_mLevelRenderer);
}

LoopbackPacketSender* ClientInstance::getPacketSender()
{
    return hat::member_at<LoopbackPacketSender*>(this, OffsetProvider::ClientInstance_mPacketSender);
}

GuiData* ClientInstance::getGuiData()
{
    return hat::member_at<GuiData*>(this, OffsetProvider::ClientInstance_mGuiData);
}

bgfx_context* ClientInstance::getBGFX()
{
    return *reinterpret_cast<bgfx_context**>(SigManager::ClientInstance_mBgfx);
}

ClientInputHandler* ClientInstance::getInputHandler()
{
    return MemUtils::callVirtualFunc<ClientInputHandler*>(OffsetProvider::ClientInstance_getInputHandler, this);
}

KeyboardMouseSettings* ClientInstance::getKeyboardSettings()
{
    return getInputHandler()->getMappingFactory()->getKeyboardMouseSettings();
}

MinecraftGame* ClientInstance::getMinecraftGame()
{
    return MinecraftGame::getInstance();
}

Actor* ClientInstance::getLocalPlayer()
{
    return MemUtils::callVirtualFunc<Actor*>(OffsetProvider::ClientInstance_getLocalPlayer, this);
}

BlockSource* ClientInstance::getBlockSource()
{
    return MemUtils::callVirtualFunc<BlockSource*>(OffsetProvider::ClientInstance_getBlockSource, this);
}

Options* ClientInstance::getOptions()
{
    return MemUtils::callVirtualFunc<Options*>(OffsetProvider::ClientInstance_getOptions, this);
}

std::string ClientInstance::getScreenName()
{
    std::string name = "no_screen";
    name = MemUtils::callVirtualFunc<std::string&, std::string&>(OffsetProvider::ClientInstance_getScreenName, this, name);
    return name;
}

void ClientInstance::setDisableInput(bool disable)
{
    MemUtils::callVirtualFunc<void>(OffsetProvider::ClientInstance_setDisableInput, this, disable);
}

bool ClientInstance::getMouseGrabbed()
{
    // idk why but the original virtual func's result was inverted
    // so im just gonna recreate that here because i'm too lazy to refactor everything
    return !getMinecraftGame()->getMouseGrabbed();
}

void ClientInstance::grabMouse()
{
    static uintptr_t func = SigManager::ClientInstance_grabMouse;
    MemUtils::callFastcall<void>(func, this);

}

void ClientInstance::releaseMouse()
{
    static uintptr_t func = SigManager::ClientInstance_releaseMouse;
    MemUtils::callFastcall<void>(func, this);
    getMinecraftGame()->setMouseGrabbed(false);
}

void ClientInstance::playUi(const std::string& soundName, float volume, float pitch)
{   // this is exactly what the game does
    getMinecraftGame()->playUi(soundName, volume, pitch);
}
