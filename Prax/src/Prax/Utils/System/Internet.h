//
// Created by Flash on 7/19/2023.
//

#pragma once


#include <wininet.h>
#include <tchar.h>
#include <fstream>

class Internet {
public:
    static std::string Get(std::string url);
    static void Post(const std::string &url, const std::string &jsonData);
    static bool DownloadFile(std::string Url, std::string path);
    static std::string GetFileNameFromURL(std::string url);
    static std::string GetSillyPicture(const std::string& endpoint = "waifu", bool nsfw = false);
    static int CheckUser(std::string username);
    static std::string GetHiveStatsJson(const std::string& gamemode, const std::string& gamertag);
    static std::string GetHiveStats(const std::string &gamemode, const std::string &gamertag);
    static int64_t GetFirstPlayed(const std::string& gamemode, const std::string& gamertag);
    static bool IsPlayerNicked(const std::string &playa);

    // nice killswitch frfr.
    //#define Killswitch() (Internet::Get(xorstr_("https://raw.githubusercontent.com/Prax-Client/Releases/main/latest.txt")).empty() ? true : Internet::Get(xorstr_("https://raw.githubusercontent.com/Prax-Client/Releases/main/latest.txt")) == xorstr_("false"))
};