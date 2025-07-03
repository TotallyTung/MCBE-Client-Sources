//
// Created by vastrakai on 7/8/2024.
//

#include "QueueCommand.hpp"

#include <Features/Modules/Misc/AutoQueue.hpp>
#include <Utils/GameUtils/ChatUtils.hpp>
#include <Utils/MiscUtils/ColorUtils.hpp>

void QueueCommand::execute(const std::vector<std::string>& args)
{
    auto autoQueue = gFeatureManager->mModuleManager->getModule<AutoQueue>();
    if (autoQueue == nullptr)
    {
        ChatUtils::displayClientMessage("§cERROR: AutoQueue module not found.");
        return;
    }

    if (!autoQueue->mEnabled)
    {
        ChatUtils::displayClientMessage("§cAutoQueue is not enabled!");
        return;
    }

    autoQueue->mQueueForGame = true;
    autoQueue->mLastQueueTime = NOW - autoQueue->mQueueDelay.mValue * 1000; // Instantly queue
}

std::vector<std::string> QueueCommand::getAliases() const
{
    return {"q"};
}

std::string QueueCommand::getDescription() const
{
    return "Automatically queues for a game.";
}

std::string QueueCommand::getUsage() const
{
    return "Usage .queue";
}
