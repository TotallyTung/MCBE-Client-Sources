#include "ModuleManager.h"
#include "../Client.h"
#include "Modules/Player/AutoRespawn.h"
#include "Modules/Movement/Jetpack.h"
#include "../ModuleManager/Modules/Exploit/Wolf.h"
#include "../ModuleManager/Modules/Misc/Clip.h"
#include "../ModuleManager/Modules/Utility/Developer.h"
#include "../ModuleManager/Modules/Combat/BreakerCrystal.h"
#include "../ModuleManager/Modules/Combat/TPAura.h"
ModuleManager::ModuleManager() {
	{ 
		moduleList.push_back(new Fly());
		moduleList.push_back(new Gui());
		moduleList.push_back(new HUD());
		moduleList.push_back(new Clip());
		moduleList.push_back(new Woof());
		moduleList.push_back(new Phase());
		moduleList.push_back(new Reach());
		moduleList.push_back(new Timer());
		moduleList.push_back(new Noclip());
        moduleList.push_back(new TpAura());
		moduleList.push_back(new Jetpack());
		moduleList.push_back(new SelfHit());
		moduleList.push_back(new Killaura());
		moduleList.push_back(new NameTags());
		moduleList.push_back(new NoPacket());
		moduleList.push_back(new NoRender());
		moduleList.push_back(new Surround());
		moduleList.push_back(new Velocity());
		moduleList.push_back(new ArrayList());
		moduleList.push_back(new NoHurtcam());
		moduleList.push_back(new Watermark());
		moduleList.push_back(new XPosition());
		moduleList.push_back(new ZPosition());
        moduleList.push_back(new Developer());
        moduleList.push_back(new AntiDesync());
		moduleList.push_back(new AutoSprint());
        moduleList.push_back(new BlockReach());
		moduleList.push_back(new Fullbright());
		moduleList.push_back(new PacketMine());
        moduleList.push_back(new AutoCrystal());
		moduleList.push_back(new AutoOffhand());
		moduleList.push_back(new AutoRespawn());
		moduleList.push_back(new CameraNoClip());
		moduleList.push_back(new CrystalChams());
		moduleList.push_back(new ClientManager());
		moduleList.push_back(new Notifications());
		moduleList.push_back(new PacketSpoofer());
        moduleList.push_back(new CrystalBreaker1());
		moduleList.push_back(new VerticalPosition());
	}
	std::sort(moduleList.begin(), moduleList.end(), [](Module* lhs, Module* rhs) {
		return lhs->getModuleName() < rhs->getModuleName();
	});
    getModule("Gui")->setEnabled(false);
    getModule("Clip")->setEnabled(false);
    getModule("PacketMine")->setEnabled(false);
	getModule("Notifications")->setEnabled(true);
	this->initialized = true;
}

ModuleManager::~ModuleManager() {
    for (Module* mod : moduleList) {
        if (mod) {
            mod->setEnabled(false);
            delete mod;
        }
    }
    moduleList.clear();
}

void ModuleManager::onSaveConfig(json* currentConfig) {
    Module* module = getModule("Gui");
    if (module) {
        if (Gui* gui = dynamic_cast<Gui*>(module)) {
            gui->setEnabled(false);
        }
    }
    for (Module* mod : moduleList) {
        if (mod) {
            mod->onSaveConfig(currentConfig);
        }
    }
}
void ModuleManager::onNormalTick(Actor* actor) {
    if (!client || !client->isInitialized()) return;
    for (Module* mod : moduleList) {
        if (mod && mod->isEnabled()) {
            mod->onNormalTick(actor);
        }
    }
}

void ModuleManager::onContainerScreenControllerTickEvent(ContainerScreenControllerTickEvent* events) {
    if (!client || !client->isInitialized()) return;
    for (Module* mod : moduleList) {
        if (mod && mod->isEnabled()) {
            mod->onContainerScreenControllerTickEvent(events);
        }
    }
}

void ModuleManager::onSendPacket(Packet* packet, bool& shouldCancel) {
    if (!client || !client->isInitialized()) return;
    for (Module* mod : moduleList) {
        if (mod && mod->isEnabled()) {
            mod->onSendPacket(packet, shouldCancel);
            if (shouldCancel) break;
        }
    }
}

void ModuleManager::onLevelRender() {
    if (!client || !client->isInitialized()) return;
    for (Module* mod : moduleList) {
        if (mod && mod->isEnabled()) {
            mod->onLevelRender();
        }
    }
}

void ModuleManager::onLoadConfig(json* config) {
    for (Module* mod : moduleList) {
        if (mod) {
            mod->onLoadConfig(config);
        }
    }
    Module* module = getModule("Gui");
    if (module) {
        if (Gui* gui = dynamic_cast<Gui*>(module)) {
            gui->setEnabled(false);
        }
    }
}

void ModuleManager::onKeyUpdate(int key, bool isDown) {
    if (!client || !client->isInitialized()) return;
    for (Module* mod : moduleList) {
        if (mod) {
            mod->onKeyUpdate(key, isDown);
        }
    }
}

void ModuleManager::onRender(MinecraftUIRenderContext* ctx) {
    if (!client || !client->isInitialized()) return;
    for (Module* mod : moduleList) {
        if (mod && mod->isEnabled()) {
            mod->onRender(ctx);
        }
    }
}

void ModuleManager::onImGuiRender(ImDrawList* drawlist) {
    if (!client || !client->isInitialized()) return;
    for (Module* mod : moduleList) {
        if (mod && mod->isEnabled()) {
            mod->onImGuiRender(drawlist);
        }
    }
}
