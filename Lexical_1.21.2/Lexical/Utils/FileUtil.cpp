#include "FileUtil.h"

std::string FileUtil::getRoamingStatePath() {
	static std::string RoamingStatePath = (getenv("AppData") + std::string("\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\"));
	return RoamingStatePath;
}

std::string FileUtil::getClientPath() {
	static std::string ClientPath = getRoamingStatePath() + "Lexical\\";
	return ClientPath;
}

#include <windows.h>
#include <thread>
#include <iostream>
#pragma comment(lib, "winmm.lib")   
#pragma comment(lib, "urlmon.lib")  

void PlayWithMCI(const std::string& filePath, bool loop) {
    mciSendStringA("close sound", NULL, 0, NULL);
    std::string openCommand = "open \"" + filePath + "\" type waveaudio alias sound";
    mciSendStringA(openCommand.c_str(), NULL, 0, NULL);
    std::string playCommand = "play sound";
    if (loop) {
        playCommand += " repeat";
    }
    mciSendStringA(playCommand.c_str(), NULL, 0, NULL);
}

void FileUtil::PlaySoundFromUrl(const std::string& url, float volume, bool loop) {
    std::thread([url, volume, loop]() {
        std::string fileName = "sound_" + std::to_string(time(0)) + ".wav";
        std::string tempPath = std::getenv("TEMP");
        std::string filePath = tempPath + "\\" + fileName;

        if (GetFileAttributesA(filePath.c_str()) == INVALID_FILE_ATTRIBUTES) {
            std::cout << "Downloading file from: " << url << std::endl;
            HRESULT hr = URLDownloadToFileA(NULL, url.c_str(), filePath.c_str(), 0, NULL);
            if (FAILED(hr)) {
                std::cerr << "Failed to download file: " << url << std::endl;
                return;
            }
        }

        PlayWithMCI(filePath, loop);
        }).detach();
}
