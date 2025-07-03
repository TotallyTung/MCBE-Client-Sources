#pragma once
#include "../Category.h"
#include <functional>

template<typename T>
class Event {
public:
    std::function<void(T*)> callback;
    const char* type;
};

class Events {
public:
    std::vector<__int64*> events;
public:
    template<typename T>
    auto registerEvent(std::function<void(T*)> fn) {
        auto ev = new Event<T>();

        ev->type = typeid(T).name();
        ev->callback = fn;

        this->events.push_back((__int64*)ev);
    };
public:
    template<typename T>
    auto callEvent(T args) {
        for (auto ptr : this->events) {
            auto ev = (Event<T>*)ptr;

            if (typeid(T).name() == ev->type)
                ev->callback(&args);
        };
    };
};

struct KeyEvent {
    uint64_t key;
    bool isDown;
    bool* cancel;
};

struct MouseEvent {
    bool isDown;
    bool* cancel;
};

struct ImGuiEvent {

};

struct ClickGuiModConfigEvent {
    //
};

struct PacketEvent {
    bool* cancel;
    class Packet* packet;
};

struct GameModeTickEvent {
    class GameMode* GM;
};