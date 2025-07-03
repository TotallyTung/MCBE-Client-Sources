//
// Created by vastrakai on 7/11/2024.
//

#pragma once

#include <SDK/Minecraft/Network/Packets/Packet.hpp>
#include <SDK/Minecraft/mce.hpp>
#include <map>
#include <vector>

enum class BuildPlatform : int {
    Invalid = -1,  // if we have to switch to decimal ig we can
    Android = 0x1,
    iOS = 0x2,
    OSX = 0x3,
    Amazon = 0x4,
    GearVR = 0x5,
    WIN10 = 0x7,
    Win32 = 0x8,
    Dedicated = 0x9,
    PS4 = 0xB,
    Nx = 0xC,
    Xbox = 0xD,
    WindowsPhone = 0xE,
    Linux = 0xF,
    Unknown = 0xFF,
};

const std::map<BuildPlatform, std::string> BuildPlatformNames = {
    {BuildPlatform::Invalid, "Unknown"},
    {BuildPlatform::Android, "Android"},
    {BuildPlatform::iOS, "iOS"},
    {BuildPlatform::OSX, "macOS"},
    {BuildPlatform::Amazon, "Amazon"},
    {BuildPlatform::GearVR, "GearVR"},
    {BuildPlatform::WIN10, "Windows"},
    {BuildPlatform::Win32, "Win32"},
    {BuildPlatform::Dedicated, "Dedicated"},
    {BuildPlatform::PS4, "Playstation"},
    {BuildPlatform::Nx, "Switch"},
    {BuildPlatform::Xbox, "Xbox"},
    {BuildPlatform::WindowsPhone, "WindowsPhone"},
    {BuildPlatform::Linux, "Linux"},
    {BuildPlatform::Unknown, "Unknown"}
};

/*enum class PlayerListPacketType : signed char {
    Add    = 0x0,
    Remove = 0x1,
};

class PlayerListEntry {
public:
    int64_t        mId;               // this+0x0
    mce::UUID      mUuid;             // this+0x8
    std::string    mName;             // this+0x18
    std::string    mXuid;             // this+0x38
    std::string    mPlatformOnlineId; // this+0x58
    BuildPlatform  mBuildPlatform;    // this+0x78
    /*SerializedSkin mSkin;             // this+0x80
    bool           mIsTeacher;        // this+0x2E8
    bool           mIsHost;           // this+0x2E9#1#
};

class PlayerListPacket : public ::Packet {
public:
    std::vector<PlayerListEntry> mEntries; // this+0x30
    PlayerListPacketType         mAction;  // this+0x48
};*/