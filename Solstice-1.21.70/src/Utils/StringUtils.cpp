//
// Created by vastrakai on 6/28/2024.
//

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

#include "StringUtils.hpp"

#include <algorithm>
#include <hex.h>
#include <windows.h>
#include <regex>
#include "spdlog/spdlog.h"
#include <cryptopp/files.h>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/base64.h>
#include <cryptopp/md5.h>
#include <cryptopp/hex.h>
#include <random>
#include <md5.h>
#include <cryptopp/osrng.h>
#include <sstream>

#include "SysUtils/SHA256.hpp"

std::vector<std::string> motherboardModels = {
        "ASUS ROG Strix Z590-E",
        "MSI MPG Z490 Gaming Edge WiFi",
        "Gigabyte Z590 AORUS Master",
        "ASRock B450M PRO4",
        "ASUS TUF Gaming X570-PLUS",
        "MSI B450 TOMAHAWK MAX",
        "Gigabyte B550 AORUS Elite",
        "ASRock Z490 Phantom Gaming 4",
        "ASUS Prime B560M-A",
        "MSI MAG B550 TOMAHAWK",
        "Gigabyte B450M DS3H",
        "ASRock H570M Pro4",
        "ASUS ROG Crosshair VIII Hero",
        "MSI MPG Z390 Gaming PRO Carbon",
        "Gigabyte Z390 AORUS ULTRA",
        "ASRock B365M PRO4",
        "ASUS Prime Z490-P",
        "MSI MAG Z490 TOMAHAWK",
        "Gigabyte Z490 VISION G",
        "ASRock B460M Steel Legend",
        "ASUS ROG Strix B450-F Gaming",
        "MSI MPG B550 Gaming Plus",
        "Gigabyte Z390 GAMING X",
        "ASRock H410M-HDV",
        "ASUS TUF Z390-PLUS GAMING",
        "MSI B450 GAMING PLUS MAX",
        "Gigabyte B460M DS3H",
        "ASRock X570 Phantom Gaming 4",
        "ASUS Prime B365M-A",
        "MSI Z370 GAMING PLUS",
        "Gigabyte X570 AORUS ELITE",
        "ASRock B550M Steel Legend",
        "ASUS ROG Strix X570-E Gaming",
        "MSI MEG Z490 GODLIKE",
        "Gigabyte A520M DS3H",
        "ASRock Z390 Phantom Gaming 9",
        "ASUS TUF B450-PLUS GAMING",
        "MSI B365M PRO-VDH",
        "Gigabyte Z370 HD3P",
        "ASRock H310M-HDV",
        "ASUS Prime A320M-K",
        "MSI H310M PRO-VDH PLUS",
        "Gigabyte B450 I AORUS PRO WIFI",
        "ASRock B450M Steel Legend",
        "ASUS ROG Maximus XII Hero",
        "MSI MPG Z590 Gaming Carbon WiFi",
        "Gigabyte Z590 AORUS PRO AX",
        "ASRock B365 Phantom Gaming 4",
        "ASUS TUF Gaming B460-PRO",
        "MSI Z490-A PRO",
        "Gigabyte Z390 AORUS PRO WIFI",
        "ASRock H410M-HDV/M.2",
        "ASUS Prime Z390-P",
        "MSI MEG X570 UNIFY",
        "Gigabyte B365M DS3H",
        "ASRock X470 Taichi",
        "ASUS ROG Strix Z490-E Gaming",
        "MSI MAG B460M MORTAR WIFI",
        "Gigabyte Z490M GAMING X",
        "ASRock B460 Steel Legend",
        "ASUS Prime H310M-E R2.0",
        "MSI B450I GAMING PLUS AC",
        "Gigabyte H370 HD3",
        "ASRock B360M Pro4",
        "ASUS TUF Gaming B550M-PLUS",
        "MSI MPG Z490 GAMING EDGE WIFI",
        "Gigabyte B360 AORUS GAMING 3 WIFI",
        "ASRock Z370 Killer SLI",
        "ASUS Prime H310M-D R2.0",
        "MSI Z390-A PRO",
        "Gigabyte B365M D3H",
        "ASRock B365M Phantom Gaming 4",
        "ASUS TUF B360-PRO Gaming",
        "MSI H310M PRO-M2 PLUS",
        "Gigabyte Z370 AORUS ULTRA GAMING WIFI",
        "ASRock H370M-ITX/ac",
        "ASUS Prime B250M-A",
        "MSI B360 GAMING PLUS",
        "Gigabyte H310M S2H",
        "ASRock B250M Pro4",
        "ASUS TUF Z370-PLUS Gaming",
        "MSI B450M GAMING PLUS",
        "Gigabyte B460M GAMING HD",
        "ASRock Z370 Extreme4",
        "ASUS Prime Z270-A",
        "MSI H270 GAMING M3",
        "Gigabyte H270-HD3",
        "ASRock B250M-HDV",
        "ASUS Prime H270-PLUS",
        "MSI B250M Mortar",
        "Gigabyte GA-Z270X-Gaming K5",
        "ASRock H110M-DGS",
        "ASUS Prime A320M-E",
        "MSI H110M PRO-D",
        "Gigabyte GA-H110M-A",
        "ASRock Z170 Extreme6",
        "ASUS Z170-A",
        "MSI Z170A GAMING M5",
        "Gigabyte GA-Z170X-Gaming 7",
        "ASRock H97M Pro4",
        "ASUS H97-PLUS",
        "MSI H97 GAMING 3",
        "Gigabyte GA-H97-D3H",
        "ASUS Prime Z270-A",
        "MSI B350 TOMAHAWK",
        "Gigabyte GA-AX370-Gaming K7",
        "ASRock AB350M Pro4",
        "ASUS ROG Crosshair VI Hero",
        "MSI X470 GAMING PRO CARBON",
        "Gigabyte GA-AB350-Gaming 3",
        "ASRock X399 Taichi",
        "ASUS ROG Zenith Extreme",
        "MSI X399 GAMING PRO CARBON AC",
        "Gigabyte X299 AORUS Gaming 7",
        "ASRock X299 Taichi",
        "ASUS Prime X299-DELUXE",
        "MSI X299 SLI PLUS",
        "Gigabyte Z270X-Ultra Gaming",
        "ASRock Z270 Taichi",
        "ASUS ROG Maximus IX Hero",
        "MSI Z270 GAMING PRO CARBON",
        "Gigabyte GA-H270-Gaming 3",
        "ASRock H270M Pro4",
        "ASUS Prime H270M-PLUS",
        "MSI H270 TOMAHAWK",
        "Gigabyte GA-B250M-DS3H",
        "ASRock B250M Pro4",
        "ASUS Prime B250M-PLUS",
        "MSI B250M BAZOOKA",
        "Gigabyte GA-Z170X-Gaming GT",
        "ASRock Z170 Pro4",
        "ASUS Z170-DELUXE",
        "MSI Z170A KRAIT GAMING 3X",
        "Gigabyte GA-Z170X-UD3",
        "ASRock H110M-ITX/ac",
        "ASUS H110M-E/M.2",
        "MSI H110M GAMING",
        "Gigabyte GA-H110M-S2H",
        "ASRock H81M-HDS",
        "ASUS H81M-A",
        "MSI H81M-P33",
        "Gigabyte GA-H81M-DS2V",
        "ASRock Z97 Extreme4",
        "ASUS Z97-PRO",
        "MSI Z97 GAMING 5",
        "Gigabyte GA-Z97X-UD5H",
        "ASRock H97 Anniversary",
        "ASUS H97M-E",
        "MSI H97 PC MATE",
        "Gigabyte GA-H97N-WIFI",
        "ASRock B85M Pro4",
        "ASUS B85M-G",
        "MSI B85-G43 GAMING",
        "Gigabyte GA-B85M-DS3H",
        "ASRock Z87 Extreme4",
        "ASUS Z87-A",
        "MSI Z87-G45 GAMING",
        "Gigabyte GA-Z87X-UD3H",
        "ASRock H87 Pro4",
        "ASUS H87M-E",
        "MSI H87-G43",
        "Gigabyte GA-H87-D3H",
        "ASRock B75 Pro3",
        "ASUS P8B75-M",
        "MSI B75MA-P45",
        "Gigabyte GA-B75M-D3H",
        "ASRock Z77 Extreme4",
        "ASUS P8Z77-V",
        "MSI Z77A-G45",
        "Gigabyte GA-Z77X-UD3H",
        "ASRock H61M-HVS",
        "ASUS P8H61-M LX3",
        "MSI H61M-P31/W8",
        "Gigabyte GA-H61M-DS2",
        "ASRock A320M-HDV",
        "ASUS PRIME A320M-C R2.0",
        "MSI A320M PRO-VD/S",
        "Gigabyte GA-A320M-S2H",
        "ASRock FM2A68M-HD+",
        "ASUS A68HM-K",
        "MSI A68HM GRENADE",
        "Gigabyte GA-F2A68HM-S1",
        "ASRock 970M Pro3",
        "ASUS M5A97 LE R2.0",
        "MSI 970 GAMING",
        "Gigabyte GA-970A-DS3P",
        "ASRock 990FX Extreme6",
        "ASUS SABERTOOTH 990FX R2.0",
        "MSI 990FXA GAMING",
        "Gigabyte GA-990FXA-UD3",
        "ASRock A88M-G/3.1",
        "ASUS A88XM-A",
        "MSI A88XM GAMING",
        "Gigabyte GA-F2A88XM-D3H",
        "ASRock H61M-VG3",
        "ASUS H61M-K",
        "MSI H61M-E33/W8",
        "Gigabyte GA-H61M-USB3V",
        "ASRock Q1900-ITX",
        "ASUS J1800I-C",
        "MSI C847MS-E33",
        "Gigabyte GA-J1800N-D2P"
};

