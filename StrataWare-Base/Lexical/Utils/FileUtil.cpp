#include "FileUtil.h"

std::string FileUtil::getRoamingStatePath() {
	static std::string RoamingStatePath = (getenv("AppData") + std::string("\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\"));
	return RoamingStatePath;
}

std::string FileUtil::getClientPath() {
	static std::string ClientPath = getRoamingStatePath() + "StrataWare\\";
	return ClientPath;
}