//
// Created by Fash on 9/15/2023.
//

#pragma once

#include <string>
#include <vector>

class String {
public:

    static bool ContainsIgnoreCase(std::string str, std::string find);

    static std::vector<std::string> SplitString(std::string str, std::string delimiter);
    static std::vector<std::string> SplitString(std::string str, char delimiter);

    static std::string ToHex(uintptr_t address);

    static std::string ToLower(const std::string &string);

    static std::string ToUpper(const std::string &string);

    static bool IsNumber(std::string str);

    static bool InvalidChar(char c);

    static std::string SanitizeName(const std::string &name);

    static std::string RemoveWhitespace(std::string str);

    static std::string RemoveSymbols(std::string str);

    static std::string ReplaceAll(std::string str, const char *c1, const char *c2);

    static std::string ReplaceAll(const std::string& str, const std::string& from, const std::string& to);

    static std::string ReplaceAll(const std::string& str, const std::vector<std::pair<std::string, std::string>>& replacements);

    static std::string ReplaceFirst(std::string str, const char *c1, const char *c2);

    static std::string GenerateUUID();

    static std::string RemoveColorCodes(std::string str);

    static bool StartsWith(std::string str, std::string str2);

    static std::string FormatTime(int64_t time, const std::string &format);

    static std::string RelativeTime(int64_t time);

    static std::string Base64Decode(const std::string& string);

    static std::vector<uint8_t> Base64DecodeBytes(const std::string& input);

    static std::string BytesToHex(const std::vector<uint8_t>& bytes);

    // Format function
    template<typename ... Args>
    static std::string Format(const char *fmt, Args ... args) {
        size_t len = snprintf(nullptr, 0, fmt, args ...);
        std::vector<char> buf(len + 1);
        snprintf(&buf[0], len + 1, fmt, args ...);
        return std::string(&buf[0], &buf[0] + len);
    }

    static std::string Join(std::vector<std::string> vec, std::string const &string);

    static std::vector<std::string> Split(std::string str, std::string splitter);

    static std::string RemoveNonAscii(std::string& str);

    static std::string Trim(std::string& str);
    static std::string GetLineContaining(const std::string& string, const char* str);
};
