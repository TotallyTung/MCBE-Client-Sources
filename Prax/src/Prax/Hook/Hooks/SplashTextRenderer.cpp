//
// Created by Flash on 9/9/2023.
//

#include <Prax/SDK/SigManager.h>
#include <xorstr.hpp>
#include "SplashTextRenderer.h"
#include <Psapi.h>
#include <Prax/ExceptionHandler.h>

#include <Prax/Utils/System/Internet.h>


std::unique_ptr<Detour> RenderDetour = nullptr;

// unsigned __int64 __fastcall SplashTextRenderer::render(
//        SplashTextRenderer *this,
//        MinecraftUIRenderContext *a2,
//        IClientInstance *a3,
//        UIControl *a4,
//        int a5,
//        RectangleArea *a6,
//        double a7)

unsigned __int64 __fastcall SplashTextRender(void* _this,
                      class MinecraftUIRenderContext* renderContext,
                      class IClientInstance* clientInstance,
                      class UIControl* uiControl,
                      int a5,
                      Rect const& rect,
                      double a7
                      ) {

    auto ofunc = RenderDetour->GetFastcall<unsigned __int64, void*, MinecraftUIRenderContext*, IClientInstance*, UIControl*, int, Rect, double>();



    //return ofunc(_this , renderContext, clientInstance, uiControl, a5, rect, a7);
    return 0;
}


void SplashTextRenderer::Init() {
    auto addr = SigManager::GetSignatureAddress(SignatureEnum::SplashTextRenderer_render);


    RenderDetour = std::make_unique<Detour>("SplashTextRenderer::render", addr, SplashTextRender);
    RenderDetour->Enable();
}

void SplashTextRenderer::UnHook() {

}
