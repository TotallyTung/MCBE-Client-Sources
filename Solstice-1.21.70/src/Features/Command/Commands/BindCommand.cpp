//
// Created by vastrakai on 6/29/2024.
//

#include "BindCommand.hpp"

#include <Utils/GameUtils/ChatUtils.hpp>
#include <Utils/Keyboard.hpp>

void BindCommand::execute(const std::vector<std::string>& args)
{
    /*if (args.size() < 3)
    {
        ChatUtils::displayClientMessage("§c" + getUsage());
        return;
    }*/

    // If no args were provided, show all the modules and their keys (if bound)
    if (args.size() == 1)
    {
        ChatUtils::displayClientMessage("§6Binds: ");
        for (const auto& module : gFeatureManager->mModuleManager->mModules)
        {
            if (module->mKey == 0) continue;
            std::string keyStr = Keyboard::getKey(module->mKey);
            ChatUtils::displayClientMessage("§6- §e{} §7: §6{}", module->getName(), keyStr);
        }

        ChatUtils::displayClientMessage("§6Use §e.bind <module> <key> §6to bind a module to a key");
        ChatUtils::displayClientMessage("§6Use §e.bind <module> §6to see the key a module is bound to");

        return;
    }

    // if only one arg was provided (module name), show the key it's bound to
    if (args.size() == 2)
    {
        const auto& moduleName = args[1];
        auto module = gFeatureManager->mModuleManager->getModule(moduleName);

        if (!module)
        {
            ChatUtils::displayClientMessage("§cThe module §6'{}'§c does not exist!{}", moduleName, ""); // bruh
            return;
        }

        std::string keyStr = Keyboard::getKey(module->mKey);
        ChatUtils::displayClientMessage("§6{}§a is currently bound to §6{}§a.", module->getName(), keyStr);
        return;
    }

    // Else, bind the module to the key

    const auto& moduleName = args[1];
    const auto& key = args[2];

    int keyId = Keyboard::getKeyId(key);

    auto module = gFeatureManager->mModuleManager->getModule(moduleName);

    if (!module)
    {
        return;
    }

    module->mKey = keyId;

    std::string keyStr = Keyboard::getKey(keyId);
    ChatUtils::displayClientMessage("§6{}§a is now bound to §6" + keyStr + "§a!",  module->getName(), keyStr);
}

std::vector<std::string> BindCommand::getAliases() const
{
    return {"b"};
}

std::string BindCommand::getDescription() const
{
    return "Binds a module to a key";
}

std::string BindCommand::getUsage() const
{
    return "Usage: .bind [module] [key]";
}
