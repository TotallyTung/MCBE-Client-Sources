#pragma once

enum EventType
{
    BaseTick,
    ActorSetMotion,
    ChangeDimension,
    ContainerTick,
    ImGuiRender,
    Keyboard,
    LevelEventPkt,
    Mouse,
    MouseScroll,
    PacketSendToServer,
    RenderContext,
    RunUpdateCycle,
    BobHurt,
    UpdateBlock
};

class Event
{
public:
    virtual EventType getType() const = 0;
    bool* mCancelled = nullptr;
};

#include "Events/BaseTickEvent.hpp"
#include "Events/ActorSetMotionEvent.hpp"
#include "Events/ChangeDimensionEvent.hpp"
#include "Events/ContainerTickEvent.hpp"
#include "Events/ImRenderEvent.hpp"
#include "Events/KeyboardEvent.hpp"
#include "Events/LevelEventPacketEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Events/MouseScrollEvent.hpp"
#include "Events/PacketEvent.hpp"
#include "Events/RenderContextEvent.hpp"
#include "Events/RunUpdateCycleEvent.hpp"
#include "Events/BobHurtEvent.hpp"
#include "Events/UpdateBlockEvent.hpp"

namespace EventManager
{
    template<typename EventT>
    void registerEvent(EventT* event)
    {
        for (const auto& module : mModules)
        {
            if (MainClient::g_isRunning)
            {
                if (module->isEnabled() || module->mCallEventsWhenDisabled)
                {
                    module->onEvent(event);
                }
            }
        }
    }
}