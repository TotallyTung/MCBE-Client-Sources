#pragma once
#include "../Nuke.pl/DrawUtils.h"

class ClientColors {
public:

	//ClickGui
	static MC_Color clickGuiTooltipFillColor;
	static void setClickGuiTooltipFillColor(MC_Color color) {
		clickGuiTooltipFillColor = color;
	}

	static MC_Color clickGuiTooltipOutlineColor;
	static void setClickGuiTooltipOutlineColor(MC_Color color) {
		clickGuiTooltipOutlineColor = color;
	}

	static MC_Color clickGuiEnabledModuleColor;
	static void setClickGuiEnabledModuleColor(MC_Color color) {
		clickGuiEnabledModuleColor = color;
	}

	static MC_Color clickGuiModuleColor;
	static void setClickGuiModuleColor(MC_Color color) {
		clickGuiModuleColor = color;
	}

	static MC_Color clickGuiHoveredModuleColor;
	static void setClickGuiHoveredModuleColor(MC_Color color) {
		clickGuiHoveredModuleColor = color;
	}

	static MC_Color clickGuiModuleTabsBackgroundColor;
	static void setClickGuiModuleTabsBackgroundColor(MC_Color color) {
		clickGuiModuleTabsBackgroundColor = color;
	}

	static MC_Color clickGuiModuleTabsOutlineColor;
	static void setClickGuiModuleTabsOutlineColor(MC_Color color) {
		clickGuiModuleTabsOutlineColor = color;
	}

	//Nametags
	static MC_Color nametagsBackgroundColor;
	static void setNametagsBackgroundColor(MC_Color color) {
		nametagsBackgroundColor = color;
	}

	static MC_Color nametagsUnderlineColor;
	static void setNametagsUnderlineColor(MC_Color color) {
		nametagsUnderlineColor = color;
	}

	//MenuBackground
	static MC_Color menuBackgroundColor;
	static void setMenuBackgroundColor(MC_Color color) {
		menuBackgroundColor = color;
	}

};