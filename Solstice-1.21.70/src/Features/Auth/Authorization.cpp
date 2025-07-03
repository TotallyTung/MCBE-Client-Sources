//
// Created by alteik on 30/10/2024.
//

#include "Authorization.hpp"
#include "CustomCryptor.hpp"
#include <src/Utils/OAuthUtils.hpp>

void Auth::init()
{
    if (!InternetCheckConnectionA(xorstr_("https://dllserver.solstice.works"), FLAG_ICC_FORCE_CONNECTION, 0)) {
        exit();
    }

    if(FileUtils::fileExists(uniqueIdFile))
    {
        std::ifstream file(uniqueIdFile);
        file >> UniqueID;
        file.close();

        if(UniqueID.empty())
        {
            exit();
        }
    }
    else
    {
        exit();
    }
}

void Auth::exit()
{
#ifndef __DEBUG__
    __fastfail(0);
#endif
}

bool Auth::isPrivateUser()
{
    HttpRequest request(HttpMethod::GET, url + UniqueID, "", "", [](HttpResponseEvent event) {}, nullptr);
    HttpResponseEvent event = request.send();

    if(event.mStatusCode == 200)
    {
        try
        {
            nlohmann::json json = nlohmann::json::parse(event.mResponse);
            bool success = false;

            try
            {
                success = json[xorstr_("isPrivateUser")].get<bool>();
            }
            catch(...)
            {
                return false;
            }

            return success;
        } catch (nlohmann::json::exception& e) {
#ifdef __DEBUG__
            Solstice::console->error(e.what());
#endif
            return false;
        }
    }

    return false;
}

SysTime Auth::getOnlineTime()
{
    HttpRequest request(HttpMethod::GET, url2, "", "", [](HttpResponseEvent event) {}, nullptr);
    HttpResponseEvent event = request.send();

    if(event.mStatusCode == 200)
    {
        nlohmann::json jsonResponse = nlohmann::json::parse(event.mResponse);
        std::string datetime = jsonResponse.at(xorstr_("dateTime"));

        std::tm tm = {};
        std::istringstream ss(datetime);
        ss >> std::get_time(&tm, xorstr_("%Y-%m-%dT%H:%M:%S"));

        SysTime sysTime;
        sysTime.wYear = tm.tm_year + 1900;
        sysTime.wMonth = tm.tm_mon + 1;
        sysTime.wDay = tm.tm_mday;
        sysTime.wHour = tm.tm_hour;
        sysTime.wMinute = tm.tm_min;

        return sysTime;
    }
    else
    {
        __fastfail(0);
    }
}