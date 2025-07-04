#pragma once
#include <string>

namespace NetworkUtil {
	bool canAccessInternet();
	bool downloadFile(std::string name, std::string path, std::string url);
}