std::string generateMotherboardPC() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> modelDistr(0, motherboardModels.size() - 1);

    int modelIndex = modelDistr(gen);
    std::string mboard = motherboardModels[modelIndex];

    return mboard;
}

std::vector<std::string> motherboardsAndroid = {
        "Samsung Galaxy S21",
        "Samsung Galaxy S21+",
        "Samsung Galaxy S21 Ultra",
        "Samsung Galaxy Note 20",
        "Samsung Galaxy Note 20 Ultra",
        "Samsung Galaxy A52",
        "Samsung Galaxy A72",
        "Samsung Galaxy M32",
        "Samsung Galaxy M42",
        "Samsung Galaxy F62",
        "Samsung Galaxy Z Fold 2",
        "Samsung Galaxy Z Flip",
        "OnePlus 9",
        "OnePlus 9 Pro",
        "OnePlus 8T",
        "OnePlus 8 Pro",
        "OnePlus Nord",
        "OnePlus Nord N10 5G",
        "OnePlus Nord N100",
        "Google Pixel 5",
        "Google Pixel 4a",
        "Google Pixel 4a 5G",
        "Google Pixel 4",
        "Google Pixel 4 XL",
        "Google Pixel 3a",
        "Google Pixel 3a XL",
        "Xiaomi Mi 11",
        "Xiaomi Mi 11 Ultra",
        "Xiaomi Mi 11X",
        "Xiaomi Mi 10T",
        "Xiaomi Mi 10T Pro",
        "Xiaomi Mi 10",
        "Xiaomi Mi 10 Pro",
        "Xiaomi Redmi Note 10",
        "Xiaomi Redmi Note 10 Pro",
        "Xiaomi Redmi Note 10S",
        "Xiaomi Redmi Note 9",
        "Xiaomi Redmi Note 9 Pro",
        "Xiaomi Redmi Note 8",
        "Xiaomi Redmi Note 8 Pro",
        "Sony Xperia 5 II",
        "Sony Xperia 1 II",
        "Sony Xperia 10 II",
        "Sony Xperia 10 III",
        "Sony Xperia L4",
        "LG Wing",
        "LG Velvet",
        "LG V60 ThinQ",
        "LG G8X ThinQ",
        "LG K92 5G",
        "LG Stylo 6",
        "Nokia 8.3 5G",
        "Nokia 7.2",
        "Nokia 5.4",
        "Nokia 3.4",
        "Nokia 2.4",
        "Nokia 1.4",
        "Motorola Edge Plus",
        "Motorola Edge",
        "Motorola Moto G100",
        "Motorola Moto G60",
        "Motorola Moto G50",
        "Motorola Moto G30",
        "Motorola Moto G10",
        "Motorola Moto E7 Plus",
        "Motorola Moto E6i",
        "Oppo Find X3 Pro",
        "Oppo Find X3 Neo",
        "Oppo Find X3 Lite",
        "Oppo Reno5 Pro 5G",
        "Oppo Reno5 5G",
        "Oppo A94 5G",
        "Oppo A74 5G",
        "Oppo A54 5G",
        "Oppo A53",
        "Vivo X60 Pro",
        "Vivo X60",
        "Vivo Y72 5G",
        "Vivo Y52 5G",
        "Vivo Y20s",
        "Vivo Y12s",
        "Realme GT",
        "Realme 8 Pro",
        "Realme 8",
        "Realme 7 Pro",
        "Realme 7",
        "Realme Narzo 30 Pro",
        "Realme Narzo 30A",
        "Realme C21",
        "Realme C20",
        "Asus ROG Phone 5",
        "Asus Zenfone 7",
        "Asus Zenfone 7 Pro",
        "Asus Zenfone 6",
        "Asus ROG Phone 3",
        "Poco F3",
        "Poco X3 Pro",
        "Poco M3 Pro 5G",
        "Poco M2 Pro",
        "Poco X2",
        "ZTE Axon 30",
        "ZTE Blade V2021 5G",
        "ZTE Blade A7s 2020",
        "ZTE Blade A3Y",
        "ZTE Nubia Red Magic 5S",
        "Honor 50",
        "Honor 50 Pro",
        "Honor View 40",
        "Honor 30 Pro+",
        "Honor 30 Pro",
        "Honor 30",
        "Honor 30S",
        "Honor 20 Pro",
        "Honor 20",
        "Honor 20 Lite",
        "Huawei P40 Pro",
        "Huawei P40",
        "Huawei P40 Lite",
        "Huawei Mate 40 Pro",
        "Huawei Mate 40",
        "Huawei Mate 30 Pro",
        "Huawei Mate 30",
        "Huawei P30 Pro",
        "Huawei P30",
        "Huawei P30 Lite",
        "Huawei Nova 7i",
        "Huawei Nova 7 SE",
        "Huawei Y9a",
        "Huawei Y8p",
        "Huawei Y7a",
        "Samsung Galaxy A32",
        "Samsung Galaxy A22",
        "Samsung Galaxy A12",
        "Samsung Galaxy A02s",
        "Samsung Galaxy M62",
        "Samsung Galaxy M21",
        "Samsung Galaxy M12",
        "Samsung Galaxy F41",
        "Samsung Galaxy F22",
        "Samsung Galaxy F12",
        "Samsung Galaxy Z Flip 3",
        "Samsung Galaxy Z Fold 3",
        "OnePlus 9R",
        "OnePlus 8",
        "OnePlus 7T",
        "OnePlus 7 Pro",
        "Google Pixel 3",
        "Google Pixel 3 XL",
        "Google Pixel 2",
        "Google Pixel 2 XL",
        "Xiaomi Mi Mix Fold",
        "Xiaomi Mi 10 Lite",
        "Xiaomi Redmi 9",
        "Xiaomi Redmi 9T",
        "Xiaomi Redmi Note 9T",
        "Xiaomi Redmi K40",
        "Xiaomi Redmi K30",
        "Sony Xperia 10",
        "Sony Xperia L3",
        "LG G8 ThinQ",
        "LG V50 ThinQ",
        "Nokia 6.2",
        "Nokia 5.3",
        "Motorola Moto G8 Plus",
        "Motorola Moto G8 Power",
        "Oppo A91",
        "Oppo A9 2020",
        "Vivo V21",
        "Vivo V20",
        "Realme X50 Pro",
        "Realme X3 SuperZoom",
        "Realme 6 Pro",
        "Realme 6",
        "Asus ROG Phone 2",
        "Asus Zenfone 5Z",
        "Poco X3 NFC",
        "Poco F2 Pro",
        "ZTE Nubia Red Magic 3",
        "Honor 10",
        "Honor 9X Pro",
        "Huawei Mate X2",
        "Huawei Y6p",
        "Huawei Y5p"
};

