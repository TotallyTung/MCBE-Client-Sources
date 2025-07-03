#pragma once

enum EventType
{
    LerpMotion,
    ViewBobTick,
    ContainerTick,
    GetPickRange,
    _Keyboard,
    ActorTick,
    Mouse,
    MouseScroll,
    Update,
    LayerUpdate,
    BeforeUpdate,
    ImGUIRender,
    Hurtcam,
    PlaceBlock,
    ContinuePlaceBlock,
    StopPlaceBlock,
    GameModeTick,
    PacketSending,
    UpdateTick,
    PreUpdateTick,
};

class Event
{
public:
    virtual EventType getType() const = 0;
    bool* cancelled;
};

#include "Events/ImGUIRenderEvent.h"
#include "Events/KeyboardEvent.h"
#include "Events/UpdateEvent.h"
#include "Events/PacketSendingEvent.h"

//Module* funnehmodule;

// event utils
template<typename EventT>
void DispatchEvent(EventT* event)
{
    for (auto module : modules)
    {
        if (Game::isRunning)
        {
            if (module->isEnabled() or module->getCallWhenDisabled())
            {
                module->onEvent(event);
            }
        }
    }
}