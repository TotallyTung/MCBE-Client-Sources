//
// Created by vastrakai on 9/30/2024.
//

#include "mce.hpp"

#include <Utils/SysUtils/SHA256.hpp>

mce::UUID mce::UUID::fromString(const std::string& str)
{
    UUID uuid;
    std::string hashed = SHA256::hash(str);
    uuid.mLow = std::stoull(hashed.substr(0, 16), nullptr, 16);
    uuid.mHigh = std::stoull(hashed.substr(16, 16), nullptr, 16);
    return uuid;
}
