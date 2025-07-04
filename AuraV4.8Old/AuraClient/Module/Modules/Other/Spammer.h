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
	std::vector<std::string> LoholismList = {
		"Team Aura Owns All", 
		"AuraV4 best client", 
		"So ez for Aura", 
		"Aura Owns YOU", 
		"Why use other clients when Aura V4 exists", 
		"AuraV4 is too good", 
		"@here I am too good at this game because I use AuraV4", 
		"too ez for Aura",
		"Sit down weaks Aura owns all", 
		"Aura > ALL", 
		"Strongest client for spvp: AuraV4",
		"Why use any other client?", 
		"AuraV4 Owns IGN", 
		"Join Team Aura Today!",
		"Don't be mad when you die to AuraV4, it is just too good",
		"Want some free skills? Use AuraV4 lah",
		"Ngl AuraV4 boosts your skill for spvp",
		"AuraV4 = God Client for pvp",
		"Imagine using those fancy looking clients that suck at pvp",
		"Why AuraV4 is the best: ADVANCED Killaura and Crystalaura and features that will save you a lot of totems in pvp!",//20
		"I hate it when people just hate when they never use the client even once",
		"Can't win for a duel that someone requested with you? Use AuraV4 for free skill boost!",
		"Question: Who made AuraV4? Answer: Afar Arkaris!",
		"Tired of getting killed over and over again? Tired of getting absolutely dumpstered in a pvp fight? Use AuraV4 today! We have so much new features for you to explore!",
		"Where to get Aurav4 public: Team DynastY discord - bedrock clients channel! Have fun using Aura!",
		"Why is AuraV4 so good??? I kill everyone with it!",
		"AuraV4 is a Nuke skid: but a good one. Proof: NukeV6 user pop me only once in rain while AuraV4 user popped him TWELVE TIMES",
		"Guys, you should actually try out AuraV4 before hating on it. Everyone who tried it out in Spvp said it has insane damage!",
		"AuraV4's dev focuses on the client's power rather than its looks!",
		"AuraV4's dev listens to his user feedbacks! If you have used AuraV4 and have a problem or a bug, please report to the dev of this client! His discord: afararkaris",
		"You can't beat AuraV4",
		"Let's be honest: AuraV4 is currently the best client for Spvp which has advanced Killaura features",//32
		"If you are using horion, please consider switching to AuraV4, horion is so bad",
		"Why use shitty clients and make your life hard in PvP? Use AuraV4 today for a free boost!",
		"Imagine still using the basic Nuke or Borion for pvp, what a pity, how about start using AuraV4 instead?"
		"Want to pop the enemy's totems more? Use AuraV4 today! We offer ADVANCED Killaura Autoweapon and features, advanced utilities and so much more!"
		"Remember Afar Arkaris would LOVE to hear your feedback on AuraV4!",
		"How to kill the enemy fast: use AuraV4",
		"How to deal more damage in Spvp: use AuraV4",
		"GGs guys for those of you all who are still using old clients: they can not be compared to AuraV4"
	};

	std::vector<std::string> toxicList = { 
		"You are so bad at the game please go kill yourself", 
		"You aren't worthy of life", 
		"my dog is better at pvp than you", 
		"stay mad nn", 
		"I win again", 
		"Win Win for me again I guess", 
		"Cope niggers", 
		"Ez shitters", 
		"Shut nn", 
		"keep yapping nn", 
		"hang yourself incel",
		"*monkey noises*", 
		"smh cringe newfags get blocked",
		"bow down to the king @here", 
		"i am so much better than you LOLZ",
		"Fu ck you",
		"Hey retards, guess what? you can never beat me LOL",
		"Give me your address and I will kill you irl",	
	};

	inline int& getDelay() { return delay; };
	SettingEnum messageMode;

	Spammer();
	~Spammer();

	const char* getModuleName() override;
	virtual void onEnable() override;
	virtual void onTick(C_GameMode* gm) override;
};