std::string generateDeviceAndroid() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> modelDistr(0, motherboardsAndroid.size() - 1);

    int modelIndex = modelDistr(gen);
    std::string mboard = motherboardsAndroid[modelIndex];

    std::transform(mboard.begin(), mboard.end(), mboard.begin(), ::toupper);

    return mboard;
}

std::vector<std::string> modelsiOS = {
        "iPhone5,3", "iPhone5,4", "iPhone6,1", "iPhone6,2", "iPhone7,2", "iPhone7,1",
        "iPhone8,1", "iPhone8,2", "iPhone8,4", "iPhone9,1", "iPhone9,3", "iPhone9,2",
        "iPhone9,4", "iPhone10,1", "iPhone10,4", "iPhone10,2", "iPhone10,5", "iPhone10,3",
        "iPhone10,6", "iPhone11,8", "iPhone11,2", "iPhone11,6", "iPhone12,1", "iPhone12,3",
        "iPhone12,5", "iPhone12,8", "iPhone13,1", "iPhone13,2", "iPhone13,3", "iPhone13,4",
        "iPhone14,4", "iPhone14,5", "iPhone14,2", "iPhone14,3", "iPhone14,6", "iPhone14,7",
        "iPhone14,8", "iPhone15,2", "iPhone15,3", "iPhone15,4", "iPhone15,5", "iPhone16,1",
        "iPhone16,2", "iPad6,11", "iPad6,12", "iPad7,5", "iPad7,6", "iPad7,11", "iPad7,12",
        "iPad11,6", "iPad11,7", "iPad12,1", "iPad12,2", "iPad13,18", "iPad13,19", "iPad4,4",
        "iPad4,5", "iPad4,6", "iPad4,7", "iPad4,8", "iPad4,9", "iPad5,1", "iPad5,2", "iPad11,1",
        "iPad11,2", "iPad14,1", "iPad14,2", "iPad4,1", "iPad4,2", "iPad4,3", "iPad5,3", "iPad5,4",
        "iPad11,3", "iPad11,4", "iPad13,1", "iPad13,2", "iPad13,16", "iPad13,17", "iPad6,3", "iPad6,4",
        "iPad7,3", "iPad7,4", "iPad8,1", "iPad8,2", "iPad8,3", "iPad8,4", "iPad8,9", "iPad8,10", "iPad13,4",
        "iPad13,5", "iPad13,6", "iPad13,7", "iPad14,3", "iPad14,4", "iPad6,7", "iPad6,8", "iPad7,1", "iPad7,2",
        "iPad8,5", "iPad8,6", "iPad8,7", "iPad8,8", "iPad8,11", "iPad8,12", "iPad13,8", "iPad13,9", "iPad13,10",
        "iPad13,11", "iPad14,5", "iPad14,6"
};

