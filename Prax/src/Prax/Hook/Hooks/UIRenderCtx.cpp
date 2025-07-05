#pragma once

#include "UIRenderCtx.h"
#include <src/Prax/SDK/Game/Minecraft.h>
#include <Prax/Utils/System/Internet.h>
#include <Prax/Utils/String.h>
#include <Prax/SDK/Game/Render/GuiData.h>
#include <Prax/SDK/Game/Render/MinecraftUIRenderContext.h>
#include <Prax/Module/Modules/Visual/HUD.h>
#include <Prax/Module/ModuleManager.h>
#include <Prax/SDK/Game/Render/LevelRenderer.h>

#include "D3D.h"

std::unique_ptr<Detour> drawImageHook = 0;
std::unique_ptr<Detour> draw9SliceHook = 0;


MinecraftUIRenderContext* UIRenderCtx::context = nullptr;

Vector2 hotbarPos = Vector2(0, 0);

void* drawImageDetour(void* a1, TexturePtr* texturePtr, Vector2* ImagePos, Vector2* imageSize, Vector2* uvPos, Vector2* uvSize) {
    auto oFunc = drawImageHook.get()->GetFastcall<void*, void*, TexturePtr*, Vector2*, Vector2*, Vector2*, Vector2*>();
    std::string screen = Minecraft::ClientInstance->GetScreenName();
    if (screen != "start_screen" && screen != "hud_screen") return oFunc(a1, texturePtr, ImagePos, imageSize, uvPos, uvSize);
    auto pathStr = texturePtr->texture.get()->filePath.c_str();

    if (screen == "start_screen") {



        /*if (String::ContainsIgnoreCase(pathStr, "prax"))
            return oFunc(a1, texturePtr, ImagePos, imageSize, uvPos, uvSize);

        if (strcmp(pathStr, "textures/ui/title") == 0 &&
            (~strcmp(pathStr, "assets/minecraft/textures/gui/title/minecraft") == 0 ||
             strcmp(pathStr, "assets/minecraft/textures/gui/title/edition") != 0)) {
            if (UIRenderCtx::context != nullptr) {

                Vector2 resolution = Minecraft::ClientInstance->guiData->resolution;

                UIRenderCtx::context->fillRectangle(Rect(0, resolution.x, 0, resolution.y), rgb(0.06f), 1.0f);
                UIRenderCtx::context->DrawImageFromUrl(
                        "https://github.com/Prax-Client/Releases/raw/main/banner",
                        "banner", ImagePos, imageSize, uvPos, uvSize);
                UIRenderCtx::context->FlushCustomImage();

                return 0;
            }
        }*/
    }


    static auto* mod = ModuleManager::GetModule<HUD>();

    if (!mod) {
        mod = ModuleManager::GetModule<HUD>();
        return oFunc(a1, texturePtr, ImagePos, imageSize, uvPos, uvSize);
    }

    if (screen == "hud_screen") {
        // check if it is textures/ui/selected_hotbar_slot
        if (strcmp(pathStr, "textures/ui/selected_hotbar_slot") == 0) {
            float deltaTime = ImGui::GetIO().DeltaTime * (mod->SelectedSlotEasing.Value * 2.f);
            if (hotbarPos.x == 0 || hotbarPos.y == 0) hotbarPos = *ImagePos;
            hotbarPos = Vector2::Lerp(hotbarPos, *ImagePos, deltaTime);
            *ImagePos = hotbarPos;
        }
    }

    return oFunc(a1, texturePtr, ImagePos, imageSize, uvPos, uvSize);



    /*else {


        std::string newPath = pathStr;

        size_t lastSeparatorPos = newPath.find_last_of("/\\");
        if (lastSeparatorPos != std::string::npos) {
            // Extract the substring after the last separator to get the filename
            newPath = newPath.substr(lastSeparatorPos + 1);
        }

        std::string fullPath = FileSystem::ResourcesDirectory + "\\" + newPath;

        std::string sillyPicture = "url";

        if (!FileSystem::FileExists(fullPath)) {
            Logger::Write("Silly picture not found", "downloading...");
            sillyPicture = "https://cataas.com/cat";
        }

        UIRenderCtx::context->DrawImageFromUrl(sillyPicture,
                                  newPath, ImagePos, imageSize, uvPos, uvSize);
        UIRenderCtx::context->FlushCustomImage();

        return 0;
    }*/
}

__int64 drawNinesliceDetour(void* a1, TexturePtr* texturePtr, NinesliceInfo* ninesliceInfo, int degrees) {
    auto oFunc = draw9SliceHook.get()->GetFastcall<__int64, void*, TexturePtr *, const NinesliceInfo*, int>();

    //auto texture = UIRenderCtx::context->CreateTexture("textures/ui/title");

    // if textureptr contains ui then darken it
    /*if (String::ContainsIgnoreCase(texturePtr->texture->filePath.getText(), "ui")) {
        ninesliceInfo->topLeft.
    }*/


    return oFunc(a1, texturePtr, ninesliceInfo, degrees);
}

