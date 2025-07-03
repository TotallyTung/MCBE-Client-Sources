#pragma once
//
// Created by vastrakai on 7/21/2024.
//

#include <SDK/Minecraft/mce.hpp>
#include <array>
#include <string>
#include <memory>
#include <stdexcept>
#include <cstdio>
#include <intrin.h>

class HWUtils {
public:
    static mce::UUID getHWID();
    static mce::UUID hashfnv1a(const std::string& str) {
        const uint64_t fnv_prime = 1099511628211;
        const uint64_t fnv_offset_basis = 14695981039346656037;

        uint64_t hash = fnv_offset_basis;
        for (const char& c : str) {
            hash ^= c;
            hash *= fnv_prime;
        }

        // convert this uint64 hash to a uuid
        mce::UUID uuid;
        uuid.mHigh = hash;
        uuid.mLow = hash;
        return uuid;
    }
    static mce::UUID getCpuInfo();
};



#define GET_HWID() \
    []() -> mce::UUID { \
        std::vector<std::string> hwid_parts; \
        hwid_parts.push_back(RegUtils::readReg(std::string(xorstr_("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0")), std::string(xorstr_("ProcessorNameString")), std::string(xorstr_("Unknown")))); \
        hwid_parts.push_back(RegUtils::readReg(std::string(xorstr_("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0")), std::string(xorstr_("Identifier")), std::string(xorstr_("Unknown")))); \
        hwid_parts.push_back(RegUtils::readReg(std::string(xorstr_("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0")), std::string(xorstr_("VendorIdentifier")), std::string(xorstr_("Unknown")))); \
        hwid_parts.push_back(RegUtils::readReg(std::string(xorstr_("HARDWARE\\DESCRIPTION\\System\\BIOS")), std::string(xorstr_("BaseBoardProduct")), std::string(xorstr_("Unknown")))); \
        hwid_parts.push_back(RegUtils::readReg(std::string(xorstr_("HARDWARE\\DESCRIPTION\\System\\BIOS")), std::string(xorstr_("BIOSVendor")), std::string(xorstr_("Unknown")))); \
        hwid_parts.push_back(RegUtils::readReg(std::string(xorstr_("HARDWARE\\DESCRIPTION\\System\\BIOS")), std::string(xorstr_("SystemManufacturer")), std::string(xorstr_("Unknown")))); \
        hwid_parts.push_back(RegUtils::readReg(std::string(xorstr_("HARDWARE\\DESCRIPTION\\System\\BIOS")), std::string(xorstr_("SystemProductName")), std::string(xorstr_("Unknown")))); \
        hwid_parts.push_back(RegUtils::readReg(std::string(xorstr_("HARDWARE\\DESCRIPTION\\System\\BIOS")), std::string(xorstr_("SystemSKU")), std::string(xorstr_("Unknown")))); \
        hwid_parts.push_back(RegUtils::readReg(std::string(xorstr_("HARDWARE\\DESCRIPTION\\System\\BIOS")), std::string(xorstr_("SystemFamily")), std::string(xorstr_("Unknown")))); \
        hwid_parts.push_back(RegUtils::readReg(std::string(xorstr_("SYSTEM\\CurrentControlSet\\Services\\Disk\\Enum")), std::string(xorstr_("0")), std::string(xorstr_("Unknown")))); \
        auto installTime = RegUtils::readReg<uint64_t>(std::string(xorstr_("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion")), std::string(xorstr_("InstallDate")), 0); \
        hwid_parts.push_back(fmt::format(xorstr_("{:x}"), installTime)); \
        hwid_parts.push_back(RegUtils::readReg(std::string(xorstr_("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion")), std::string(xorstr_("ProductId")), std::string(xorstr_("Unknown")))); \
        std::string hwid_str; \
        for (const auto& part : hwid_parts) { \
            hwid_str += part; \
        } \
        return HWUtils::hashfnv1a(hwid_str); \
    }()