std::string generateDeviceiOS() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> modelDistr(0, modelsiOS.size() - 1);

    int modelIndex = modelDistr(gen);

    return modelsiOS[modelIndex];
}

std::string StringUtils::generateMboard(int index) {
    switch (index)
    {
        case 1:
            return generateDeviceAndroid();
        case 2:
            return generateDeviceiOS();
        case 7:
            return generateMotherboardPC();
        default:
            return generateMotherboardPC();
    }
}

static std::string namespaceUUID = "1234567890abcdef1234567890abcdef";

std::string generateRandomName(size_t length) {
    static const char alphabet[] =
            "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_int_distribution<size_t> dist(0, sizeof(alphabet) - 2);

    std::string name;
    name.reserve(length);
    for (size_t i = 0; i < length; ++i) {
        name += alphabet[dist(rng)];
    }

    return name;
}

std::string generateUUID3() {
    std::array<uint8_t, 16> namespaceBytes;
    for (size_t i = 0; i < 16; ++i) {
        namespaceBytes[i] = static_cast<uint8_t>(std::stoi(namespaceUUID.substr(i * 2, 2), nullptr, 16));
    }

    std::string combined(namespaceBytes.begin(), namespaceBytes.end());
    std::string name = generateRandomName(32);
    combined += name;

    CryptoPP::Weak::MD5 hash;
    std::array<uint8_t, CryptoPP::Weak::MD5::DIGESTSIZE> digest;
    hash.CalculateDigest(digest.data(), reinterpret_cast<const uint8_t*>(combined.data()), combined.size());

    digest[6] = (digest[6] & 0x0F) | 0x30;
    digest[8] = (digest[8] & 0x3F) | 0x80;

    std::ostringstream uuid;
    uuid << std::hex << std::setfill('0');
    for (size_t i = 0; i < 16; ++i) {
        if (i == 4 || i == 6 || i == 8 || i == 10) {
            uuid << '-';
        }
        uuid << std::setw(2) << static_cast<int>(digest[i]);
    }

    return uuid.str();
}

