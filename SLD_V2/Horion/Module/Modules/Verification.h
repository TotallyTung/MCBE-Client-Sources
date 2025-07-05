#include "Module.h"

class Verify : public IModule {
public:
	bool passedTest = false;

	bool test = false;
	bool BLACKLIST = false;
	std::string R1 = "R1";
	std::string e = "e";

	std::string de = "de";

	Verify();
	~Verify();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable();
};
