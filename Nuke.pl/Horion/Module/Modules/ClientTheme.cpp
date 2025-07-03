#include "ClientTheme.h"

#include "../../../Utils/ClientColors.h"
ClientTheme::ClientTheme() : IModule(0, Category::CLIENT, "Go for a retro style or streamline.") {
	Theme.addEntry(EnumEntry("Dark", 0)).addEntry(EnumEntry("Legacy", 1));
	registerEnumSetting("Client Theme", &Theme, 0);
}

ClientTheme::~ClientTheme() {
}

const char* ClientTheme::getModuleName() {
	return ("ClientTheme");
}

unsigned char mode = 0;
auto lastMode = mode;

void ClientTheme::onRender(MinecraftUIRenderContext* renderCtx) {
	mode = Theme.GetSelectedEntry().GetValue();
	if (lastMode != mode) {
		switch (Theme.selected) {
		case 0: {
			// ClickGui
			ClientColors::setClickGuiTooltipFillColor(MC_Color(18, 18, 18));
			ClientColors::setClickGuiTooltipOutlineColor(MC_Color(50, 50, 50));
			ClientColors::setClickGuiEnabledModuleColor(MC_Color(30, 30, 30));
			ClientColors::setClickGuiModuleColor(MC_Color(18, 18, 18));
			ClientColors::setClickGuiHoveredModuleColor(MC_Color(50, 50, 50));
			ClientColors::setClickGuiModuleTabsBackgroundColor(MC_Color(18, 18, 18));
			ClientColors::setClickGuiModuleTabsOutlineColor(MC_Color(50, 50, 50));
			// Nametags
			ClientColors::setNametagsBackgroundColor(MC_Color(12, 12, 12));
			ClientColors::setNametagsUnderlineColor(MC_Color(85, 85, 85));
			// Menu
			ClientColors::setMenuBackgroundColor(MC_Color(12, 12, 12));
			break;
		}
		case 1: {
			// ClickGui
			ClientColors::setClickGuiTooltipFillColor(MC_Color(15, 30, 50));
			ClientColors::setClickGuiTooltipOutlineColor(MC_Color(30, 110, 200));
			ClientColors::setClickGuiEnabledModuleColor(MC_Color(15, 60, 100));
			ClientColors::setClickGuiModuleColor(MC_Color(15, 30, 50));
			ClientColors::setClickGuiHoveredModuleColor(MC_Color(30, 110, 200));
			ClientColors::setClickGuiModuleTabsBackgroundColor(MC_Color(15, 30, 50));
			ClientColors::setClickGuiModuleTabsOutlineColor(MC_Color(30, 110, 200));
			// Nametags
			ClientColors::setNametagsBackgroundColor(MC_Color(13, 29, 48));
			ClientColors::setNametagsUnderlineColor(MC_Color(30, 110, 20));
			// Menu
			ClientColors::setMenuBackgroundColor(MC_Color(13, 29, 48));
			break;
		}
		}
		lastMode = mode;
	}
}