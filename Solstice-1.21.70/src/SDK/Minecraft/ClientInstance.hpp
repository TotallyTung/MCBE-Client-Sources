#pragma once

#include <Utils/MemUtils.hpp>

#include <Utils/Structs.hpp>
//
// Created by vastrakai on 6/24/2024.
//



class ClientInstance {
public:
    //CLASS_FIELD(class MinecraftGame*, mcGame, 0xC8);

    static ClientInstance* get();



    class MinecraftSim* getMinecraftSim();
    class LevelRenderer* getLevelRenderer();
    class LoopbackPacketSender* getPacketSender();
    class GuiData* getGuiData();
    class bgfx_context* getBGFX();
    glmatrixf getViewMatrix();
    glm::vec2 getFov();

    class ClientInputHandler* getInputHandler();
    class KeyboardMouseSettings* getKeyboardSettings();
    class MinecraftGame* getMinecraftGame();
    class Actor* getLocalPlayer();
    class BlockSource* getBlockSource();
    class Options* getOptions();
    std::string getScreenName();
    void setDisableInput(bool disable);
    bool getMouseGrabbed();
    void grabMouse();
    void releaseMouse();
    void playUi(const std::string& soundName, float volume, float pitch);
};