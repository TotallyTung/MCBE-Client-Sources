#include "ModuleManager.h"
#include "../../Client.h"
//#include "Modules/Category/Render/Tracer.h"
void ModuleManager::init() {
	moduleList.push_back(new AutoClicker());
	moduleList.push_back(new AutoAnvil());
	moduleList.push_back(new Reach());
	moduleList.push_back(new BowSpam());
	moduleList.push_back(new Hitbox());
	moduleList.push_back(new AutoCrystal());
	moduleList.push_back(new CustomFont());
	moduleList.push_back(new ClickGUI());
	moduleList.push_back(new Aura());
	moduleList.push_back(new Hud());
	moduleList.push_back(new HUD());
	std::sort(moduleList.begin(), moduleList.end(), [](Module* lhs, Module* rhs) {
		return !(lhs->getModuleName() < rhs->getModuleName());
		});
	getModule<ClickGUI>()->InitClickGUI();
}

void ModuleManager::shutdown() {
	for (auto& mod : moduleList) {
		mod->setEnabled(false);
	}
	for (auto& mod : moduleList) {
		delete mod;
		mod = nullptr;
	}
	moduleList.clear();
}

void ModuleManager::onKeyUpdate(int key, bool isDown) {
	if (!Client::isInitialized())
		return;

	for (auto& mod : moduleList) {
		mod->onKeyUpdate(key, isDown);
	}
}

void ModuleManager::onPlayerTick() {
	if (!Client::isInitialized())
		return;

	for (auto& mod : moduleList) {
		if (mod->isEnabled() || mod->runOnBackground()) {
			mod->onPlayerTick();
		}
	}
}

void ModuleManager::onPlayerTick(Packet* packet) {
	if (!Client::isInitialized())
		return;

	for (auto& mod : moduleList) {
		if (mod->isEnabled() || mod->runOnBackground()) {
			mod->onSendPacket(packet);
		}
	}
}

void ModuleManager::onMatrixRender(glm::mat4* matrix) {
	if (!Client::isInitialized())
		return;

	for (auto& mod : moduleList) {
		if (mod->isEnabled() || mod->runOnBackground()) {
			mod->onMatrixRender(matrix);
		}
	}
}

void ModuleManager::onNormalTick(LocalPlayer* localPlayer) {
	if (!Client::isInitialized())
		return;

	for (auto& mod : moduleList) {
		if (mod->isEnabled() || mod->runOnBackground()) {
			mod->onNormalTick(localPlayer);
		}
	}
}

void ModuleManager::onWorldTick(Level* level) {
	if (!Client::isInitialized())
		return;

	for (auto& mod : moduleList) {
		if (mod->isEnabled() || mod->runOnBackground()) {
			mod->onWorldTick(level);
		}
	}
}

void ModuleManager::onUpdateRotation(LocalPlayer* localPlayer) {
	if (!Client::isInitialized())
		return;

	for (auto& mod : moduleList) {
		if (mod->isEnabled() || mod->runOnBackground()) {
			mod->onUpdateRotation(localPlayer);
		}
	}
}

bool ModuleManager::onSendPacket(Packet* packet) {
	if (!Client::isInitialized())
		return false;
	bool cancel = false;
	for (auto& mod : moduleList) {
		if (mod->isEnabled() || mod->runOnBackground()) {
			if (mod->onSendPacket(packet)) {
				cancel = true;
			}
		}
	}
	return cancel;
}

bool ModuleManager::onSendDelayedPacket(Packet* packet) {
	if (!Client::isInitialized())
		return false;
	bool cancel = false;
	for (auto& mod : moduleList) {
		if (mod->isEnabled() || mod->runOnBackground()) {
			if (mod->onSendPacket(packet)) {
				cancel = true;
			}
		}
	}
	return cancel;
}

void ModuleManager::onD2DRender() {
	if (!Client::isInitialized())
		return;

	for (auto& mod : moduleList) {
		if (mod->isEnabled() || mod->runOnBackground()) {
			mod->onD2DRender();
		}
	}
}

void ModuleManager::onMCRender(MinecraftUIRenderContext* renderCtx) {
	if (!Client::isInitialized())
		return;

	for (auto& mod : moduleList) {
		if (mod->isEnabled() || mod->runOnBackground()) {
			mod->onMCRender(renderCtx);
		}
	}
}

void ModuleManager::onLevelRender() {
	if (!Client::isInitialized())
		return;

	for (auto& mod : moduleList) {
		if (mod->isEnabled() || mod->runOnBackground()) {
			mod->onLevelRender();
		}
	}
}

void ModuleManager::onLoadConfig(void* conf) {
	if (!Client::isInitialized())
		return;

	for (auto& mod : moduleList) {
		mod->onLoadConfig(conf);
	}
}

void ModuleManager::onSaveConfig(void* conf) {
	if (!Client::isInitialized())
		return;

	for (auto& mod : moduleList) {
		mod->onSaveConfig(conf);
	}
}