void UIRenderCtx::InitVtableHooks(MinecraftUIRenderContext* ctx) {
    auto VTable = *(uintptr_t**)ctx;
    drawImageHook = std::make_unique<Detour>("DrawImage", VTable[7], &drawImageDetour);
    drawImageHook->Enable();
    /*draw9SliceHook = std::make_unique<Detour>("NineSlice", VTable[8], &drawNinesliceDetour);
    draw9SliceHook->Enable();*/
}


std::unique_ptr<Detour> renderCtxHook = 0;

void renderCtxDetour(ScreenView* screenView, MinecraftUIRenderContext* ctx) {
    auto oFunc = renderCtxHook.get()->GetFastcall<void, ScreenView*, MinecraftUIRenderContext*>();


    UIRenderCtx::context = ctx;
    Minecraft::ClientInstance = ctx->clientInstance;


    static bool isFirstUIRender = true;

    if (isFirstUIRender) {
        UIRenderCtx::InitVtableHooks(ctx);
        //Logger::Write("UIRenderCtx", Misc::GetModuleOfAddress((uintptr_t)ctx));

        isFirstUIRender = false;
    }



    auto player = Minecraft::ClientInstance->ThePlayer();

    glm::vec3 origin = { 0, 0, 0 };
    Vector3 pos = { 0, 0, 0 };

    if (player && Minecraft::ClientInstance->levelRenderer)
    {
        origin = Minecraft::ClientInstance->levelRenderer->levelRendererPlayer->cameraPos;
        pos = player->GetLerpedPos();
    }

    FrameTransform transform = { *Minecraft::ClientInstance->GetViewMatrix(), origin, pos};

    D3D::FrameTransforms.push(transform);

    return oFunc(screenView, ctx);
}


// unsigned __int64 __fastcall UIScene::render(IClientInstance **, ScreenContext *)
std::unique_ptr<Detour> uiRender = 0;

void UISceneRenderDetour(class IClientInstance** clientInstance, ScreenContext* screenContext) {
    auto oFunc = uiRender.get()->GetFastcall<void, IClientInstance**, ScreenContext*>();

    /*if (UIRenderCtx::context && Minecraft::ClientInstance->guiData)
        UIRenderCtx::context->fillRectangle(Rect(0, Minecraft::ClientInstance->guiData->resolution.x, 0, Minecraft::ClientInstance->guiData->resolution.y), rgb(), 1.0f);
*/

    return oFunc(clientInstance, screenContext);
}

void UIRenderCtx::Init() {
    uintptr_t renderCtxAddr = Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::renderCtxAddr));

    // Print out the address of the render context
    //Logger::Write("UIRenderCtx", Misc::GetModuleOfAddress(renderCtxAddr));

    renderCtxHook = std::make_unique<Detour>("UIRenderCTX", renderCtxAddr, &renderCtxDetour);
    renderCtxHook->Enable();
    this->Detours.emplace_back(renderCtxHook.get());

    // 48 89 ? ? ? 55 56 57 41 ? 41 ? 41 ? 41 ? 48 8D ? ? ? 48 81 EC ? ? ? ? 48 8B ? ? ? ? ? 48 33 ? 48 89 ? ? 4D 8B ? 4D 8B ? 48 8B ? 45 33

    /*// Address of signature = Minecraft.Windows.exe + 0x00367180
    //"\x48\x89\x00\x00\x00\x48\x89\x00\x00\x00\x57\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x00\x00\x00\x00\x00\x48\x33\x00\x48\x89\x00\x00\x00\x00\x00\x00\x48\x8B\x00\x48\x8B\x00\xB9", "xx???xx???xxxx????xx?????xx?xx??????xx?xx?x"
    //"48 89 ? ? ? 48 89 ? ? ? 57 48 81 EC ? ? ? ? 48 8B ? ? ? ? ? 48 33 ? 48 89 ? ? ? ? ? ? 48 8B ? 48 8B ? B9"

    uintptr_t uiRenderAddr = Memory::SearchAOB("48 89 ? ? ? 48 89 ? ? ? 57 48 81 EC ? ? ? ? 48 8B ? ? ? ? ? 48 33 ? 48 89 ? ? ? ? ? ? 48 8B ? 48 8B ? B9");

    uiRender = std::make_unique<Detour>("UIScene::Render", uiRenderAddr, &UISceneRenderDetour);
    uiRender->Enable();
    this->Detours.emplace_back(uiRender.get());*/
}

void UIRenderCtx::UnHook() {
}
