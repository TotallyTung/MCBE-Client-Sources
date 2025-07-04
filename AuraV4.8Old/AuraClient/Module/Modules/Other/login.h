#include "../Module.h"

class Verify : public IModule {
public:
	bool passedTest = false;
	bool test = false;

	Verify();
	~Verify();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable();
	virtual bool isEnabled() override;
	virtual void onKeyUpdate(int key, bool isDown) override {};
	virtual void setKeybind(int) override {};
	virtual void setEnabled(bool) override {};
	virtual void onSaveConfig(void* conf) override {};
	virtual void onLoadConfig(void* conf) override {};

};
