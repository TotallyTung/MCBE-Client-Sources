#pragma once
//
// Created by jcazm on 7/27/2024.
//
#include <Hook/Hook.hpp>


class ItemRendererHook : public Hook {
public:
    ItemRendererHook() : Hook() {
        mName = "ItemRenderer::render";
    }

    static std::unique_ptr<Detour> mDetour;
    // symbol: void ItemRenderer::render(ItemRenderer* _this, BaseActorRenderContext* renderContext, ActorRenderData* actorRenderData)
    static void render(class ItemRenderer* _this, class BaseActorRenderContext* renderContext, struct ActorRenderData* actorRenderData);
    void init() override;
};
