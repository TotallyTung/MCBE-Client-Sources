//
// Created by vastrakai on 10/10/2024.
//

#include "KeyboardMouseSettings.hpp"

int KeyboardMouseSettings::operator[](const std::string& key)
{
    for (auto& bind : mKeyTypeA) {
        if (bind.mBindName == key)
            return bind.mBindKey[0];
    }

    spdlog::error("Failed to find keybind: {}", key);
    return -1;
}

KeyboardMouseSettings* ClientInputMappingFactory::getKeyboardMouseSettings()
{
    return hat::member_at<KeyboardMouseSettings*>(this, OffsetProvider::ClientInputMappingFactory_mKeyboardMouseSettings);
}

ClientInputMappingFactory* ClientInputHandler::getMappingFactory()
{
    return hat::member_at<ClientInputMappingFactory*>(this, OffsetProvider::ClientInputHandler_mMappingFactory);
}
