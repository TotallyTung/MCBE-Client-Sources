#include "Logger.h"
#include "FileUtil.h"
#include <Windows.h>
#include <iostream>
#include <fstream>

void Logger::init() {
	logPath = FileUtil::getClientPath() + "Logs.txt";

	if (FileUtil::doesFilePathExist(logPath))
		FileUtil::deletePath(logPath);
}

void Logger::debugOutput(const char* output, ...) {

	CloseHandle(CreateFileA(logPath.c_str(), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL));

	std::ofstream fStream;
	fStream.open(logPath.c_str(), std::ios::app);

	if (fStream.is_open()) {
		va_list arg;
		va_start(arg, output);
		char message[300];
		vsprintf_s(message, 300, output, arg);
		va_end(arg);

		fStream << message << std::endl;
	}

	fStream.close();
}