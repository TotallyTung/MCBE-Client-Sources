// Created by Tony on 2024-10-10 10:01:54

#pragma once

#include <Windows.h>
#include <thread>
#include <fstream>
#include <wininet.h>
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "wininet.lib")
#include "Utils.h"
#include "../Libs/xor.hpp"

class Network {
public:
    static void downloadFile(const std::string& name, const std::string& url, const std::string& path) {
        std::string filePath = Utils::getClientPath() + xorstr_("\\") + path + xorstr_("\\") + name;


        HINTERNET webH = InternetOpenA(xorstr_("Mozilla/5.0"), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
        HINTERNET urlFile = NULL;

        if (webH) {
            urlFile = InternetOpenUrlA(webH, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
            if (urlFile) {
                std::ofstream outputFile(filePath, std::ios::binary);
                if (outputFile.is_open()) {
                    char buffer[2000];
                    DWORD bytesRead = 0;
                    while (InternetReadFile(urlFile, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
                        outputFile.write(buffer, bytesRead);
                    }
                    outputFile.close();
                }
                InternetCloseHandle(urlFile);
            }
            InternetCloseHandle(webH);
        }
    }
};