#pragma once
#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace FileUtil {

	std::string getRoamingStatePath();
	std::string getClientPath();

	inline bool doesFilePathExist(const std::string& path) {
		return std::filesystem::exists(path);
	}

	inline bool createPath(const std::string& path) {
		return std::filesystem::create_directory(path);
	}

	inline bool deletePath(const std::string& path) {
		return std::filesystem::remove(path);
	}
}