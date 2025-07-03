#include "ClientSetting.h"
#include "../../Memory/Hooks.h"
#include "../../Utils/DrawUtils.h"
int i2 = 0;
ClientSetting::ClientSetting() : IModule(0, Category::OTHER, "Changes how the client works internally")
{
	registerEnumSetting("AutoSave", &autoSave, 0, "Save config automatically");
	autoSave.addEntry("None", 0);
	autoSave.addEntry("Everytime", 1);
	autoSave.addEntry("CloseGUI", 2);
	registerEnumSetting("Font", &ejectMsg, 0, "What font should client use");
	ejectMsg.addEntry("Mojangles", 0);
	ejectMsg.addEntry("Smooth", 1);
	registerBoolSetting("MainMenu Logo", &mm, mm, "Should show the gamesense text on title screen");
	//registerBoolSetting("Logs", &logs, logs , "Should show logs like inject logs, this is for developers but its here if you want(Work in Progress, still adding logs on more functions)");
	registerBoolSetting("Changelogs", &changelog, changelog, "Should show the changelogs on title screen");
	registerBoolSetting("Loading Screen", &welcome, welcome);
	shouldHide = true;
}
const char *ClientSetting::getRawModuleName()
{
	return "ClientSettings";
}
void ClientSetting::onTick(C_GameMode *gm)
{
	auto nigga = "Gamesense User";
	TextHolder *fakeName = new TextHolder(nigga);
	if (nameSpoof)
		g_Data.setFakeName(fakeName);
	if (!nameSpoof)
		g_Data.setFakeName(NULL);
	if (autoSave.selected == 1)
	{
		configMgr->saveConfig();
	}
	if (autoSave.selected == 2)
	{
		const auto clickGui = moduleMgr->getModule<ClickGUIMod>();
		if (clickGui->isEnabled())
		{
			i2 = 1;
		}
		else if (i2 == 2)
		{
			configMgr->saveConfig();
			i2 = 0;
		}
	}
	shouldHide = true;
}
void ClientSetting::onPostRender(C_MinecraftUIRenderContext *renderCtx)
{
}
void ClientSetting::onDisable()
{
	g_Data.setFakeName(NULL);
	setEnabled(true);
}