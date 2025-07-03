//
// Created by vastrakai on 6/28/2024.
//

#include "Command.hpp"

#include <algorithm>

Command::Command(const std::string& name)
{
	this->name = name;
}

bool Command::matchName(std::string_view toMatch) const
{
	if(toMatch == this->name) {
		return true;
	}

	const auto aliases = this->getAliases();
	const bool found = std::ranges::any_of(aliases, [=](auto alias)
	{
		if (alias == toMatch) {
			return true;
		}
		return false;
	});

	return found;
}

std::vector<std::string> Command::getNames() const
{
	std::vector<std::string> names;
	names.push_back(this->name);
	const auto aliases = this->getAliases();
	names.insert(names.end(), aliases.begin(), aliases.end());
	return names;
}
