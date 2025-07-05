//
// Created by Flash on 7/19/2023.
//

#include "Internet.h"

#include <wininet.h>
#include <tchar.h>
#include <fstream>
#include <xorstr.hpp>
#include <chrono>
#include <codecvt>
#include <Psapi.h>
#include <Prax/ExceptionHandler.h>
#include <Prax/Utils/String.h>

#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "wininet.lib")

// Yes, chatgpt made this
void Internet::Post(const std::string& url, const std::string& jsonData) {
    HINTERNET hInternet = InternetOpenA("PraxClient/1.0", INTERNET_OPEN_TYPE_DIRECT, nullptr, nullptr, 0);
    if (!hInternet) {
        Logger::Write("Internet", "Failed to open Internet.", Logger::LogType::Error);
        return;
    }

    HINTERNET hConnect = InternetOpenUrlA(hInternet, url.c_str(), nullptr, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hConnect) {
        Logger::Write("Internet", "Failed to open Internet URL.", Logger::LogType::Error);
        InternetCloseHandle(hInternet);
        return;
    }

    std::string headers = "Content-Type: application/json\r\n";
    if (!HttpSendRequestA(hConnect, headers.c_str(), static_cast<DWORD>(headers.length()), const_cast<char*>(jsonData.c_str()), static_cast<DWORD>(jsonData.length()))) {
        Logger::Write("Internet", "Failed to send HTTP request.", Logger::LogType::Error);

        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return;
    }

    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
}

std::string Internet::Get(std::string url) {
    HINTERNET interwebs = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT,
                                        NULL, NULL, NULL);
    HINTERNET urlFile;
    std::string rtn;

    if (interwebs) {
        DWORD flags = INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_NO_UI | INTERNET_FLAG_SECURE;
        urlFile = InternetOpenUrlA(interwebs, url.c_str(), NULL, NULL, flags, NULL);
        if (urlFile) {
            DWORD statusCode = 0;
            DWORD statusCodeSize = sizeof(statusCode);
            HttpQueryInfo(urlFile, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &statusCode, &statusCodeSize, NULL);

            if (statusCode == 200) {
                char buffer[2000];
                DWORD bytesRead;
                do {
                    InternetReadFile(urlFile, buffer, sizeof(buffer), &bytesRead);
                    rtn.append(buffer, bytesRead);
                } while (bytesRead > 0);
            }

            InternetCloseHandle(urlFile);
        }

        InternetCloseHandle(interwebs);
    }

    return rtn;
}

bool Internet::DownloadFile(std::string url, std::string path) {
    std::string response = Get(url);

    if (response.empty()) {
        return false;
    }

    if (response.find("HTTP/1.1 404") != std::string::npos) {
        return false;
    }

    std::ofstream outputFile(path, std::ios::binary);
    if (!outputFile.is_open()) {
        return false;
    }

    outputFile.write(response.c_str(), response.size());
    if (!outputFile) {
        return false;
    }

    outputFile.close();

    Logger::Write("Internet", "Downloaded file " + path, Logger::LogType::Debug);

    return true;
}

std::string Internet::GetSillyPicture(const std::string& endpoint, bool nsfw) {
    std::string category = nsfw ? xorstr_("nsfw/") : xorstr_("sfw/");
    std::string jsonString = Get(xorstr_("https://api.waifu.pics/") + category + endpoint);

    std::string imageURL;
    size_t startPos = jsonString.find("https://");

    if (startPos != std::string::npos) {
        size_t endPos = jsonString.find("\"", startPos);
        if (endPos != std::string::npos) {
            imageURL = jsonString.substr(startPos, endPos - startPos);
        }
    }

    return imageURL;
}

std::string Internet::GetFileNameFromURL(std::string url) {
    size_t lastSlashPos = url.find_last_of('/');
    if (lastSlashPos != std::string::npos) {
        return url.substr(lastSlashPos + 1);
    }
    return "";
}



int Internet::CheckUser(std::string username) {
    static std::map<std::string, int> cache;
    if (cache.find(username) != cache.end()) {
        return cache[username];
    }

    std::string url = "https://peoplehub-public.xboxlive.com/people/gt(" + username + ")";
    HINTERNET interwebs = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
    HINTERNET urlFile;
    std::string rtn;

    if (interwebs) {
        DWORD flags = INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_NO_UI | INTERNET_FLAG_SECURE;
        urlFile = InternetOpenUrlA(interwebs, url.c_str(), "x-xbl-contract-version: 3\r\nAccept-Language: en-US,en;q=0.5\r\n", -1L, flags, NULL);
        if (urlFile) {
            // Add custom headers
            BOOL headerAdded = HttpAddRequestHeadersA(urlFile, "x-xbl-contract-version: 3\r\nAccept-Language: en-US,en;q=0.5\r\n", -1L, HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);

            if (headerAdded) {
                DWORD statusCode = 0;
                DWORD statusCodeSize = sizeof(statusCode);
                HttpQueryInfo(urlFile, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &statusCode, &statusCodeSize, NULL);

                if (statusCode == 200) {
                    char buffer[2000];
                    DWORD bytesRead;
                    do {
                        InternetReadFile(urlFile, buffer, sizeof(buffer), &bytesRead);
                        rtn.append(buffer, bytesRead);
                    } while (bytesRead > 0);

                    // Check if rtn contains the required substrings
                    if (rtn.find("https://") != std::string::npos) {
                        Logger::Write("Internet", "Rtn: " + rtn + " Status: " + std::to_string(statusCode));
                        InternetCloseHandle(urlFile);
                        // Return true if status code is 200 and required substrings are found
                        cache[username] = statusCode;
                        return statusCode;
                    }
                    else {
                        // Return false if required substrings are not found
                        cache[username] = 0;
                        return 0;
                    }
                }
                else {
                    // Return the status code
                    return statusCode;
                }

                InternetCloseHandle(urlFile);
            }
            else {
                // Error adding headers
                InternetCloseHandle(urlFile);
            }
        }

        InternetCloseHandle(interwebs);
    }

    // Return false in case of errors or if required substrings are not found
    cache[username] = 0;
    return 0;
}

