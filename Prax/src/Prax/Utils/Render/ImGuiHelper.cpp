#pragma once

#include "ImGuiHelper.h"

#define IMGUI_DEFINE_MATH_OPERATORS

#include "Font.h"
#include "Image.h"
#include <d3d11.h>
#include <d3d12.h>
#include <imgui.h>
#include "Render.h"
#include "../Math.h"
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <Prax/UI/ClickGui.h>
#include <Prax/UI/NewClickGui.h>
#include "Prax/Hook/HookManager.h"
#include <Prax/UI/Notifications.h>
#include <Prax/Utils/Render/Resource.h>
#include "Prax/Hook/Hooks/SwingDurationHook.h"
#include <Prax/SDK/Game/Render/LevelRenderer.h>
#include <Prax/Hook/Hooks/D3D.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool initImGui = false;

HWND window;

void ImGuiHelper::Init(IDXGISwapChain *pSwapChain, ID3D11Device *pDevice, ID3D11DeviceContext *pContext) {
    if (!initImGui)
    {

        window = D3D::GetHWND();

        {
            ImGui::CreateContext();

            // Draw mouse cursor
            ImGuiIO& io = ImGui::GetIO();
            io.MouseDrawCursor = false;
            ImGui::LoadIniSettingsFromDisk("Prax.ini");



            ImGui::GetIO().WantSaveIniSettings = true;
            // Set ini setting to save window positions
            ImGui::GetIO().IniFilename = "Prax.ini";
            ImGui::GetIO().IniSavingRate = 10.0f;

            if (ImGui::GetFont() == nullptr) {



                auto inter_medium = GET_RESOURCE(fonts_inter_Inter_Medium_ttf);
                auto inter_bold = GET_RESOURCE(fonts_inter_Inter_Bold_ttf);
                auto inter_light = GET_RESOURCE(fonts_inter_Inter_Light_ttf);
                auto inter_extralight = GET_RESOURCE(fonts_inter_Inter_ExtraLight_ttf);
                auto inter_regular = GET_RESOURCE(fonts_inter_Inter_Regular_ttf);
                auto inter_semiBold = GET_RESOURCE(fonts_inter_Inter_SemiBold_ttf);
                auto sarabun_light = GET_RESOURCE(fonts_inter_Sarabun_Light_ttf);
                auto jetbrains_mono_regular = GET_RESOURCE(fonts_jetbrains_mono_JetBrainsMono_Regular_ttf);
                auto greycliff_cf_bold = GET_RESOURCE(fonts_greycliff_cf_GreycliffCF_Bold_ttf);
                auto greycliff_cf_demibold = GET_RESOURCE(fonts_greycliff_cf_GreycliffCF_DemiBold_ttf);
                auto greycliff_cf_extraBoldOblique = GET_RESOURCE(fonts_greycliff_cf_GreycliffCF_ExtraBoldOblique_ttf);
                auto greycliff_cf_extraLightOblique = GET_RESOURCE(fonts_greycliff_cf_GreycliffCF_ExtraLightOblique_ttf);
                auto greycliff_cf_extraLight = GET_RESOURCE(fonts_greycliff_cf_GreycliffCF_ExtraLight_ttf);
                auto greycliff_cf_light = GET_RESOURCE(fonts_greycliff_cf_GreycliffCF_Light_ttf);
                auto greycliff_cf_lightOblique = GET_RESOURCE(fonts_greycliff_cf_GreycliffCF_LightOblique_ttf);
                auto greycliff_cf_medium = GET_RESOURCE(fonts_greycliff_cf_GreycliffCF_Medium_ttf);
                auto greycliff_cf_regular = GET_RESOURCE(fonts_greycliff_cf_GreycliffCF_Regular_ttf);
                auto greycliff_cf_thin = GET_RESOURCE(fonts_greycliff_cf_GreycliffCF_Thin_ttf);
                auto greycliff_cf_thinOblique = GET_RESOURCE(fonts_greycliff_cf_GreycliffCF_ThinOblique_ttf);
                auto icons_v1 = GET_RESOURCE(fonts_icons_icons_v1_ttf);


                // TODO: Unretard this

                // Normal
                Font::Fonts.emplace("sarabun_light", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(sarabun_light.data2(), sarabun_light.size(), 20));
                Font::Fonts.emplace("inter_demi_bold", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(inter_semiBold.data2(), inter_semiBold.size(), 15));
                Font::Fonts.emplace("jetbrains_mono", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(jetbrains_mono_regular.data2(), jetbrains_mono_regular.size(), 15));
                Font::Fonts.emplace("inter", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(inter_medium.data2(), inter_medium.size(), 15));
                Font::Fonts.emplace("inter_bold", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(inter_bold.data2(), inter_bold.size(), 15));
                Font::Fonts.emplace("inter_light", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(inter_light.data2(), inter_light.size(), 15));
                Font::Fonts.emplace("inter_extralight", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(inter_extralight.data2(), inter_extralight.size(), 15));
                Font::Fonts.emplace("inter", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(inter_regular.data2(), inter_regular.size(), 15));
                Font::Fonts.emplace("greycliff_bold", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(greycliff_cf_bold.data2(), greycliff_cf_bold.size(), 15));
                Font::Fonts.emplace("greycliff_demibold", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(greycliff_cf_demibold.data2(), greycliff_cf_demibold.size(), 15));
                Font::Fonts.emplace("greycliff_extra_bold_oblique", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(greycliff_cf_extraBoldOblique.data2(), greycliff_cf_extraBoldOblique.size(), 15));
                Font::Fonts.emplace("greycliff_extra_light_oblique", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(greycliff_cf_extraLightOblique.data2(), greycliff_cf_extraLightOblique.size(), 15));
                Font::Fonts.emplace("greycliff_extra_light", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(greycliff_cf_extraLight.data2(), greycliff_cf_extraLight.size(), 15));
                Font::Fonts.emplace("greycliff_light", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(greycliff_cf_light.data2(), greycliff_cf_light.size(), 15));
                Font::Fonts.emplace("greycliff_light_oblique", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(greycliff_cf_lightOblique.data2(), greycliff_cf_lightOblique.size(), 15));
                Font::Fonts.emplace("greycliff_medium", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(greycliff_cf_medium.data2(), greycliff_cf_medium.size(), 12));
                Font::Fonts.emplace("greycliff", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(greycliff_cf_regular.data2(), greycliff_cf_regular.size(), 15));
                Font::Fonts.emplace("greycliff_thin", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(greycliff_cf_thin.data2(), greycliff_cf_thin.size(), 15));
                Font::Fonts.emplace("greycliff_thin_oblique", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(greycliff_cf_thinOblique.data2(), greycliff_cf_thinOblique.size(), 15));
                Font::Fonts.emplace("icons_v1", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(icons_v1.data2(), icons_v1.size(), 15));

                // Large
                Font::Fonts.emplace("inter_demi_bold_large", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(inter_semiBold.data2(), inter_semiBold.size(), 40));
                Font::Fonts.emplace("jetbrains_mono_large", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(jetbrains_mono_regular.data2(), jetbrains_mono_regular.size(), 40));
                Font::Fonts.emplace("inter_large", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(inter_medium.data2(), inter_medium.size(), 40));
                Font::Fonts.emplace("inter_bold_large", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(inter_bold.data2(), inter_bold.size(), 40));
                Font::Fonts.emplace("inter_light_large", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(inter_light.data2(), inter_light.size(), 40));
                Font::Fonts.emplace("inter_extralight_large", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(inter_extralight.data2(), inter_extralight.size(), 40));
                Font::Fonts.emplace("inter_large_large", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(inter_regular.data2(), inter_regular.size(), 40));
                Font::Fonts.emplace("greycliff_bold_large", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(greycliff_cf_bold.data2(), greycliff_cf_bold.size(), 40));
                Font::Fonts.emplace("greycliff_demibold_large", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(greycliff_cf_demibold.data2(), greycliff_cf_demibold.size(), 40));
                Font::Fonts.emplace("greycliff_extra_bold_oblique_large", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(greycliff_cf_extraBoldOblique.data2(), greycliff_cf_extraBoldOblique.size(), 40));
                Font::Fonts.emplace("greycliff_extra_light_oblique_large", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(greycliff_cf_extraLightOblique.data2(), greycliff_cf_extraLightOblique.size(), 40));
                Font::Fonts.emplace("greycliff_extra_light_large", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(greycliff_cf_extraLight.data2(), greycliff_cf_extraLight.size(), 40));
                Font::Fonts.emplace("greycliff_light_large", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(greycliff_cf_light.data2(), greycliff_cf_light.size(), 40));
                Font::Fonts.emplace("greycliff_light_oblique_large", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(greycliff_cf_lightOblique.data2(), greycliff_cf_lightOblique.size(), 40));
                Font::Fonts.emplace("greycliff_medium_large", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(greycliff_cf_medium.data2(), greycliff_cf_medium.size(), 40));
                Font::Fonts.emplace("greycliff_large", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(greycliff_cf_regular.data2(), greycliff_cf_regular.size(), 40));
                Font::Fonts.emplace("greycliff_thin_large", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(greycliff_cf_thin.data2(), greycliff_cf_thin.size(), 40));
                Font::Fonts.emplace("greycliff_thin_oblique_large", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(greycliff_cf_thinOblique.data2(), greycliff_cf_thinOblique.size(), 40));
                Font::Fonts.emplace("icons_v1_large", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(icons_v1.data2(), icons_v1.size(), 40));

            }

        }

        ImGui_ImplWin32_Init(window);
        ImGui_ImplDX11_Init(pDevice, pContext);

        Render::PushDrawList(ImGui::GetBackgroundDrawList());

        Image::Init(pDevice);

        initImGui = true;
    }
}

