//
// Created by vastrakai on 9/23/2024.
//

#include "ClientInstanceLib.hpp"

#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/KeyboardMouseSettings.hpp>
#include <SDK/Minecraft/MinecraftSim.hpp>
#include <SDK/Minecraft/World/BlockLegacy.hpp>

void ClientInstanceLib::initialize(lua_State* state)
{
    getGlobalNamespace(state)
        .beginClass<ClientInstance>("ClientInstance")
        .addStaticFunction("get", &ClientInstance::get)
        .addFunction("getLocalPlayer", &ClientInstance::getLocalPlayer)
        .addFunction("getMinecraftSim", &ClientInstance::getMinecraftSim)
        .addFunction("getLevelRenderer", &ClientInstance::getLevelRenderer)
        .addFunction("getPacketSender", &ClientInstance::getPacketSender)
        .addFunction("getGuiData", &ClientInstance::getGuiData)
        .addFunction("getBGFX", &ClientInstance::getBGFX)
        .addFunction("getViewMatrix", &ClientInstance::getViewMatrix)
        .addFunction("getFov", &ClientInstance::getFov)
        .addFunction("getMinecraftGame", &ClientInstance::getMinecraftGame)
        .addFunction("getBlockSource", &ClientInstance::getBlockSource)
        .addFunction("getOptions", &ClientInstance::getOptions)
        .addFunction("getScreenName", &ClientInstance::getScreenName)
        .addFunction("setDisableInput", &ClientInstance::setDisableInput)
        .addFunction("getMouseGrabbed", &ClientInstance::getMouseGrabbed)
        .addFunction("grabMouse", &ClientInstance::grabMouse)
        .addFunction("releaseMouse", &ClientInstance::releaseMouse)
        .addFunction("playUi", &ClientInstance::playUi)
        .addFunction("getKeyboardSettings", &ClientInstance::getKeyboardSettings)
        .endClass()
    /*
class MinecraftBind
{
public:
    std::string mBindName;
    std::vector<int> mBindKey;
    PAD(0x8);
};


class KeyboardMouseSettings {
public:
    PAD(0x8);
    std::vector<MinecraftBind> mKeyTypeA;
    std::vector<MinecraftBind> mKeyTypeB;

    int operator[](const std::string& key);
};*/
        .beginClass<MinecraftBind>("MinecraftBind")
        .addProperty("mBindName", &MinecraftBind::mBindName)
        .addProperty("mBindKey", &MinecraftBind::mBindKey)
        .endClass()
        .beginClass<KeyboardMouseSettings>("KeyboardMouseSettings")
        .addProperty("mKeyTypeA", &KeyboardMouseSettings::mKeyTypeA)
        .addProperty("mKeyTypeB", &KeyboardMouseSettings::mKeyTypeB)
        .addFunction("getBind", [](KeyboardMouseSettings* settings, const std::string& key) {
            return settings->operator[](key);
        })
        .endClass()
        .beginClass<MinecraftSim>("MinecraftSim")
        .addFunction("getGameSim", &MinecraftSim::getGameSim)
        .addFunction("getRenderSim", &MinecraftSim::getRenderSim)
        .addFunction("setSimTimer", &MinecraftSim::setSimTimer)
        .addFunction("setSimSpeed", &MinecraftSim::setSimSpeed)
        .addFunction("getSimTimer", &MinecraftSim::getSimTimer)
        .addFunction("getSimSpeed", &MinecraftSim::getSimSpeed)
        .addFunction("getGameSession", &MinecraftSim::getGameSession)
        .endClass()
        .beginClass<Simulation>("Simulation")
        .addProperty("mTimer", &Simulation::mTimer)
        .addProperty("mDeltaTime", &Simulation::mDeltaTime)
        .addProperty("mTimerMultiplier", &Simulation::mTimerMultiplier)
        .addProperty("mTime", &Simulation::mTime)
        .endClass()
        .beginClass<PacketUtils>("PacketUtils")
        .addStaticFunction("spoofSlot", &PacketUtils::spoofSlot)
        .addStaticFunction("createMobEquipmentPacket", &PacketUtils::createMobEquipmentPacket)
        .addStaticFunction("sendChatMessage", &PacketUtils::sendChatMessage)
        .endClass()
        .beginClass<CommandUtils>("CommandUtils")
        .addStaticFunction("executeCommand", &CommandUtils::executeCommand)
        .endClass()
    ;
}
