#pragma once
#include "../ModuleManager.h"
#include "Module.h"
#include "../../FriendList/FriendsManager.h"
class AutoEZ : public IModule {
public:
	bool isDead = false;
	bool antiSpamCheck = false;
	AutoEZ();
	std::vector<Vec3> PlayerPos;
	std::vector<std::string> PlayerName;
	std::vector<Entity*> targetListAutoEZLEL;
	SettingEnum msgtype = this;
	// Inherited via IModule
	virtual const char *getModuleName() override;
	virtual void onTick(GameMode *gm) override;
	virtual void onJoiningServer() override;
};