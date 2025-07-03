//
// Created by vastrakai on 7/7/2024.
//

#include "SetupAndRenderHook.hpp"

#include <SDK/Minecraft/mce.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/Rendering/LevelRenderer.hpp>
#include <Features/Events/DrawImageEvent.hpp>

#include "D3DHook.hpp"

std::unique_ptr<Detour> SetupAndRenderHook::mSetupAndRenderDetour;
std::unique_ptr<Detour> SetupAndRenderHook::mDrawImageDetour;

void* SetupAndRenderHook::onSetupAndRender(void* screenView, void* mcuirc)
{
    auto original = mSetupAndRenderDetour->getOriginal<&SetupAndRenderHook::onSetupAndRender>();

    static bool once = false;
    if (!once)
    {
        once = true;
        initVt(mcuirc);
    }

    auto ci = ClientInstance::get();
    if (!ci) return original(screenView, mcuirc);

    auto player = ClientInstance::get()->getLocalPlayer();

    glm::vec3 origin = glm::vec3(0, 0, 0);
    glm::vec3 playerPos = glm::vec3(0, 0, 0);

    if (player && ci->getLevelRenderer())
    {
        origin = *ci->getLevelRenderer()->getRendererPlayer()->getCameraPos();
        playerPos = player->getRenderPositionComponent()->mPosition;
    }

    if (D3DHook::FrameTransforms) D3DHook::FrameTransforms->push({ ci->getViewMatrix(), origin, playerPos, ci->getFov() });

    return original(screenView, mcuirc);
}

void* SetupAndRenderHook::onDrawImage(void* context, mce::TexturePtr* texture, glm::vec2* pos, glm::vec2* size, glm::vec2* uv,
    mce::Color* color, void* unk)
{
    auto original = mDrawImageDetour->getOriginal<&SetupAndRenderHook::onDrawImage>();

    //nes::event_holder<DrawImageEvent> holder = nes::make_holder<DrawImageEvent>(context, texture, pos, size, uv, color);
    //gFeatureManager->mDispatcher->trigger(holder);
    //if (holder->isCancelled()) return nullptr;

    return original(context, texture, pos, size, uv, color, unk);
}

void SetupAndRenderHook::initVt(void* ctx)
{
    const auto vtable = *static_cast<uintptr_t**>(ctx);
    mDrawImageDetour = std::make_unique<Detour>("MinecraftUIRenderContext::drawImage", reinterpret_cast<void*>(vtable[OffsetProvider::MinecraftUIRenderContext_drawImage]), &SetupAndRenderHook::onDrawImage);
    mDrawImageDetour->enable();
}

void SetupAndRenderHook::init()
{
    mSetupAndRenderDetour = std::make_unique<Detour>("ScreenView::setupAndRender", reinterpret_cast<void*>(SigManager::ScreenView_setupAndRender), &SetupAndRenderHook::onSetupAndRender);
}
