
#pragma once
#include <vector>
#include <string>
#include <string_view>
#include <cctype>
//
// Created by vastrakai on 6/28/2024.
//

#define STRING_EXPAND(s) #s
#define STRING(s) std::string(STRING_EXPAND(s))

class StringUtils {
public:
    static bool startsWith(std::string_view str, std::string_view prefix);
    static bool endsWith(std::string_view str, std::string_view suffix);
    static std::string_view trim(std::string_view str);
    static std::vector<std::string> split(std::string_view str, char delimiter);
    static std::string toLower(std::string str);
    static std::string toUpper(std::string str);
    static bool equalsIgnoreCase(const std::string& str1, const std::string& str2);
    static std::string generateUUID(int index);
    static std::string generateMboard(int index);
    static int64_t generateCID();
    static bool containsIgnoreCase(const std::string& str, const std::string& subStr);
    static bool containsAnyIgnoreCase(const std::string& str, const std::vector<std::string>& strVector);
    static std::string getClipboardText();
    static std::string join(const std::vector<std::string>& strings, const std::string& delimiter);
    static std::string replaceAll(std::string& string, const std::string& from, const std::string& to);
    static std::string RemoveColorCodes(std::string str);
    // hashing functions
    static std::string sha256(const std::string& str);
    static std::string fromBase64(const std::string& str);
    static std::string toBase64(const std::string& str);
    static std::string getRelativeTime(std::chrono::system_clock::time_point time);
    static std::string encode(const std::string& str);
    static std::string decode(const std::string& str);
    static std::string encrypt(const std::string& str, const std::string& key);
    static std::string decrypt(const std::string& str, const std::string& key);
    static std::string toHex(const std::vector<uint8_t>& data);
    static bool contains(const std::string& str, const std::string& subStr);
    static std::string replace(const std::string& str, const std::string& from, const std::string& to);
    static std::string randomString(int length);
};


