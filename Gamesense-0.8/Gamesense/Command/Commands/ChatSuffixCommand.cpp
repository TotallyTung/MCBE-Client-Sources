#include "../../Module/ModuleManager.h"
#include "ChatSuffixCommand.h"

ChatSuffixCommand::ChatSuffixCommand() : IMCCommand("suffix", "Edit chat suffix", "<string>")
{
	registerAlias("suffix");
	registerAlias("chatsuffix");
	registerAlias("chat");
}

bool ChatSuffixCommand::execute(std::vector<std::string> *args)
{
	assertTrue(g_Data.getLocalPlayer() != nullptr);

	auto chatMod = moduleMgr->getModule<ChatSuffix>();

	std::ostringstream os;
	for (int i = 1; i < args->size(); i++)
	{
		if (i > 1)
			os << " ";
		os << args->at(i);
	}
	std::string text = os.str();
	chatMod->getSuffix() = text;
	clientMessageF("%sSuffix set to %s%s%s!", GREEN, GRAY, text.c_str(), GREEN);
	return true;
}