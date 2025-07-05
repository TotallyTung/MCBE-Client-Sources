//
// Created by Flash on 10/15/2023.
//

#pragma once

#include <libhat/Scanner.hpp>
#include <Prax/Utils/System/Memory.h>

#include <winrt/Windows.UI.Notifications.h>
#include <winrt/windows.applicationmodel.core.h>

class BedrockPlatformUWP
{
public:
    BUILD_ACCESS(this, MinecraftGame**, mcGame, 0x28)

    MinecraftGame* getMinecraftGame()
    {
        return *direct_access<MinecraftGame**>(this, 0x28);
    }
};

class MainView
{
public:
    BUILD_ACCESS(this, BedrockPlatformUWP*, bedrockPlatform, 0x118)

    BedrockPlatformUWP* getBedrockPlatform()
    {
        return direct_access<BedrockPlatformUWP*>(this, 0x118);
    }

    static MainView* GetInstance()
    {
        static MainView** mainView = reinterpret_cast<MainView**>(Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::MainView_instance), 3));
        return *mainView;
    }
};

class MinecraftGame
{
public:
    class UIProfanityContext* GetProfanityContext()
    {
        return direct_access<class UIProfanityContext*>(this, 0x198);
    }

    class FontRepository* fontRepository() {
        return *(FontRepository**)(reinterpret_cast<uintptr_t>(this) + 0x112);
    }

    static MinecraftGame* GetInstance()
    {
        if (!MainView::GetInstance() || !MainView::GetInstance()->getBedrockPlatform() || !MainView::GetInstance()->getBedrockPlatform()->getMinecraftGame())
            return nullptr;
        return MainView::GetInstance()->getBedrockPlatform()->getMinecraftGame();
    }

    ClientInstance* getPrimaryClientInstance()
    {
        // std::map<unsigned char, std::shared_ptr<class ClientInstance>>* clientInstances; at 0xA08
        Logger::Write("ClientInstance", String::ToHex(reinterpret_cast<uintptr_t>(this) + 0xA08), Logger::LogType::Debug);
        Misc::SetClipboard(String::ToHex(reinterpret_cast<uintptr_t>(this) + 0xA08));
        auto helpme = reinterpret_cast<std::map<unsigned char, std::shared_ptr<class ClientInstance>>*>(reinterpret_cast<uintptr_t>(this) + 0xA08);

        for (auto& clientInstance : *helpme)
        {
            return clientInstance.second.get();
        }

        return nullptr;
    }

    // Returns TRUE if you can use your keyboard keys.
    bool canUseKeys()//Just a byte value
    {
        //return *(bool*)(reinterpret_cast<__int64>(this) + 0x190);
        return direct_access<bool>(this, 0x1A8);
    };

    // Returns the default bitmap font.
    class Font* bitmapFont()//Near a lot of random strings that say OnApp....
    {
        return *reinterpret_cast<class Font**>(reinterpret_cast<__int64>(this) + 0xE70);
    };

};