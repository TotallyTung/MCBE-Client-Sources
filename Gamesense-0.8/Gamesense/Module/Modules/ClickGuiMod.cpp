#include "ClickGUIMod.h"
bool cFontEnable = false;
int CDelay = 0;
int delayN = 20;
using namespace std;
ClickGUIMod::ClickGUIMod() : IModule(VK_INSERT, Category::OTHER, "A GUI that displays every module")
{
	registerEnumSetting("Tips", &desc, 5, "Should show tooltips");
	desc.addEntry("None", 0);
	desc.addEntry("Underline", 1);
	desc.addEntry("Rectangle", 2);
	desc.addEntry("Box", 3);
	desc.addEntry("3arthh4ck", 4);
	desc.addEntry("Dynamic", 5);
	registerIntSetting("Alpha", &opacity, opacity, 0, 255, "Opacity of clickGui");
	registerIntSetting("Scroll Speed", &scrollSpeed, scrollSpeed, 1, 10, "Speed of scrolling");
	registerBoolSetting("Slide", &animations, animations, "Should do slide animations");
	registerBoolSetting("Fade", &animation1, animation1, "Should do fade animations");
	//registerBoolSetting("Font", &cFont, cFont, "Custom font YES!");
	registerEnumSetting("Tint", &tint, 1, "Should render background tint");
	tint.addEntry("Normal", 1);
	tint.addEntry("Synced", 2);
	registerFloatSetting("R", &red, red, 0.f, 255.f, 0.1f, "Tint color red");
	registerFloatSetting("G", &green, green, 0.f, 255.f, 0.1f, "Tint color green");
	registerFloatSetting("B", &blue, blue, 0.f, 255.f, 0.1f, "Tint color blue");
	registerFloatSetting("Tint Opacity", &tintopacity, tintopacity, 0.f, 255.f, 0.1f, "Opacity of the tint");
	shouldHide = true;
}

const char *ClickGUIMod::getRawModuleName()
{
	return ("ClickGUI");
}

bool ClickGUIMod::allowAutoStart()
{
	return false;
}

void ClickGUIMod::onEnable()
{
	g_Data.getClientInstance()->releaseMouse();
	openAnim = -500;
	openTimeOffset = 0;
	openTime = 0;
	configs.clear();
	isSettingOpened = false;

	std::string ConfigFolder = (getenv("AppData") + (std::string) "\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\Gamesense\\Configs\\");
	for (const auto &file : std::filesystem::directory_iterator(ConfigFolder))
	{
		if (!file.is_directory())
		{
			configs.push_back(file);
		}
	}

	if (!showHudEditor)
	{
		hasOpenedGUI = true;
	}
}

void ClickGUIMod::onPlayerTick(C_Player *plr)
{
	if (hasOpenedGUI && openTimeOffset == 0)
		openTimeOffset = TimerUtil::getCurrentMs();
	if (openAnim < 27 && hasOpenedGUI)
		openAnim += (28 - openAnim) * 0.09f;
	if (hasOpenedGUI)
		openTime = TimerUtil::getCurrentMs() - openTimeOffset;
}

void ClickGUIMod::onTick(C_GameMode *gm)
{
	cFont = false;
}

void ClickGUIMod::onPostRender(C_MinecraftUIRenderContext *renderCtx)
{
	if (GameData::canUseMoveKeys())
		g_Data.getClientInstance()->releaseMouse();
}

void ClickGUIMod::onPreRender(C_MinecraftUIRenderContext *renderCtx)
{
	if (animation1) {
		if (opacityBackGround < 76.5f) opacityBackGround += 76.5f / 30.f;

		if (opacityReal < opacity) opacityReal += opacity / 30.f;
		if (opacityText < 255.f) opacityText += 255.f / 30.f;
		if (opacityReal > opacity) opacityReal -= 0.1f;
		if (opacityText > 255.f) opacityText -= 0.1f; // Cho nay code hoi ngu de nghi sau=))
	}
	else {
		opacityBackGround = 76.5f;
		opacityReal = opacity;
		opacityText = 255.f;
	}
	if (openAnim < 27 && hasOpenedGUI) openAnim += (28 - openAnim) * 0.09f;
}

void ClickGUIMod::onDisable()
{
	opacityText = 0;
	opacityReal = 0;
	opacityBackGround = 0;
	g_Data.getClientInstance()->grabMouse();
	openAnimation = false;
	hasOpenedGUI = false;
	animation = 1;
	openTimeOffset = 0;
	openTime = 0;
	isSettingOpened = false;
	settingOpened = false;
}

void ClickGUIMod::onLoadConfig(void *conf)
{
	IModule::onLoadConfig(conf);
	ClickGui::onLoadConfig(conf);
}
void ClickGUIMod::onSaveConfig(void *conf)
{
	IModule::onSaveConfig(conf);
	ClickGui::onSaveConfig(conf);
}
