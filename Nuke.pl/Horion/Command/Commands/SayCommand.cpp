#include "SayCommand.h"

SayCommand::SayCommand() : IMCCommand("say", "Sends a chat message", "<message>") {
}

SayCommand::~SayCommand() {
}

std::string string_replace(const std::string& s, const std::string& findS, const std::string& replaceS) {
	std::string result = s;
	auto pos = s.find(findS);
	if (pos == std::string::npos) {
		return result;
	}
	result.replace(pos, findS.length(), replaceS);
	return string_replace(result, findS, replaceS);
}

std::string parse(const std::string& s) {
	static std::vector<std::pair<std::string, std::string> > patterns = {
		{"\\\\", "\\"},
		{"\\n", "\n"},
		{"\\r", "\r"},
		{"\\t", "\t"},
		{"\\f", "\f"},
		{"\\\"", "\""}};
	std::string result = s;
	for (const auto& p : patterns) {
		result = string_replace(result, p.first, p.second);
	}
	return result;
}

bool SayCommand::execute(std::vector<std::string>* args) {
	assertTrue(args->size() > 1);

	std::ostringstream os;
	for (int i = 1; i < args->size(); i++) {
		if (i > 1)
			os << " ";
		os << args->at(i);
	}
	Utils::sendChat(parse(os.str()));
	clientMessageF(u8"%s%sSent Message", Utils::getCommandClientName(), GREEN);
	return true;
}