void ImGuiHelper::NewFrame() {

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (Minecraft::ClientInstance->guiData != nullptr) {

        // Set window size
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = Minecraft::ClientInstance->guiData->resolution;

        if (!Minecraft::ClientInstance->levelRenderer) return;


        Math::Fov = Minecraft::ClientInstance->GetFOV();
        Math::Color = ImColor(255, 255, 255, 255);
        Math::DrawList = ImGui::GetBackgroundDrawList();
        Math::ScreenSize = Minecraft::ClientInstance->guiData->resolution;
        Math::Origin = Render::Transform.origin;
    }

}

void ImGuiHelper::OnRender() {

    Notifications::Render();

    if (Minecraft::ClientInstance && Minecraft::ClientInstance->GetScreenName() == "start_screen") {
        ImVec2 renderPosition = ImVec2(20, 20);

    }

    ColorUtil::OnRender();

}

void ImGuiHelper::EndFrame() {
    try
    {
        if (!ImGui::GetCurrentContext()->FontStack.empty()){
            ImGui::PopFont();
        }
    }
    catch (...)
    {
        Logger::Write("ImGui", "Failed to pop font");
    }

    ImGui::Render();
}

void ImGuiHelper::Shutdown() {
    if (initImGui) {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();


        initImGui = false;
    }
}


