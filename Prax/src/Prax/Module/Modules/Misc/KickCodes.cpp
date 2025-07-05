//
// Created by vastrakai on 11/11/2023.
//

#include "KickCodes.h"

#include <base64.h>
#include <Prax/SDK/Game/Network/Other/DisconnectPacket.h>
#include <Prax/Utils/String.h>

#ifndef __RELEASEFREE__

std::map<uint8_t, std::string> flagNames = {
    { 0, "VerticalMotion" },
    { 2, "Highjump" },
    { 3, "Fly" },
    { 4, "Highjump" },
    { 7, "Phase" },
    { 9, "Speed" },
    { 10, "Bunnyhop" },
    { 14, "Combat" },
    { 15, "NoSwing" },
    { 25, "BadPackets" },
    { 27, "VClip" },
    { 28, "Autoclicker" },
    { 31, "Instabreak" },
    { 35, "ChestStealer" },
    { 36, "Blink" },
    { 37, "Spider" },
    { 40, "Scaffold" },
    { 42, "TimerLimit (Some weird yDelta check)"}, // dk fs
    { 43, "FastBreak" },
    { 11, "Strafe" },
    { 21, "ScaffoldRot" },
    { 16, "Reach" },
    { 8, "DownMotion" },
    { 5, "Motion" },
    { 13, "Motion_2_" },
    { 34, "InvalidGround (Step)"},
    { 44, "Mining" },
    { 45, "BadTick/Timer" },
    { 46, "Scaffold (Invalid face)" },
    {48, "Timer"}
};

std::map<uint8_t, std::string> typeMap = {
    { 0, "Single" },
    { 1, "A" },
    { 2, "B" },
    { 3, "C" },
    { 4, "D" },
    { 5, "E" },
    { 6, "F" },
    { 7, "G" },
    { 8, "H" },
    { 9, "I" },
    { 10, "J" },
    { 11, "K" },
    { 12, "L" },
    { 13, "M" },
    { 14, "N" },
    { 15, "O" },
    { 16, "P" },
    { 17, "Q" },
    { 18, "R" },
    { 19, "S" },
    { 20, "T" },
    { 21, "U" },
    { 22, "V" },
    { 23, "W" },
    { 24, "X" },
    { 25, "Y" },
    { 26, "Z" },
};

const std::string& GetFlagName(uint8_t flag)
{
    if (flagNames.contains(flag))
    {
        return flagNames[flag];
    }
    else
    {
        static std::string unmappedFlag = "Unmapped flag";
        return unmappedFlag;
    }
}

const std::string& GetType(uint8_t type)
{
    return typeMap[type];
}

void KickCodes::OnPacketReceive(std::shared_ptr<Packet> packet, bool* canceled)
{
    if (packet->getId() == (int)PacketID::Disconnect)
    {
        auto player = Minecraft::ClientInstance->ThePlayer();
        if (!player) return;
        auto disconnectPacket = std::reinterpret_pointer_cast<DisconnectPacket>(packet);
        std::string reason = disconnectPacket->message;
        // Make sure the reason starts with "Error: " (The rest of the msg is a base64 code)
        // Remove the "Error: " part
        if (!String::ContainsIgnoreCase(reason, "Error: ")) return;

        reason = reason.substr(10);

        disconnectPacket->message = "§a(Copied to clipboard)§r\n" + reason;
        // Copy the code to the clipboard
        Misc::SetClipboard(reason);

        // Get the raw bytes of this base64 code
        // Use cryptopp to decode the base64
        std::vector<uint8_t> bytes;
        CryptoPP::StringSource ss(reason, true,
            new CryptoPP::Base64Decoder(
                new CryptoPP::VectorSink(bytes)
            )
        );

        // Extract the first 8 bytes (the timestamp) based on this c# code
/*
        byte[] timestampBytes = new byte[8];
        Array.Copy(bytes, 0, timestampBytes, 0, 8);
        // Reverse the array
        Array.Reverse(timestampBytes);
        // Convert the array to a long
        long timestamp = BitConverter.ToInt64(timestampBytes, 0);
            */

        // TODO: add ms timestamp



        // Remove the first 24 bytes
        bytes.erase(bytes.begin(), bytes.begin() + 24);

        unsigned int size = bytes[0];
        unsigned int readIndex = 1;

        std::string flags = "";
        std::string coloredFlags = "";

        // Read the flags
        for (int i = 0; i < size; i++)
        {
            uint8_t flag = bytes[readIndex];
            uint8_t flagType = bytes[readIndex + 1];
            readIndex += 2;

            std::string flagName = GetFlagName(flag);
            std::string flagTypeStr = GetType(flagType);

            flags += flagName + " (Type: " + flagTypeStr + ")" + "\n";
            coloredFlags += "§a" + flagName + "§7 (Type: §c" + flagTypeStr + "§7)\n";
        }

        // Remove the last \n
        flags = flags.substr(0, flags.length() - 1);

        Logger::Write("Kick", std::to_string(size) + " check(s) flagged\n" + flags, Logger::LogType::Info);
        if (ShowReason.Enabled)
        {
            disconnectPacket->message =
                "§c[Flareon] §7Kicking " + player->localName + ", " +
                    std::to_string(size) + " check(s) flagged\n" + coloredFlags + "\n§r§7";
        }


    }
}

#endif