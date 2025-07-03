#pragma once

#ifdef DEVICEID_UTIL_EXPORTS
#define DEVICEID_UTIL_API __declspec(dllexport)
#else
#define DEVICEID_UTIL_API __declspec(dllimport)
#endif

#include <string>

class DeviceIDUtils {
public:
	static std::string getDeviceID();
};