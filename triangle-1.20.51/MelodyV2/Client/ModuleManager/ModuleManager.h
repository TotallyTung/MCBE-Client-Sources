#pragma once
#include <optional>
#include <memory>
#include <vector>
#include <algorithm>
#include "Modules/Module.h"
//Combat
#include "Modules/Combat/Killaura.h"
#include "Modules/Combat/AutoCrystal.h"
#include "Modules/Combat/Surround.h"
#include "Modules/Combat/Reach.h"
//Misc
#include "Modules/Misc/NoPacket.h"
#include "Modules/Misc/Timer.h"
//Render
#include "Modules/Render/CameraNoClip.h"
#include "Modules/Render/Fullbright.h"
#include "Modules/Render/NameTags.h"
#include "Modules/Render/NoHurtcam.h"
#include "Modules/Render/NoRender.h"
#include "Modules/Render/CrystalChams.h"
//Movement
#include "Modules/Movement/AutoSprint.h"
#include "Modules/Movement/Phase.h"
#include "Modules/Movement/Velocity.h"
#include "Modules/Movement/Fly.h"
#include "Modules/Movement/Noclip.h"
//Player
#include "Modules/Player/PacketMine.h"
#include "Modules/Player/AutoOffhand.h"
//Client
#include "Modules/Client/ArrayList.h"
#include "Modules/Client/ClickGui.h"
//#include "Modules/Client/Colors.h" not used
#include "Modules/Client/Watermark.h"
#include "Modules/Client/CustomFont.h"
#include "Modules/Client/HUD.h"
#include "Modules/Client/Notifications.h"
#include "Modules/Client/Clients.h"
//World
#include "Modules/World/BlockReach.h"
//Exploit
#include "Modules/Exploit/SelfHit.h"
//Utility
#include "Modules/Exploit/AntiDesync.h"
//Packets
#include "Modules/PacketEditor/VerticalPosition.h"
#include "Modules/PacketEditor/XPosition.h"
#include "Modules/PacketEditor/PacketSpoofer.h"
#include "Modules/PacketEditor/ZPosition.h"
class ModuleManager {
private:
	std::vector<Module*> moduleList;
    bool initialized = false;
public:
	ModuleManager();
    ~ModuleManager();
public:
    inline std::vector<Module*>* getModuleList() { return &this->moduleList; }
    inline const bool isInitialized() { return this->initialized; }

	Module* getModule(const std::string& moduleName) {
        std::string nameLower(moduleName);
        std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);

        for (Module* mod : moduleList) {
            std::string modNameLower = mod->getModuleName();
            std::transform(modNameLower.begin(), modNameLower.end(), modNameLower.begin(), ::tolower);
            if (strcmp(nameLower.c_str(), modNameLower.c_str()) == 0) return mod;
        }
        return nullptr;
	}
    std::optional<Module*> getModuleByName(const std::string& name) {
        std::string nameLower = name;
        std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);

        for (Module* mod : moduleList) {
            std::string modNameLower = mod->getModuleName();
            std::transform(modNameLower.begin(), modNameLower.end(), modNameLower.begin(), ::tolower);
            if (modNameLower == nameLower) {
                return mod;
            }
        }

        return std::nullopt;
    }
public:
    void onSaveConfig(json* currentConfig);
    void onLoadConfig(json* config);
    void onKeyUpdate(int key, bool isDown);
    void onRender(MinecraftUIRenderContext* ctx);
    void onImGuiRender(ImDrawList* drawlist);
    void onNormalTick(Actor* actor);
    void onContainerScreenControllerTickEvent(ContainerScreenControllerTickEvent* events);
    void onSendPacket(Packet* packet, bool& shouldCancel);
    void onLevelRender();
};
