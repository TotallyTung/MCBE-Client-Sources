#include "BreakHighlight.h"
#include "../../../../Utils/ImGuiUtils.h"
#include <iomanip>
#include <string>
BreakHighlight::BreakHighlight() : IModule(0, Category::VISUAL, "Renders breakprogress") {
	registerEnumSetting("Render", &render, 0, "Render type");
	render.addEntry("Fill", 0);
	render.addEntry("Outline", 1);
	render.addEntry("Both", 2);
	render.addEntry("HUD", 3);
	registerEnumSetting("Animations", &anim, 0, "Animations of the mine render");
	anim.addEntry("None", 0);
	anim.addEntry("Grow", 1);
	anim.addEntry("Fade", 2);
	registerFloatSetting("Alpha", &opacity, opacity, 0.0f, 1.f, 0.01f, "Opacity of the render");
	registerBoolSetting("Render Text", &renderProgress, renderProgress, "Renders the progress via text!");
}

const char* BreakHighlight::getRawModuleName() {
	return "BreakIndicator";
}
const char* BreakHighlight::getModName() {
	return "";
}

void BreakHighlight::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	// Packetmine code reused!!
	C_GameMode* gm = g_Data.getCGameMode();
	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(renderPos);
	if (block != nullptr && block->blockLegacy->blockId != 0 && block->blockLegacy->blockId != 7) {
		if (*(float*)((__int64)gm + 0x24) > 0.f) {
			auto blockPos = renderPos.toVector3().add(0.5, 0.5, 0.5);
			float test = *(float*)((__int64)gm + 0x24);
			float opac = opacity * 2;
			float progress(*(float*)((__int64)gm + 0x24) * 100);
			std::ostringstream oss;
			oss << std::fixed << std::setprecision(2) << progress;
			std::string formattedProgress = oss.str() + "%";
			// None
			if (anim.getSelectedValue() == 0) {
				// Outline
				if (render.getSelectedValue() == 2 || render.getSelectedValue() == 1) {
					DrawUtils::setColor(1.f - 1.f * test, 1.f * test, 0.f, 1.f);
					DrawUtils::drawBox(renderPos.toVector3().sub(0, 0, 0), renderPos.toVector3().add(1, 1, 1), .5f, false);
				}
				// Fill Render
				if (render.getSelectedValue() == 0 || render.getSelectedValue() == 2) {
					DrawUtils::setColor(1.f - 1.f * test, 1.f * test, 0.f, opacity);
					DrawUtils::drawFilledBox(blockPos);
				}
			}
			// Grow
			if (anim.getSelectedValue() == 1) {
				// Outline
				if (render.getSelectedValue() == 2 || render.getSelectedValue() == 1) {
					DrawUtils::setColor(1.f - 1.f * test, 1.f * test, 0.f, 1.f);
					DrawUtils::drawBox(renderPos.toVector3().add(0.5f).sub(0.5f * test), renderPos.toVector3().add(0.5f).add(0.5f * test), 0.5f, false, true);
				}
				// Fill Render
				if (render.getSelectedValue() == 0 || render.getSelectedValue() == 2) {
					DrawUtils::setColor(1.f - 1.f * test, 1.f * test, 0.f, opacity);
					DrawUtils::drawFilledBox(blockPos, true, test * 28);
				}
			}
			// Fade
			if (anim.getSelectedValue() == 2) {
				// Outline
				if (render.getSelectedValue() == 2 || render.getSelectedValue() == 1) {
					DrawUtils::setColor(1.f - 1.f * test, 1.f * test, 0.f, 0.1f * test * 2 * opac);
					DrawUtils::drawBox(renderPos.toVector3().sub(0, 0, 0), renderPos.toVector3().add(1, 1, 1), .5f, false);
				}
				// Fill Render
				if (render.getSelectedValue() == 0 || render.getSelectedValue() == 2) {
					DrawUtils::setColor(1.f - 1.f * test, 1.f * test, 0.f, 0.1f * test * 2 * opac);
					DrawUtils::drawFilledBox(blockPos);
				}
			}
			// Render Text
			if (renderProgress) {
				DrawUtils::drawTextInWorld(&formattedProgress, renderPos.toVector3().sub(0, 1.2f, 0), 0.6f, vec3_ti(180, 180, 180), vec3_ti(12, 12, 12), .1f);
			}
		}
	}
}
void BreakHighlight::onImGuiRender(ImDrawList* d) {
	C_GameMode* gm = g_Data.getCGameMode();
	vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	windowSize = g_Data.getClientInstance()->getGuiData()->windowSizeReal;
	auto interfaceC = ColorUtil::interfaceColor(1);
	ImColor interfaceColor = (interfaceC.r, interfaceC.g, interfaceC.b);
	float progress(*(float*)((__int64)gm + 0x24) * 100);
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(2) << progress;
	std::string formattedProgress = oss.str() + "%";
	ImGuiUtils::renderText(d, ImVec2(windowSize.x / 2, windowSize.y / 2), "Mining:", ImColor(interfaceColor), 25.f);
}