#pragma once
#include <string>

namespace NetworkUtil {
	bool canAccessInternet();
	bool sendPostRequest(const std::string& url, const std::string& data, const std::string& headers);
	bool downloadFile(std::string name, std::string path, std::string url);
}