std::string Internet::GetHiveStatsJson(const std::string& gamemode, const std::string& gamertag) {
    std::string url = "https://api.playhive.com/v0/game/all/" + gamemode + "/" + gamertag;
    return Get(url);
}

std::string Internet::GetHiveStats(const std::string& gamemode, const std::string& gamertag) {
    // https://api.playhive.com/v0/game/all/gamemode/gamertag
    std::string url = "https://api.playhive.com/v0/game/all/" + gamemode + "/" + gamertag;
    std::string jsonString = Get(url);

    // Remove the UUID line
    size_t uuidPos = jsonString.find("\"UUID\":");
    if (uuidPos != std::string::npos) {
        size_t uuidEndPos = jsonString.find(",", uuidPos);
        if (uuidEndPos != std::string::npos) {
            jsonString.erase(uuidPos, uuidEndPos - uuidPos + 1);
        }
    }

    if (jsonString.empty()) {
        Logger::Write("Internet", "GetHiveStats: jsonString is empty", Logger::LogType::Error);
        return "";
    }

    // Remove the first and last characters
    jsonString = jsonString.substr(1, jsonString.size() - 2);

    // Replace the commas with commas and newlines
    jsonString = String::ReplaceAll(jsonString, ",", "\n");

    // Replace the colons with colons and spaces
    jsonString = String::ReplaceAll(jsonString, ":", ": ");

    // Replace the quotes with nothing
    jsonString = String::ReplaceAll(jsonString, "\"", "");

    // Replace the underscores with spaces
    jsonString = String::ReplaceAll(jsonString, "_", " ");

    // HOMELESS AHH CODE
    size_t firstPlayedPos = jsonString.find("first played: ");
    size_t firstPlayedEndPos = jsonString.find("\n", firstPlayedPos);

    // Use String::FormatTime to format the first played time
    if (firstPlayedPos != std::string::npos && firstPlayedEndPos != std::string::npos) {
        std::string firstPlayedStr = jsonString.substr(firstPlayedPos + 14, firstPlayedEndPos - firstPlayedPos - 14);
        int64_t firstPlayed = std::stoll(firstPlayedStr);
        jsonString.replace(firstPlayedPos + 14, firstPlayedEndPos - firstPlayedPos - 14, String::FormatTime(firstPlayed, "%m/%d/%Y %H:%M:%S"));
    }


    // Replace the first letter of each word with a capital letter
    std::string::iterator it = jsonString.begin();
    bool capitalizeNext = true;
    while (it != jsonString.end()) {
        if (capitalizeNext) {
            *it = toupper(*it);
            capitalizeNext = false;
        }
        if (*it == ' ') {
            capitalizeNext = true;
        }
        if (*it == '\n') {
            capitalizeNext = true;
        }
        ++it;
    }


    return jsonString;
}

// Specialized function to get only the first played time integer
int64_t Internet::GetFirstPlayed(const std::string& gamemode, const std::string& gamertag) {
    std::string url = "https://api.playhive.com/v0/game/all/" + gamemode + "/" + gamertag;
    std::string jsonString = Get(url);

    // Remove the UUID line
    size_t uuidPos = jsonString.find("\"UUID\":");
    if (uuidPos != std::string::npos) {
        size_t uuidEndPos = jsonString.find(",", uuidPos);
        if (uuidEndPos != std::string::npos) {
            jsonString.erase(uuidPos, uuidEndPos - uuidPos + 1);
        }
    }

    if (jsonString.empty()) {
        Logger::Write("Internet", "GetFirstPlayed: jsonString is empty", Logger::LogType::Error);
        return 0;
    }

    jsonString = jsonString.substr(1, jsonString.size() - 2);
    jsonString = String::ReplaceAll(jsonString, ",", "\n");
    jsonString = String::ReplaceAll(jsonString, ":", ": ");
    jsonString = String::ReplaceAll(jsonString, "\"", "");
    jsonString = String::ReplaceAll(jsonString, "_", " ");

    // HOMELESS AHH CODE
    size_t firstPlayedPos = jsonString.find("first played: ");
    size_t firstPlayedEndPos = jsonString.find("\n", firstPlayedPos);

    // Use String::FormatTime to format the first played time
    if (firstPlayedPos != std::string::npos && firstPlayedEndPos != std::string::npos) {
        std::string firstPlayedStr = jsonString.substr(firstPlayedPos + 14, firstPlayedEndPos - firstPlayedPos - 14);
        int64_t firstPlayed = std::stoll(firstPlayedStr);
        return firstPlayed;
    }

    return 0;
}

std::map<std::string, bool> nickCache = {};

bool Internet::IsPlayerNicked(const std::string& playa) {
     if (nickCache.find(playa) != nickCache.end()) {
        return nickCache[playa];
    }

    bool nicked = GetHiveStats("all", playa).empty();
    nickCache[playa] = nicked;

    return nicked;
}






