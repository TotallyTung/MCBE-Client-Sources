#pragma once

#include "../Utils/Utils.h"
#include "Category.h"
#include <shared_mutex>

class Client;
class Hook;

class Manager {
public:
	Client* client;
	bool isRunning = false;
public:
	std::vector<Hook*> hooks;
	std::vector<Category*> categories;
	bool skipTicks = false;
public:
	auto ShowToastNotification(const std::string& title, const std::string& text) -> void;
	void ShowNotificationWithDelay(const std::string& title, const std::string& text, int delayInSeconds);
	Manager(Client*);
public:
	auto initHooks(void)->StatusData;
	auto initCategories(void)->void;
public:
	auto uninjectDLL(void) -> void;
};