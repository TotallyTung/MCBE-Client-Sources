#include "ShowChatFriendList.h"
#include "../../FriendList/FriendsManager.h"

ShowChatFriendList::ShowChatFriendList() : IMCCommand("ShowFriendList", "Add/Remove/Show friendly players", "None") {
	registerAlias("sf");
	registerAlias("showfriend");
	registerAlias("sflist");
	registerAlias("showfl");
	registerAlias("showf");
	registerAlias("showfr");
	registerAlias("showfrlist");
	registerAlias("showfrl");
	registerAlias("showflist");
	registerAlias("sfrlist");
}

ShowChatFriendList::~ShowChatFriendList() {
}

bool ShowChatFriendList::execute(std::vector<std::string>* args) {
	std::vector<std::string> vec = FriendsManager::getList();
	std::stringstream ss;
	for (auto it = vec.begin(); it != vec.end(); it++) {
		if (it != vec.begin()) {
			ss << " ";
		}
		ss << *it;
	}

	Utils::sendChat("MyFriendList: " + ss.str());
	return true;
}
