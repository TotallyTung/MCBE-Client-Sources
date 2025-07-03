#pragma once
//
// Created by vastrakai on 10/19/2024.
//

#include <Hook/Hook.hpp>

class RenderItemInHandDescription
{
public:
    CLASS_FIELD(glm::vec3, mGlintColor, 0x8C);
    CLASS_FIELD(float, mGlintAlpha, 0x9C);
    CLASS_FIELD(glm::vec3, mOverlayColor, 0xAC);
    CLASS_FIELD(float, mOverlayAlpha, 0xBC);
    CLASS_FIELD(glm::vec3, mChangeColor, 0xCC);
    CLASS_FIELD(float, mChangeAlpha, 0xDC);
    CLASS_FIELD(glm::vec3, mMultiplicativeTintColor, 0xEC);
    CLASS_FIELD(float, mMultiplicativeTintColorAlpha, 0xFC);
};

class RenderItemInHandHook : public Hook {
public:
    RenderItemInHandHook() : Hook() {
        mName = "RenderItemInHandHook";
    }

    static std::unique_ptr<Detour> mDetour;
    //void __fastcall mce::framebuilder::RenderItemInHandDescription::RenderItemInHandDescription( mce::framebuilder::RenderItemInHandDescription *this, const ItemRenderCall *renderObject, ItemContextFlags *itemFlags, const mce::MaterialPtr *material, const mce::ServerTexture *glintTexture, const glm::tmat4x4<float,0> *worldMatrix, bool isDrawingUI, const mce::GlobalConstantBuffers *globalConstantBuffers, unsigned __int16 viewId, const dragon::RenderMetadata *renderMetadata)
    static void* RenderItemInHandDescriptionCtor(RenderItemInHandDescription* _this, void* renderObject, void* itemFlags, void* material, void* glintTexture, void* worldMatrix, bool isDrawingUI, void* globalConstantBuffers, unsigned __int16 viewId, void* renderMetadata);
    void init() override;
};