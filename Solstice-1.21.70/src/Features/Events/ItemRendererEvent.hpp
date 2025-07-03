#pragma once
//
// Created by jcazm on 7/27/2024.
//

#include "Event.hpp"

class ItemRendererEvent : public CancelableEvent {
public:
    class ItemRenderer *mRenderer;
    class BaseActorRenderContext *mRenderContext;
    class ActorRenderData *mActorRenderData;

    explicit ItemRendererEvent(ItemRenderer *_this, BaseActorRenderContext *renderContext, ActorRenderData *actorRenderData) : CancelableEvent(), mRenderer(_this), mRenderContext(renderContext), mActorRenderData(actorRenderData){};
};