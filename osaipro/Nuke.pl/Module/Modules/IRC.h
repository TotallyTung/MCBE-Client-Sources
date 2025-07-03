#pragma once
#include "Module.h"
#include <chrono>
#include <iostream>
#include <string>
#include <winhttp.h>
#include <Windows.h>

#pragma comment(lib, "winhttp.lib")

class IRC : public IModule {
public:
	IRC();
	~IRC();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode*) override;

	virtual void SendToServer(HINTERNET hSession, const std::string& message);
	virtual std::string GetToClient(HINTERNET hSession);
};
#pragma once
