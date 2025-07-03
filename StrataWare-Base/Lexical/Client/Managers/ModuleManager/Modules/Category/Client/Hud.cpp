#include "Hud.h"
#include "../../../ModuleManager.h"

Hud::Hud() : Module("Hud", "Modify your hud components.", Category::CLIENT) {
    registerSetting(new ToggleSetting("Arraylist", "NULL", &arraylist, false));
    registerSetting(new NumberSetting<float>("ArrayX", "NULL", &arraylistx, 25, 0, 1000));
    registerSetting(new NumberSetting<float>("ArrayY", "NULL", &arraylisty, 25, 0, 1000));
    registerSetting(new ToggleSetting("Watermark", "NULL", &watermark, true));
    registerSetting(new NumberSetting<float>("WatermarkX", "NULL", &watermarkx, 25, 0, 1000));
    registerSetting(new NumberSetting<float>("WatermarkY", "NULL", &watermarky, 25, 0, 1000));
    registerSetting(new ToggleSetting("Bold", "NULL", &bold, false));

    registerSetting(new ToggleSetting("Show Position", "Toggle position display", &showPosition, false));
    registerSetting(new NumberSetting<int>("Position X Offset", "X offset for position", &positionOffsetX, 10, -1000, 1000));
    registerSetting(new NumberSetting<int>("Position Y Offset", "Y offset for position", &positionOffsetY, 10, -1000, 1000));

    registerSetting(new ToggleSetting("Show Health%", "Toggle health percentage display", &showHealth, false));
    registerSetting(new NumberSetting<int>("Health X Offset", "X offset for health", &healthOffsetX, 10, -1000, 1000));
    registerSetting(new NumberSetting<int>("Health Y Offset", "Y offset for health", &healthOffsetY, 30, -1000, 1000));
}

bool Hud::isEnabled() {
	return true;
}
void Hud::onD2DRender() {
    if (!D2D::getWindowSize().x || !D2D::getWindowSize().y) return;
}
