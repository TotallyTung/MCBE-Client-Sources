#pragma once
//
// Created by vastrakai on 9/3/2024.
//


class OAuthUtils {
public:
    static inline std::string sEndpoint = "https://dllserver.solstice.works/";

    static bool hasValidToken();
    static std::string getToken();
    static std::string getLatestCommitHash();
    static std::vector<std::string> getCommitsBetweenHash(const std::string& startHash, const std::string& endHash);
    static std::string getLastCommitHash();
    static void saveCommitHash(const std::string& commitHash);
};