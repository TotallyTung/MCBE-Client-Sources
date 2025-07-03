//
// Created by vastrakai on 9/3/2024.
//

#include "OAuthUtils.hpp"

bool OAuthUtils::hasValidToken()
{

    static uint64_t lastAttempt = 0;
    static bool lastResult = false;

    // If the last attempt was less than 5 seconds ago, return the last result
    if (lastAttempt + 5000 > NOW)
    {
        //spdlog::info(xorstr_("Returning last result"));
        return lastResult;
    }

    HttpRequest request(HttpMethod::GET, sEndpoint + xorstr_("isAuthenticated?id=") + getToken(), "", "", [](HttpResponseEvent event) {}, nullptr);
    HttpResponseEvent event = request.send();



    //spdlog::info(xorstr_("Status code: {}, response {}"), event.mStatusCode, event.mResponse);

    lastAttempt = NOW;
    lastResult = event.mStatusCode == 200;

    if (event.mStatusCode == 200)
    {
        return true;
    }
    else
    {
        return false;
    }

}

std::string OAuthUtils::getToken()
{
    std::string tokenPath = FileUtils::getSolsticeDir() + xorstr_("uniqueId.txt");
    if (FileUtils::fileExists(tokenPath))
    {
        std::ifstream file(tokenPath);
        std::string token;
        std::getline(file, token);
        return token;
    }
    return "";
}

std::string OAuthUtils::getLatestCommitHash()
{
    HttpRequest request(HttpMethod::GET, sEndpoint + "getLatestCommitHash", "", "", [](HttpResponseEvent event) {}, nullptr);

    HttpResponseEvent event = request.send();

    if(event.mStatusCode == 200)
    {
        nlohmann::json json = nlohmann::json::parse(event.mResponse);
        return json[xorstr_("commitHash")].get<std::string>();
    }

    return "";
}

std::vector<std::string> OAuthUtils::getCommitsBetweenHash(const std::string& startHash, const std::string& endHash)
{
    HttpRequest request(HttpMethod::GET, sEndpoint + "getCommitsBetween?startHash=" + startHash + "&endHash=" + endHash, "", "", [](HttpResponseEvent event) {}, nullptr);

    HttpResponseEvent event = request.send();
    // sample response: ["commit1", "commit2", "commit3"]

    nlohmann::json json = nlohmann::json::parse(event.mResponse);
    return json.get<std::vector<std::string>>();
}

std::string OAuthUtils::getLastCommitHash()
{
    std::string commitPath = FileUtils::getSolsticeDir() + xorstr_("commitHash.txt");
    if (FileUtils::fileExists(commitPath))
    {
        std::ifstream file(commitPath);
        std::string commit;
        std::getline(file, commit);
        return commit;
    }

    return "";
}

void OAuthUtils::saveCommitHash(const std::string& commitHash)
{
    std::string commitPath = FileUtils::getSolsticeDir() + xorstr_("commitHash.txt");
    std::ofstream file(commitPath);
    file << commitHash;
    file.close();
}
