#include "Utils.h"

std::string Utils::getRoamPath() {
	char* path = NULL;
	size_t length;

	_dupenv_s(&path, &length, "appdata");
	std::string pathReal = std::string(path) + std::string("\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState");
	return pathReal;
}

std::string Utils::getClientFolderPath() {
	std::string path = getRoamPath() + "\\Kyrie";
	return path;
}

std::string Utils::getConfigsFolderPath() {
	std::string path = getClientFolderPath() + "\\Configs";
	return path;
}

void Utils::DebugF(const char* output, ...) {
	std::string path = getClientFolderPath() + "\\Logs.txt";

	va_list arg;
	va_start(arg, output);
	
	char message[300];
	vsprintf_s(message, 300, output, arg);

	CloseHandle(CreateFileA(path.c_str(), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL));

	std::ofstream fStream;
	fStream.open(path.c_str(), std::ios::app);

	if (fStream.is_open()) fStream << message << std::endl;
	fStream.close();

	va_end(arg);
}