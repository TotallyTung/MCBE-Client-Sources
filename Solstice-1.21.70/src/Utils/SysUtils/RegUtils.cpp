//
// Created by vastrakai on 7/21/2024.
//

#include "RegUtils.hpp"

std::string RegUtils::readReg(const char* path, const char* key, const char* defaultValue)
{
    HKEY hKey;
    char value[255];
    DWORD size = sizeof(value);
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, path, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        if (RegQueryValueExA(hKey, key, nullptr, nullptr, (LPBYTE)&value, &size) != ERROR_SUCCESS) {
            return defaultValue;
        }
        RegCloseKey(hKey);
    } else {
        return defaultValue;
    }
    return std::string(value);
}

std::string RegUtils::readReg(const std::string& path, const std::string& key, const std::string& defaultValue)
{
    return readReg(path.c_str(), key.c_str(), defaultValue.c_str());
}
