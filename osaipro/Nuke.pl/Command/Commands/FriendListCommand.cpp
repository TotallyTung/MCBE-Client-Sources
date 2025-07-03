#include "FriendListCommand.h"
#include "../../Module/ModuleManager.h"
#include "../../FriendList/FriendsManager.h"

FriendListCommand::FriendListCommand() : IMCCommand("friend", "Add/Remove friendly players", "<add/remove>") {
	registerAlias("friendlist");
	registerAlias("fr");
	registerAlias("f");
	registerAlias("friends");
	registerAlias("addfriends");
	registerAlias("af");
	registerAlias("addfriends");
	registerAlias("afriends");
	registerAlias("afriend");
	registerAlias("addfriend");
	registerAlias("addfr");
	registerAlias("removefriend");
	registerAlias("removefriends");
	registerAlias("removefr");
	registerAlias("rfriend");
	registerAlias("rfriends");
	registerAlias("rfr");
	registerAlias("rmfriend");
	registerAlias("rmfr");
	registerAlias("rmfriends");
	registerAlias("rmfrs");
	registerAlias("delfriend");
	registerAlias("delfriends");
	registerAlias("delfr");
	registerAlias("delf");
	registerAlias("delfr");
	registerAlias("rmf");
	registerAlias("rmfr");
	registerAlias("rf");
	registerAlias("rfr");
}

FriendListCommand::~FriendListCommand() {
}

bool FriendListCommand::execute(std::vector<std::string>* args) {
	assertTrue(args->size() >= 3);
	std::string subcommand = args->at(1); std::transform(subcommand.begin(), subcommand.end(), subcommand.begin(), ::tolower);
	std::string searchedName = args->at(2);
	if (searchedName.size() <= 1) {
		clientMessageF(u8"%s%sCouldn't find player: %s!", Utils::getCommandClientName(), RED, searchedName.c_str());
		return true;
	}
	if (subcommand == "add" && !FriendsManager::findFriend(searchedName) || subcommand == "a" && !FriendsManager::findFriend(searchedName) || subcommand == "addtofriends" && !FriendsManager::findFriend(searchedName) ) {
		if (FriendsManager::findFriend(searchedName)) {
			clientMessageF(u8"%s%s%s was already your friend!", Utils::getCommandClientName(), GREEN, searchedName.c_str());
			return true;
		}
		FriendsManager::addFriendToList(std::string(searchedName));
		clientMessageF(u8"%s%s%s is now your friend!", Utils::getCommandClientName(), GREEN, searchedName.c_str());
		if (searchedName.find(u8" ") != std::string::npos) {
			searchedName = '"' + searchedName + '"';
		}
		if (moduleMgr->getModule<MidClick>()->tell) Utils::sendCommand("tell " + searchedName + u8" ‚ ‚È‚½‚ğFriendList‚É’Ç‰Á‚µ‚Ü‚µ‚½ (Added you to FriendList) by FriendCommand");
		return true;
	}
	else if (subcommand == "remove" || subcommand == "rm" || subcommand == "r" || subcommand == "del" || subcommand == "delete") {
		if (FriendsManager::removeFriend(searchedName)) {
			clientMessageF(u8"%s%s%s has been removed from your friendlist!", Utils::getCommandClientName(), GREEN, searchedName.c_str());
			if (searchedName.find(u8" ") != std::string::npos) {
				searchedName = '"' + searchedName + '"';
			}
			if (moduleMgr->getModule<MidClick>()->tell) Utils::sendCommand("tell " + searchedName + u8" ‚ ‚È‚½‚ğFriendList‚©‚çíœ‚µ‚Ü‚µ‚½ (removed you from FriendList) by FriendCommand");
			return true;
		} else {
			clientMessageF(u8"%s%s%s was not in your friendlist!", Utils::getCommandClientName(), GREEN, searchedName.c_str());
			return true;
		}
	}
	return true;
}