std::string generateUUID4() {
    CryptoPP::AutoSeededRandomPool prng;

    unsigned char randomBytes[16];
    prng.GenerateBlock(randomBytes, sizeof(randomBytes));

    randomBytes[6] = (randomBytes[6] & 0x0F) | 0x40;
    randomBytes[8] = (randomBytes[8] & 0x3F) | 0x80;

    std::stringstream uuidStream;
    uuidStream << std::hex;

    for (int i = 0; i < 16; ++i) {
        uuidStream << (int)(randomBytes[i] >> 4);
        uuidStream << (int)(randomBytes[i] & 0x0F);
    }

    return uuidStream.str();
}

std::string StringUtils::generateUUID(int index) {
    std::string uuid;

    switch (index)
    {
        case 1: // android
            uuid = generateUUID4();
            return uuid;
            break;
        case 2: // ios
            uuid = toUpper(generateUUID4());
            return uuid;
            break;
        case 7: // windows
            uuid = generateUUID3();
            return uuid;
            break;
        default:
            std::string name = generateRandomName(32);
            uuid = generateUUID3();
            return uuid;
            break;
    }
}

int64_t StringUtils::generateCID() {
    static std::random_device rd;
    static std::mt19937_64 gen(rd());
    static std::uniform_int_distribution<uint64_t> dis(1000000000000000000ULL, 9999999999999999999ULL);
    static std::uniform_int_distribution<int> sign_dis(0, 1);

    uint64_t id = dis(gen);
    if (sign_dis(gen) == 1) {
        return static_cast<int64_t>(-id);
    } else {
        return static_cast<int64_t>(id);
    }
}

