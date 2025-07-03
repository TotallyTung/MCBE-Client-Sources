#pragma once
#include "Setting.h"
#include <vector>

class PageSetting : public Setting {
public:
	int* valuePtr;
	std::vector<std::string> pageNames;

	PageSetting(const std::string& name, const std::string& des, std::vector<std::string> pages, int* selectedPage, int defaultPage = 0) {
		this->name = name;
		this->description = des;
		this->valuePtr = selectedPage;
		this->pageNames = pages;
		*this->valuePtr = defaultPage;

		this->type = SettingType::PAGE_S;
	}
};
