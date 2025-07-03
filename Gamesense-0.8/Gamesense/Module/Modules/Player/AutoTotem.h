#pragma once
#include "../Module.h"

class AutoTotem : public IModule {
public:
    std::string name = ("Offhand");
	SettingEnum item;
	SettingEnum activation = this;

	bool notify = false;
	bool setOffhand = false;
	bool healthMode = false;
	int health = 20;

	SettingEnum mode = this;
	SettingEnum calling = this;
	// this is here for convenience
	void runNormal();
	void runNukkit();
	void runNukkitStrict();
	void offhandNotify();
	AutoTotem();

    virtual const char* getRawModuleName() override;
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onPlayerTick(C_GameMode* gm);
};
