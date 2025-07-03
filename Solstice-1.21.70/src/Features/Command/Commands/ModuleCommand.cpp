//
// Created by vastrakai on 7/12/2024.
//

#include "ModuleCommand.hpp"

#include <Features/FeatureManager.hpp>
#include <Utils/StringUtils.hpp>
#include <Utils/GameUtils/ChatUtils.hpp>

#include "spdlog/spdlog.h"

void ModuleCommand::execute(const std::vector<std::string>& args)
{
    if (args.size() < 2)
    {
        ChatUtils::displayClientMessage("§c" + getUsage());
        return;
    }

    std::string action = args[1];
    // If the action is export, look for another arg
    if (action == "export")
    {
        if (args.size() < 3)
        {
            ChatUtils::displayClientMessage("§c" + getUsage());
            return;
        }

        std::string module = args[2];
        // If the module is not found, return
        auto modptr = gFeatureManager->mModuleManager->getModule(module);
        if (!modptr)
        {
            ChatUtils::displayClientMessage("§cThe module §6'" + module + "'§c does not exist!");
            return;
        }

        // Export the module
        nlohmann::json json = gFeatureManager->mModuleManager->serializeModule(modptr);
        // Dump the json and make it as compact as possible (1 indentation level and no newlines)
        std::string jsonStr = json.dump(0, ' ', false);
        // Remove newlines
        std::erase(jsonStr, '\n');
        ChatUtils::displayClientMessage("§aModule exported! §bThe JSON has been copied to your clipboard!");
        ImGui::SetClipboardText(jsonStr.c_str());
        return;
    }

    // If the action is import, look for valid json in the clipboard
    if (action == "import")
    {
        std::string jsonStr = StringUtils::getClipboardText();
        // If the json is empty, return
        if (jsonStr.empty())
        {
            ChatUtils::displayClientMessage("§cNo JSON found in clipboard");
            return;
        }

        // Deserialize the json
        nlohmann::json json;
        try
        {
            json = nlohmann::json::parse(jsonStr);
        }
        catch (const nlohmann::json::parse_error& e)
        {
            ChatUtils::displayClientMessage("§cInvalid JSON found in clipboard! §ePlease copy module JSON to clipboard first!");
            return;
        }

        try
        {
            // Deserialize the module
            gFeatureManager->mModuleManager->deserialize(json, false);
            ChatUtils::displayClientMessage("§aSuccessfully imported module settings!");
        } catch (const std::exception& e)
        {
            ChatUtils::displayClientMessage("§cFailed to import module settings! §e" + std::string(e.what()));
            spdlog::error("Failed to import module settings: {}", e.what());
        }
        return;
    }

    ChatUtils::displayClientMessage("§c" + getUsage());
}

std::vector<std::string> ModuleCommand::getAliases() const
{
    return {"mod", "m"};
}

std::string ModuleCommand::getDescription() const
{
    return "Allows you to export or import a specific module's settings";
}

std::string ModuleCommand::getUsage() const
{
    return "Usage: .module <export/import> <module>";
}
