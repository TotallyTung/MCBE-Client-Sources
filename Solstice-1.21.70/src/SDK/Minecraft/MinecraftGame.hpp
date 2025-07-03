#pragma once
#include <string>
//
// Created by vastrakai on 6/24/2024.
//


class MinecraftGame {
public:
    static MinecraftGame* getInstance();
    class ClientInstance* getPrimaryClientInstance();
    class UIProfanityContext* getProfanityContext();
    bool getMouseGrabbed();
    void setMouseGrabbed(bool grabbed);
    void playUi(const std::string& soundName, float volume, float pitch);
};