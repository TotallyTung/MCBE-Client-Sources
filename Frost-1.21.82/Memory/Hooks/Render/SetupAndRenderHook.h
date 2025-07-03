#pragma once

#include "../../SDK/Render/MinecraftUIRenderContext.h"
#include "../../SDK/Render/Controls/HoverRenderer.h"
#include "../../SDK/Render/Layer/UILayer.h"

void* onRender;
float mAnim = 0;
__int64* onDrawText;
__int64* onDrawImage;
__int64* onDrawNineSlice;

void onDrawTextDetour(MinecraftUIRenderContext* _this, FontRepos* font, Vector4<float> const& pos, std::string* str, Color const& colour, float alpha, float alinM, TextMeasureData const& textdata, CaretMeasureData const& caretdata) 
{
    std::string mMarketPlace = "The Hive";
    std::string mTruth = TextFormat::getColorCode(TextFormat::Color::WHITE) + "Stinky Anticheat";

    if (str->find(mMarketPlace) != std::string::npos) {
        str = &mTruth;
    }
    
    Memory::CallFunc<void, MinecraftUIRenderContext*, FontRepos*, Vector4<float> const&, std::string*, Color const&, float, float, TextMeasureData const&, CaretMeasureData const&>(
        onDrawText, _this, font, pos, str, colour, alpha, alinM, textdata, caretdata);
}

void onDrawImageDetour(MinecraftUIRenderContext* ctx, BedrockTextureData* path, Vector2<float>& ImagePos, Vector2<float>& a3, Vector2<float>& a4, Vector2<float>& a5, mce::mColor* color, bool mDefaultTransformRotation) 
{

    Memory::CallFunc<void*, MinecraftUIRenderContext*, BedrockTextureData*, Vector2<float>&, Vector2<float>&, Vector2<float>&, Vector2<float>&, mce::mColor*, bool>(
        onDrawImage, ctx, path, ImagePos, a3, a4, a5, color, mDefaultTransformRotation);
}

void onDrawNineSliceDetour(MinecraftUIRenderContext* _this, mce::TexturePtr* texturePtr, NinesliceInfo* nineSliceInfo) 
{

    Memory::CallFunc<void*, MinecraftUIRenderContext*, mce::TexturePtr*, NinesliceInfo*>(
        onDrawNineSlice, _this, texturePtr, nineSliceInfo);
}

void onScreenViewDetour(ScreenView* _this, MinecraftUIRenderContext* ctx) {
    auto VTable = *(uintptr_t**)ctx;

    InstanceManager::set<ScreenView>(_this);
    InstanceManager::set<MinecraftUIRenderContext>(ctx);
    InstanceManager::set<ClientInstance>(ctx->mClientInstance);
    InstanceManager::set<Tessellator>(ctx->mScreenContext->getTessellator());

    RenderUtils::initialize();

    ClientInstance* mClientInstance = InstanceManager::get<ClientInstance>();
    Player* mPlayer = mClientInstance->getLocalPlayer();
    GuiData* mGuiData = mClientInstance->getGuiData();

    Vector3<float> mOrigin = Vector3<float>(0, 0, 0);
    Vector3<float> mPlayerPos = Vector3<float>(0, 0, 0);

    if (mPlayer != nullptr && mClientInstance->getLevelRender() != nullptr)
    {
        mOrigin = mClientInstance->getLevelRender()->getOrigin();
        mPlayerPos = mPlayer->getComponent<RenderPositionComponent>()->mRenderPos;
    }

    FrameUtil::mFrameTransforms.push({ mClientInstance->getGLMatrix(), mOrigin, mPlayerPos, mClientInstance->getFov() });

    if (!UILayer::Is(_this, UILayer::Ingame_InventoryScreen))
    {
        if (getModuleByName("testmodule")->isEnabled()) 
        {
            
        }
    }

    static bool mHookedVtables = false;

    if (!mHookedVtables) {
        Memory::HookFunction((void*)VTable[OffsetManager::MinecraftUIRenderContext_drawText], (void*)&onDrawTextDetour, &onDrawText, "MinecraftUIRenderContext::drawText");
        Memory::HookFunction((void*)VTable[OffsetManager::MinecraftUIRenderContext_drawImage], (void*)&onDrawImageDetour, &onDrawImage, "MinecraftUIRenderContext::drawImage");
        Memory::HookFunction((void*)VTable[OffsetManager::MinecraftUIRenderContext_drawNineSlice], (void*)&onDrawNineSliceDetour, &onDrawNineSlice, "MinecraftUIRenderContext::drawNineSlice");
        mHookedVtables = true;
    }

    static int mLayers = 0;

    if (mLayers == 3) 
    {
        //ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(0, 0), ImVec2(200, 200), 0xFF0000FF);
        mLayers = 0;
    }
    else
    {
        mLayers += 1;
    }

    RenderContextEvent event{};
    EventManager::registerEvent(&event);

    Memory::CallFunc<void*, ScreenView*, MinecraftUIRenderContext*>(
        onRender, _this, ctx);
}

class SetupAndRenderHook : public FuncHook 
{
public:
    bool Initialize() override
    {
        Memory::HookFunction(SigManager::ScreenView_setupAndRender, (void*)&onScreenViewDetour, &onRender, "ScreenView::setupAndRender");

        return true;
    }
};