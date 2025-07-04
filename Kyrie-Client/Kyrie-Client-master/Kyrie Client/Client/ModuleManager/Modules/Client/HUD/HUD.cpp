#include "HUD.h"
#include "../../../../Client.h"

HUD::HUD() : Module(0x0, Category::CLIENT, "Display HUD") {
	registerBoolSetting("Position", &showPosition, showPosition);
	registerBoolSetting("FPS", &showFPS, showFPS);
}

const char* HUD::getModuleName() {
	return "HUD";
}

void HUD::onImGuiRender(ImDrawList* d) {
	static Colors* colorsMod = client->moduleMgr->getModule<Colors>();
	float textPadding = 1.f * 2.f;
	float textHeight = RenderUtils::getTextHeight("A", 1.f);
	float x = 0.f + textPadding;
	float y = ImGui::GetIO().DisplaySize.y - textHeight - textPadding;

	if (mc.getLocalPlayer() != nullptr) {
		if (showPosition) {
			Vec3<float>* pos = mc.getLocalPlayer()->getPosition();
			RenderUtils::drawText(d, Vec2<float>(x, y), "Pos:", colorsMod->getColor(), 1.f, true);
			std::string text2 = std::to_string((int)floorf(pos->x)) + std::string(" ") + std::to_string((int)floorf(pos->y)) + std::string(" ") + std::to_string((int)floorf(pos->z));
			RenderUtils::drawText(d, Vec2<float>(x + RenderUtils::getTextWidth("Pos: ", 1.f), y), text2.c_str(), Color(255, 255, 255), 1.f, true);
			y -= textHeight;
		}
		if (showFPS) {
			RenderUtils::drawText(d, Vec2<float>(x, y), "FPS:", colorsMod->getColor(), 1.f, true);
			char fpsStr[10];
			sprintf_s(fpsStr, "%i", (int)ImGui::GetIO().Framerate);
			RenderUtils::drawText(d, Vec2<float>(x + RenderUtils::getTextWidth("FPS: ", 1.f), y), fpsStr, Color(255, 255, 255), 1.f, true);
			y -= textHeight;
		}
	}
}