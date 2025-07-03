#include "mce.h"
#include "SHA256.h"

mce::UUID mce::UUID::fromString(const std::string& str)
{
    UUID uuid;
    std::string hashed = SHA256::hash(str);
    uuid.mLow = std::stoull(hashed.substr(0, 16), nullptr, 16);
    uuid.mHigh = std::stoull(hashed.substr(16, 16), nullptr, 16);
    return uuid;
}
