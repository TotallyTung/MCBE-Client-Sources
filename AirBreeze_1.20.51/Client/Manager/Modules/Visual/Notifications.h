#pragma once

#include "../../Module.h"

class NotificationsModule : public Module {
public:
    NotificationsModule(Category* c) : Module(c, "Notifications", "Notifications") {}
    ~NotificationsModule() = default;

    void onImGuiRender() override;
};