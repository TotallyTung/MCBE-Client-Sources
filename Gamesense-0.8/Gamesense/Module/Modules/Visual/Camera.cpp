#include "Camera.h"

Camera::Camera() : IModule(0, Category::VISUAL, "Modifies ur camera") {
	registerBoolSetting("NoHurtcam", &nohurtcam, nohurtcam);
	registerBoolSetting("Fullbright", &fullbright, fullbright);
	registerBoolSetting("Tinted", &tint, tint);
	registerBoolSetting("Sync Tint", &synced, synced);
	registerIntSetting("Intensity", &intensity, intensity, -25, 25);
	registerFloatSetting("R", &red, red, 0.f, 255.f);
	registerFloatSetting("G", &green, green, 0.f, 255.f);
	registerFloatSetting("B", &blue, blue, 0.f, 255.f);
	registerFloatSetting("Tint Opacity", &tintopacity, tintopacity, 0.f, 255.f);
}

const char* Camera::getRawModuleName() {
	return "Camera";
}

float originalGamma = -1;

void Camera::onEnable() {
	if (fullbright) {
		if (gammaPtr != nullptr) {
			originalGamma = *gammaPtr;
			*gammaPtr = 10;
		}
	}
}

void Camera::onTick(C_GameMode* gm) {
	if (fullbright) {
		if (gammaPtr != nullptr && *gammaPtr != 10)
			*gammaPtr = 10;
	}
}

void Camera::onWorldTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	if (nohurtcam) g_Data.getLocalPlayer()->cancelHurtAnimation();
}
void Camera::onPreRender(C_MinecraftUIRenderContext* renderCtx)
{
	auto clickGuiMod = moduleMgr->getModule<ClickGUIMod>();
	auto interfacrColor = ColorUtil::interfaceColor(1);
	if (!clickGuiMod->isEnabled() && !synced && tint) {
		DrawUtils::fillRectangle(vec4_t(0, 0, g_Data.getClientInstance()->getGuiData()->widthGame, g_Data.getClientInstance()->getGuiData()->heightGame),
			MC_Color(red, green, blue), tintopacity / 255);
	}
	if (!clickGuiMod->isEnabled() && synced && tint) {
		DrawUtils::fillRectangle(vec4_t(0, 0, g_Data.getClientInstance()->getGuiData()->widthGame, g_Data.getClientInstance()->getGuiData()->heightGame),
			MC_Color(interfacrColor), tintopacity / 255);
	}
}
void Camera::onDisable() {
	if (fullbright) {
		if (gammaPtr != nullptr) {
			if (originalGamma >= 0 && originalGamma <= 1)
				*gammaPtr = originalGamma;
			else
				*gammaPtr = 0.5f;
		}
	}
}