bool StringUtils::startsWith(std::string_view str, std::string_view prefix)
{
    return str.substr(0, prefix.size()) == prefix;
}

bool StringUtils::endsWith(std::string_view str, std::string_view suffix)
{
    return str.substr(str.size() - suffix.size(), suffix.size()) == suffix;
}

std::string StringUtils::RemoveColorCodes(std::string str) {
    return std::regex_replace(std::regex_replace(str, std::regex("§."), ""), std::regex("&."), "");
}

std::string_view StringUtils::trim(std::string_view str)
{
    size_t start = 0;
    size_t end = str.size();

    while (start < end && std::isspace(str[start])) {
        start++;
    }

    while (end > start && std::isspace(str[end - 1])) {
        end--;
    }

    return str.substr(start, end - start);
}

std::vector<std::string> StringUtils::split(std::string_view str, char delimiter)
{
    std::vector<std::string> result;
    size_t start = 0;
    size_t end = 0;

    while ((end = str.find(delimiter, start)) != std::string::npos)
    {
        result.emplace_back(str.substr(start, end - start));
        start = end + 1;
    }

    result.emplace_back(str.substr(start));

    return result;
}

// toLower and toUpper
std::string StringUtils::toLower(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
    return str;
}

std::string StringUtils::toUpper(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::toupper(c); });
    return str;
}

bool StringUtils::equalsIgnoreCase(const std::string& str1, const std::string& str2)
{
    return toLower(str1) == toLower(str2);
}

bool StringUtils::containsIgnoreCase(const std::string& str, const std::string& subStr)
{
    return toLower(str).find(toLower(subStr)) != std::string::npos;
}

bool StringUtils::containsAnyIgnoreCase(const std::string& str, const std::vector<std::string>& strVector)
{
    if (std::ranges::any_of(strVector, [&str](const std::string& subStr) { return containsIgnoreCase(str, subStr); }))
    {
        return true;
    }

    return false;
}

