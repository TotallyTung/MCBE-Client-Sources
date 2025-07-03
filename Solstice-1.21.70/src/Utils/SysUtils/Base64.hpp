#pragma once
//
// Created by vastrakai on 7/26/2024.
//

#include <string>

class Base64 {
public:
    static std::string encode(const std::string& input);
    static std::string decode(const std::string& input);
    static std::string encodeBytes(std::vector<uint8_t>& input);
    static std::vector<uint8_t> decodeBytes(const std::string& input);
};