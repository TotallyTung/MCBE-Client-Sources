//
// Created by vastrakai on 7/5/2024.
//

#include "ContainerScreenControllerHook.hpp"

#include <memory>
#include <Features/Events/ContainerScreenTickEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>

std::unique_ptr<Detour> ContainerScreenControllerHook::mDetour;

uint32_t ContainerScreenControllerHook::onContainerTick(class ContainerScreenController *csc)
{
    auto original = mDetour->getOriginal<&ContainerScreenControllerHook::onContainerTick>();

    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return original(csc); // just in case
    if (player->getContainerManagerModel()->mContainerType != ContainerType::Container)
    {
        spdlog::debug("Not a container - screen {} (id: {})", std::string(magic_enum::enum_name(player->getContainerManagerModel()->mContainerType)), player->getContainerManagerModel()->mContainerType);
        return original(csc);
    }

    auto holder = nes::make_holder<ContainerScreenTickEvent>(csc);
    gFeatureManager->mDispatcher->trigger(holder);

    return original(csc);
}

void ContainerScreenControllerHook::init()
{
    auto func = SigManager::ContainerScreenController_tick;
    mDetour = std::make_unique<Detour>("ContainerScreenController::tick", reinterpret_cast<void*>(func), &ContainerScreenControllerHook::onContainerTick);
}