std::string StringUtils::getClipboardText()
{
    // Try opening the clipboard
    if (! OpenClipboard(nullptr))
    {
        spdlog::error("Failed to open clipboard: {}", GetLastError());
        return "";
    }

    // Get handle of clipboard object for ANSI text
    HANDLE hData = GetClipboardData(CF_TEXT);
    if (hData == nullptr)
    {
        spdlog::error("Failed to get clipboard text: {}", GetLastError());
        return "";
    }

    // Lock the handle to get the actual text pointer
    char * pszText = static_cast<char*>( GlobalLock(hData) );
    if (pszText == nullptr)
    {
        spdlog::error("Failed to get clipboard text: {}", GetLastError());
        return "";
    }

    // Save text in a string class instance
    std::string text( pszText );

    // Release the lock
    GlobalUnlock( hData );

    // Release the clipboard
    CloseClipboard();

    return text;
}

std::string StringUtils::join(const std::vector<std::string>& strings, const std::string& delimiter)
{
    std::string result;
    for (size_t i = 0; i < strings.size(); i++)
    {
        result += strings[i];
        if (i != strings.size() - 1)
        {
            result += delimiter;
        }
    }

    return result;
}

std::string StringUtils::replaceAll(std::string& string, const std::string& from, const std::string& to)
{
    size_t start_pos = 0;
    while ((start_pos = string.find(from, start_pos)) != std::string::npos)
    {
        string.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }

    return string;
}

std::string StringUtils::sha256(const std::string& str)
{
    return SHA256::hash(str);
}

std::string StringUtils::fromBase64(const std::string& str)
{
    return Base64::decode(str);
}

std::string StringUtils::toBase64(const std::string& str)
{
    return Base64::encode(str);
}

std::string StringUtils::getRelativeTime(std::chrono::system_clock::time_point time)
{
    auto now = std::chrono::system_clock::now();

    auto diff = now - time;

    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(diff);
    if (seconds.count() < 60) return std::to_string(seconds.count()) + " seconds ago";

    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(diff);
    if (minutes.count() < 60) return std::to_string(minutes.count()) + " minutes ago";

    auto hours = std::chrono::duration_cast<std::chrono::hours>(diff);
    if (hours.count() < 24) return std::to_string(hours.count()) + " hours ago";

    auto days = std::chrono::duration_cast<std::chrono::days>(diff);
    if (days.count() < 30) return std::to_string(days.count()) + " days ago";

    auto months = std::chrono::duration_cast<std::chrono::months>(diff);
    if (months.count() < 12) return std::to_string(months.count()) + " months ago";

    auto years = std::chrono::duration_cast<std::chrono::years>(diff);
    return std::to_string(years.count()) + " years ago";
}

/*
public class StringCipher
{
    public static string Encode(string input)
    {
        byte[] bytes = Encoding.UTF8.GetBytes(input);

        for (int i = 0; i < bytes.Length - 1; i += 2) (bytes[i], bytes[i + 1]) = (bytes[i + 1], bytes[i]);
        for (int i = 0; i < bytes.Length; i++) bytes[i] = (byte)~bytes[i];

        return Convert.ToBase64String(bytes);
    }

    public static string Decode(string input)
    {
        byte[] bytes = Convert.FromBase64String(input);

        for (int i = 0; i < bytes.Length; i++) bytes[i] = (byte)~bytes[i];
        for (int i = 0; i < bytes.Length - 1; i += 2) (bytes[i], bytes[i + 1]) = (bytes[i + 1], bytes[i]);

        return Encoding.UTF8.GetString(bytes);
    }
}
*/




std::string StringUtils::decode(const std::string& str)
{
    std::vector<uint8_t> bytes = Base64::decodeBytes(str);

    for (auto& byte : bytes) {
        byte = ~byte;
    }

    for (size_t i = 0; i < bytes.size() - 1; i += 2) {
        std::swap(bytes[i], bytes[i + 1]);
    }

    return std::string(bytes.begin(), bytes.end());
}

