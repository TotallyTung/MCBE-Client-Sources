#pragma once
#include "../Module.h"

class Spammer : public IModule
{
private:
	int delay = 1;
	int length = 5;
	int messageTimer = 0;
	int messageIndex = 0;

public:
	bool bypass = false;
	const char *LoholismList[14] = {"Gamesense owns YOU", "Gamesense best client", "gamesense owning :yawn:", "Get Good. Get Gamesense", "Gamesense make u cry", "quit now gs kill u", "gamesense always on top!", "2ez4gamesense", "GS owns YOU and ALL", "GS > ALL", "GS users are Untouchable", "Why use any other client?", "gamesense ownage", "join the gs gang today"};
	const char *toxicList[15] = {"You are complete dogwater", "You aren't worthy of life", "my dog is better at pvp than you", "stay losing nn", "i winning all day", "i win again :yawn:", "sit down nn and cope", "uninstall or swat", "hush mode shitter", "keep fighting nn", "hang yourself incel", "*monkey noises*", "smh cringe newfags get blocked", "bow down doggie", "i am so much better than you LOLZ"};

	inline int &getDelay() { return delay; };
	SettingEnum messageMode;

	Spammer();
	~Spammer();

	const char *getRawModuleName() override;
	virtual void onEnable() override;
	virtual void onTick(C_GameMode *gm) override;
};
