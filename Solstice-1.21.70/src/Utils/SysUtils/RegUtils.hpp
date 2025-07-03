#pragma once
//
// Created by vastrakai on 7/21/2024.
//

#include <Windows.h>
#include <string>
#include <iostream>
#include <comdef.h>
#include <Wbemidl.h>

class RegUtils {
public:
    template <typename T>
    static T readReg(const char* path, const char* key, T defaultValue) {
        HKEY hKey;
        T value;
        DWORD size = sizeof(T);
        if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, path, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
            if (RegQueryValueExA(hKey, key, NULL, NULL, (LPBYTE)&value, &size) != ERROR_SUCCESS) {
                value = defaultValue;
            }
            RegCloseKey(hKey);
        } else {
            value = defaultValue;
        }
        return value;
    }

    template <typename T>
    static T readReg(const std::string& path, const std::string& key, T defaultValue) {
        return readReg<T>(path.c_str(), key.c_str(), defaultValue);
    }

    // read string
    static std::string readReg(const char* path, const char* key, const char* defaultValue);
    static std::string readReg(const std::string& path, const std::string& key, const std::string& defaultValue);
};