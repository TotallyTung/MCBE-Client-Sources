#include "ClickGUIMod.h"
#include "pch.h"

ClickGUIMod::ClickGUIMod() : IModule(VK_INSERT, Category::VISUAL, "A GUI that displays every module") {
	shouldHide = true;
}

const char* ClickGUIMod::getModuleName() {
	return ("ClickGUI");
}

bool ClickGUIMod::allowAutoStart() {
	return false;
}

void ClickGUIMod::onEnable() {
	g_Data.getClientInstance()->releaseMouse();
	openAnim = -500;
	//if (!showHudEditor) hasOpenedGUI = true;
	hasOpenedGUI = true;
}

void ClickGUIMod::onPlayerTick(C_Player* plr) {
	if (openAnim < 27 && hasOpenedGUI) openAnim += (28 - openAnim) * 0.09f;
}

//void ClickGUIMod::onTick(C_GameMode* gm) {
//	shouldHide = true;
//}

void ClickGUIMod::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	auto interfaceCC = ColorUtil::interfaceColor(1);
	if (!hasOpenedGUI) DrawUtils::drawRectangle(vec4_t(0, 0, g_Data.getClientInstance()->getGuiData()->widthGame, g_Data.getClientInstance()->getGuiData()->heightGame), MC_Color(interfaceCC), 1.f, 2.f);
	if (GameData::canUseMoveKeys())
		g_Data.getClientInstance()->releaseMouse();
}

void ClickGUIMod::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (!hasOpenedGUI) DrawUtils::fillRectangle(vec4_t(0, 0, g_Data.getClientInstance()->getGuiData()->widthGame, g_Data.getClientInstance()->getGuiData()->heightGame), MC_Color(0, 0, 0), 0.3f);
}

void ClickGUIMod::onDisable() {
	g_Data.getClientInstance()->grabMouse();
	openAnimation = false;
	hasOpenedGUI = false;
	animation = 1;
}

void ClickGUIMod::onLoadConfig(void* conf) {
	IModule::onLoadConfig(conf);
	ClickGui::onLoadConfig(conf);
	HudEditor::onLoadSettings(conf);
}
void ClickGUIMod::onSaveConfig(void* conf) {
	IModule::onSaveConfig(conf);
	ClickGui::onSaveConfig(conf);
	HudEditor::onSaveSettings(conf);
}