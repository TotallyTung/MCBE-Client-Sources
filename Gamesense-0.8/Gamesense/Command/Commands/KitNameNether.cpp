#include "../../Module/ModuleManager.h"
#include "KitNameNether.h"

KitNameNether::KitNameNether() : IMCCommand("kitnamenether", "edit autokit name for nether", "<string>")
{
	registerAlias("kitnamenether");
	registerAlias("knn");
	registerAlias("kitnn");
	registerAlias("knether");
}

bool KitNameNether::execute(std::vector<std::string>* args)
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
	kitMod->getKitNether() = text;
	clientMessageF("%sKit name set to %s%s%s!", GREEN, GRAY, text.c_str(), GREEN);
	return true;
}