std::string StringUtils::encode(const std::string& input)
{
    std::vector<uint8_t> bytes(input.begin(), input.end());

    for (size_t i = 0; i < bytes.size() - 1; i += 2) {
        std::swap(bytes[i], bytes[i + 1]);
    }

    for (auto& byte : bytes) {
        byte = ~byte;
    }

    return Base64::encodeBytes(bytes);
}
/*
public static string Encrypt(string plaintext, string key)
    {
        // Truncate key to 16 bytes (128-bit)
        byte[] truncatedKey = new byte[16];
        Array.Copy(Encoding.UTF8.GetBytes(key), truncatedKey, Math.Min(16, key.Length));

        // simple encryption
        byte[] plaintextBytes = Encoding.UTF8.GetBytes(plaintext);
        byte[] ciphertextBytes = new byte[plaintextBytes.Length];
        for (int i = 0; i < plaintextBytes.Length; i++)
        {
            ciphertextBytes[i] = (byte)(plaintextBytes[i] ^ truncatedKey[i % 16]);
        }

        return Convert.ToBase64String(ciphertextBytes);
    }

    public static string Decrypt(string ciphertext, string key)
    {
        // Truncate key to 16 bytes (128-bit)
        byte[] truncatedKey = new byte[16];
        Array.Copy(Encoding.UTF8.GetBytes(key), truncatedKey, Math.Min(16, key.Length));

        // simple decryption
        byte[] ciphertextBytes = Convert.FromBase64String(ciphertext);
        byte[] plaintextBytes = new byte[ciphertextBytes.Length];
        for (int i = 0; i < ciphertextBytes.Length; i++)
        {
            plaintextBytes[i] = (byte)(ciphertextBytes[i] ^ truncatedKey[i % 16]);
        }

        return Encoding.UTF8.GetString(plaintextBytes);
    }*/

using namespace CryptoPP;
class EncUtils
{
public:
    static std::string Encrypt(const std::string& plaintext, const std::string& key) {
        // Truncate key to 16 bytes
        std::string truncatedKey = key.substr(0, 16);
        // add 2 to every byte in the key
        for (auto& byte : truncatedKey) {
            byte += 2;
        }

        // Simple encryption
        std::vector<unsigned char> plaintextBytes(plaintext.begin(), plaintext.end());
        std::vector<unsigned char> ciphertextBytes(plaintextBytes.size());

        for (size_t i = 0; i < plaintextBytes.size(); i++) {
            ciphertextBytes[i] = plaintextBytes[i] ^ truncatedKey[i % 16];
        }

        return Base64::encode(std::string(ciphertextBytes.begin(), ciphertextBytes.end()));
    }

    static std::string Decrypt(const std::string& ciphertext, const std::string& key) {
        // Truncate key to 16 bytes
        std::string truncatedKey = key.substr(0, 16);
        // add 2 to every byte in the key
        for (auto& byte : truncatedKey) {
            byte += 2;
        }

        // Simple decryption
        std::vector<unsigned char> ciphertextBytes = Base64::decodeBytes(ciphertext);
        std::vector<unsigned char> plaintextBytes(ciphertextBytes.size());

        for (size_t i = 0; i < ciphertextBytes.size(); i++) {
            plaintextBytes[i] = ciphertextBytes[i] ^ truncatedKey[i % 16];
        }

        return std::string(plaintextBytes.begin(), plaintextBytes.end());
    }
};

std::string StringUtils::encrypt(const std::string& input, const std::string& key) {
    return EncUtils::Encrypt(input, key);
}

std::string StringUtils::decrypt(const std::string& input, const std::string& key) {
    return EncUtils::Decrypt(input, key);
}

std::string StringUtils::toHex(const std::vector<uint8_t>& data)
{
    return ""; // not implemented
}

bool StringUtils::contains(const std::string& str, const std::string& subStr)
{
    return str.find(subStr) != std::string::npos;
}

std::string StringUtils::replace(const std::string& str, const std::string& from, const std::string& to)
{
    // replace only the first occurrence
    std::string result = str;
    size_t start_pos = result.find(from);
    if (start_pos != std::string::npos)
    {
        result.replace(start_pos, from.length(), to);
    }
    return result;
}

std::string StringUtils::randomString(int length)
{
    std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

    std::random_device rd;
    std::mt19937 generator(rd());

    std::ranges::shuffle(str, generator);

    return str.substr(0, length);};
