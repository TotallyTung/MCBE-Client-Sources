#pragma once
#include "../../../Utils/MemoryUtil.h"
#include "../../Network/PacketSender.h"
#include "../Physical/HitResult.h"
#include <vector>

class Actor;
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

class PlayerListEntry {
public:
	uint64_t mId; // This is the ActorUniqueID
	mce::UUID mUuid;
	std::string mName, mXUID, mPlatformOnlineId;
	BuildPlatform mBuildPlatform;

	PlayerListEntry() {
		mId = 0;
		mUuid = mce::UUID();
		mName = "";
		mXUID = "";
		mPlatformOnlineId = "";
		mBuildPlatform = BuildPlatform::Unknown;
	}
};

#pragma once

#include <cstdint>
#include <concepts>
#include <libhat.hpp>
class Level {
public:

	std::unordered_map<mce::UUID, PlayerListEntry>& getPlayerMap() {
		return hat::member_at<std::unordered_map<mce::UUID, PlayerListEntry>>(this, 0x1BC8);
	}

	std::vector<Actor*> getRuntimeActorList() {
		std::vector<Actor*> listOut;
		MemoryUtil::CallVFunc<278, decltype(&listOut)>(this, &listOut);
		return listOut;
	}

	PacketSender* getPacketSender() {
		return MemoryUtil::CallVFunc<281, PacketSender*>(this);
	}

	HitResult* getHitResult() {
		return MemoryUtil::CallVFunc<288, HitResult*>(this);
	}
};
