#pragma once
#include "../ModuleManager.h"
#include "Module.h"
class AutoReply : public IModule {
public:
	std::string Message = u8"Hello! Thank you for your message!";
	AutoReply() : IModule(0x0, Category::MISC, u8"/r AutoReply::CUSTOM MESSAGES:)!") {
	};
	~AutoReply() {};
	virtual const char* getModuleName() override {
		return "AutoReply";
	}
};