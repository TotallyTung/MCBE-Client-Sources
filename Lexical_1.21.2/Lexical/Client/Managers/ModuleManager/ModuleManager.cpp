#include "ModuleManager.h"
#include "../../Client.h"

void ModuleManager::init() {

	//COMBAT
	moduleList.push_back(new BowSpam());
	moduleList.push_back(new Criticals());
	moduleList.push_back(new AutoCrystal());
	moduleList.push_back(new Hitbox());
	moduleList.push_back(new Reach());
	moduleList.push_back(new Surround());
	moduleList.push_back(new KillAura());

	//MOVEMENT
	moduleList.push_back(new AirJump());
	moduleList.push_back(new AutoSprint());
	moduleList.push_back(new Fly());
	moduleList.push_back(new Jetpack());
	moduleList.push_back(new NoSlowDown());
	moduleList.push_back(new Phase());
	moduleList.push_back(new Step());
	moduleList.push_back(new Speed());
	moduleList.push_back(new ElytraFly()); 
	moduleList.push_back(new NoClip());
	moduleList.push_back(new ReverseStep());
	moduleList.push_back(new Spider());
	moduleList.push_back(new Velocity());

	//RENDER
	moduleList.push_back(new BlockHighlight());
	moduleList.push_back(new BreakIndicators());
	moduleList.push_back(new CameraTweaks());
	moduleList.push_back(new ChunkBorders());
	moduleList.push_back(new CustomFov());
	moduleList.push_back(new ESP());
	//moduleList.push_back(new HurtColor()); i can fix htis diamond trust
	moduleList.push_back(new Fullbright());
	moduleList.push_back(new GlintColor());
	moduleList.push_back(new NoHurtCam());
	moduleList.push_back(new NameTags());
	moduleList.push_back(new NoRender);
	moduleList.push_back(new Swing());
	moduleList.push_back(new Tracer());

	//PLAYER
	moduleList.push_back(new AntiBot());
	moduleList.push_back(new AutoTool());
	moduleList.push_back(new BlockReach());
	moduleList.push_back(new FastEat());
	moduleList.push_back(new FastUse());
	moduleList.push_back(new NoSwing());
	moduleList.push_back(new Offhand());

	//WORLD
	moduleList.push_back(new AirPlace());

	//MISC
	moduleList.push_back(new Disabler());
	moduleList.push_back(new NoPacket());
	moduleList.push_back(new Timer());
	moduleList.push_back(new PopCounter());
	moduleList.push_back(new Spammer());

	//CLIENT
	moduleList.push_back(new ClickGUI());
	moduleList.push_back(new Arraylist());
	moduleList.push_back(new NotificationsModule());
	moduleList.push_back(new HUD());
	moduleList.push_back(new Colors());
	moduleList.push_back(new CustomFont());

	std::sort(moduleList.begin(), moduleList.end(), [](Module* lhs, Module* rhs) {
		return lhs->getModuleName() < rhs->getModuleName();
	});

	getModule<ClickGUI>()->InitClickGUI();

	getModule<Arraylist>()->setEnabled(true);
	getModule<HUD>()->setEnabled(true);
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

void ModuleManager::onClientTick() {
	if (!Client::isInitialized())
		return;

	for (auto& mod : moduleList) {
		if (mod->isEnabled() || mod->runOnBackground()) {
			mod->onClientTick();
		}
	}
}

void ModuleManager::onClientTick(Packet* packet) {
	if (!Client::isInitialized())
		return;

	for (auto& mod : moduleList) {
		if (mod->isEnabled() || mod->runOnBackground()) {
			mod->onSendPacket(packet);
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

void ModuleManager::onLevelTick(Level* level) {
	if (!Client::isInitialized())
		return;

	for (auto& mod : moduleList) {
		if (mod->isEnabled() || mod->runOnBackground()) {
			mod->onLevelTick(level);
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

void ModuleManager::onSendPacket(Packet* packet) {
	if (!Client::isInitialized())
		return;

	for (auto& mod : moduleList) {
		if (mod->isEnabled() || mod->runOnBackground()) {
			mod->onSendPacket(packet);
		}
	}
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