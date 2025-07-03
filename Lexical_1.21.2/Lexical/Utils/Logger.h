#pragma once
#include <string>

#define LOGGER

#ifdef LOGGER
#define logF(output, ...) Logger::debugOutput(output, __VA_ARGS__);
#else
#define logF(output, ...)
#endif

class Logger {
private:
	static inline std::string logPath;
public:
	static void init();
	static void debugOutput(const char* output, ...);
};