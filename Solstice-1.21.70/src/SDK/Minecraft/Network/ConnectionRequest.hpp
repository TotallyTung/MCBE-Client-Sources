#pragma once
//
// Created by vastrakai on 8/20/2024.
//

#include <memory>
#include <string>
#include <nlohmann/json.hpp>
#include <SDK/Minecraft/JSON.hpp>
#include <Utils/StringUtils.hpp>


class WebToken
{
public:
    std::string mHeader;
    MinecraftJson::Value mHeaderInfo;
    std::string mData;
    MinecraftJson::Value mDataInfo;
    std::string mSignature;
};

class UnverifiedCertificate
{
public:
    class WebToken const                   mRawToken;
    std::unique_ptr<UnverifiedCertificate> mParentUnverifiedCertificate;
};

class Certificate
{
public:
    class UnverifiedCertificate  mUnverifiedCertificate;
    std::unique_ptr<Certificate> mParentCertificate;
    bool                         mIsValid;
};


class ConnectionRequest {
public:
    std::unique_ptr<UnverifiedCertificate> mCertificateData;
    std::unique_ptr<Certificate>           mCertificate;
    std::unique_ptr<WebToken>              mRawToken;
    unsigned  char                         mClientSubId;

    std::string toString()
    {
        // Return with info about each field
        std::string result = "ConnectionRequest:\n";
        // Show da mRawToken info
        result += "mRawToken:\n";
        result += "mHeader: " + mRawToken->mHeader + "\n";
        result += "mHeaderInfo: " + mRawToken->mHeaderInfo.toString() + "\n";
        result += "mData: " + mRawToken->mData + "\n";
        result += "mDataInfo: " + mRawToken->mDataInfo.toString() + "\n";
        result += "mSignature: " + mRawToken->mSignature + "\n";
        // Show da mCertificateData info
        return result;
    }

    [[nodiscard]] nlohmann::json toJson() const
    {
        std::string jsonStr = mRawToken->mData;
        jsonStr = StringUtils::fromBase64(jsonStr);
        nlohmann::json result = nlohmann::json::parse(jsonStr);
        return result;
    }

    void fromJson(nlohmann::json const& json) const
    {
        std::string jsonStr = json.dump();
        jsonStr = StringUtils::toBase64(jsonStr);
        mRawToken->mData = jsonStr;
    }

    void generateSignature()
    {
        // Generate signature

    }
};