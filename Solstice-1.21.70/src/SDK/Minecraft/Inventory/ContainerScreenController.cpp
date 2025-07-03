//
// Created by vastrakai on 7/5/2024.
//

#include "ContainerScreenController.hpp"

#include <SDK/OffsetProvider.hpp>
#include <SDK/SigManager.hpp>
#include <Utils/MemUtils.hpp>

void ContainerScreenController::handleAutoPlace(const std::string& name, int slot)
{
    static auto func = SigManager::ContainerScreenController_handleAutoPlace;
    return MemUtils::callFastcall<void>(func, this, 0x7FFFFFFF, name, slot);
}

void* ContainerScreenController::_tryExit()
{
    // TODO: implement index signature instead of raw index
    return MemUtils::callVirtualFunc<void*>(OffsetProvider::ContainerScreenController_tryExit, this);
}
