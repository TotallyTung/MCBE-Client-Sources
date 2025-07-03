//
// Created by vastrakai on 7/21/2024.
//

#include "HWUtils.hpp"

mce::UUID HWUtils::getHWID()
{
    return GET_HWID();
}

mce::UUID HWUtils::getCpuInfo() {
    std::array<int, 4> cpuid_data;
    std::vector<std::string> hwid_parts;

    __cpuid(cpuid_data.data(), 0);
    char vendor[13];
    std::memcpy(vendor, &cpuid_data[1], 4);
    std::memcpy(vendor + 4, &cpuid_data[3], 4);
    std::memcpy(vendor + 8, &cpuid_data[2], 4);
    vendor[12] = '\0';

    hwid_parts.push_back(std::string(vendor));

    __cpuid(cpuid_data.data(), 1);
    int model = (cpuid_data[0] >> 4) & 0xF;
    int family = (cpuid_data[0] >> 8) & 0xF;
    int stepping = cpuid_data[0] & 0xF;

    hwid_parts.push_back(std::to_string(family));
    hwid_parts.push_back(std::to_string(model));
    hwid_parts.push_back(std::to_string(stepping));

    std::string hwid_str;
    for (const auto& part : hwid_parts) {
        hwid_str += part;
    }

    return HWUtils::hashfnv1a(hwid_str);
}

