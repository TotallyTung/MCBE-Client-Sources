#include "../../Module/ModuleManager.h"
#include "KitName.h"

KitName::KitName() : IMCCommand("kitname", "edit autokit name for overworld", "<string>")
{
	registerAlias("kitname");
	registerAlias("kn");
	registerAlias("kitn");
}

bool KitName::execute(std::vector<std::string>* args)
{
	assertTrue(g_Data.getLocalPlayer() != nullptr);

	auto kitMod = moduleMgr->getModule<AutoKit>();

	std::ostringstream os;
	for (int i = 1; i < args->size(); i++)
	{
		if (i > 1)
			os << " ";
		os << args->at(i);
	}
	std::string text = os.str();
	kitMod->getKitOW() = text;
	clientMessageF("%sKit name set to %s%s%s!", GREEN, GRAY, text.c_str(), GREEN);
	return true;
}