#pragma once
#include "../Module.h"

class Announcer : public IModule {
private:
	//Movement shit
	std::string walking = "I just moved forward thanks to AuraV4!";
	std::string backwards = "I just moved backward thanks to AuraV4!";
	std::string left = "I just moved to the left thanks to AuraV4!";
	std::string right = "I just moved to the right thanks to AuraV4!";
	std::string jump = "I just jumped thanks to AuraV4!";
	std::string sneak = "I just sneaked thanks to AuraV4!";
	bool movement = false;
	int forwardYes = 0;
	int backYes = 0;
	int rightYes = 0;
	int leftYes = 0;
	int jumpYes = 0;
	int sneakYes = 0;

	//Distance stuff
	std::string message = "I just Walked ";
	std::string dist = "";
	std::string blocks = " Blocks!";  //wow such good data
	vec3_t posBefore;
	vec3_t posNow;
	bool resetPos = true;
	int delay = 5;
	int Odelay = 0;

public:
	Announcer();
	~Announcer();
	//bools for sending stuff to chat
	bool WalkingDist = false;
	bool Module = true; //code for this is located in the Module.cpp
	bool BadMan = false;
	bool bypass = false;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm);
	virtual void onEnable() override;
};
