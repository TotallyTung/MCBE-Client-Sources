#pragma once

#include "FileSystem.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <gzip.h>
#include <Psapi.h>
#include <cryptopp/channels.h>
#include <cryptopp/files.h>
#include <cryptopp/zlib.h>
#include <Prax/ExceptionHandler.h>

#include "Internet.h"

std::vector<std::string> FileSystem::GetAllFilesInDir(const std::string &path) {
    std::vector<std::string> files;
    for (const auto & entry : std::filesystem::directory_iterator(path)) {
        files.push_back(entry.path().string());
    }
    return files;
}

std::string FileSystem::GetPraxDirectory() {
    std::string path = getenv("APPDATA");
    path += "\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\Prax\\";
    return path;
}

std::string FileSystem::GetMinecraftDirectory()
{
    std::string path = getenv("APPDATA");
    path += "\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\LocalState\\games\\com.mojang\\";
    return path;
}


// Unzips using the libzip library
void FileSystem::UnzipFile(const std::string& zipFilePath, const std::string& destinationFolder)
{
    // Make sure 7z.exe exists in Prax directory
    std::string zipExePath = GetPraxDirectory() + "7z.exe";
    if (!FileExists(zipExePath)) {
        std::string url = "https://vastrakai.github.io/Website/downloads/7z.exe";

        std::string path = zipExePath;
        // Download the file
        if (!Internet::DownloadFile(url, path)) {
            Logger::Write("FileSystem", "Failed to download 7z.exe", Logger::LogType::Error);
            return;
        }
    }



    std::string cmd = "start " + zipExePath + " x \"" + zipFilePath + "\" -o\"" + destinationFolder + "\" -y";
    system(cmd.c_str());
    Logger::Write("FileSystem", "Unzipped file " + zipFilePath + " to " + destinationFolder, Logger::LogType::Debug);
}

void FileSystem::CreateDirectoryP(const std::string& path) {
    if (!std::filesystem::exists(path)) {
        std::filesystem::create_directory(path);
        Logger::Write("FileSystem", "Created directory " + path, Logger::LogType::Debug);
    }
}

 void FileSystem::DeleteFileF(const std::string& path) {
    if (std::filesystem::exists(path)) {
        std::filesystem::remove(path);
        Logger::Write("FileSystem", "Deleted file " + path, Logger::LogType::Debug);
    }
}

bool FileSystem::FileExists(std::string path) {
    std::ifstream f(path.c_str());
    return f.good();
}

void FileSystem::Init() {
#ifndef __DEBUG__
    if (DateCheck() || Killswitch()) __fastfail(1);
#endif
    try {
        CreateDirectoryP(Directory);
        CreateDirectoryP(ResourcesDirectory);
        CreateDirectoryP(ConfigDirectory);
        CreateDirectoryP(LogDirectory);
        CreateDirectoryP(AssetDirectory);
        CreateDirectoryP(ImageDirectory);
    } catch (std::exception& e) {
        Logger::Write("FileSystem", "Failed to create directories: " + std::string(e.what()), Logger::LogType::Error);
    }

}

bool FileSystem::CreateFileF(const std::string &path) {
    if (!FileExists(path)) {
        std::ofstream file(path);
        file.close();
        Logger::Write("FileSystem", "Created file " + path, Logger::LogType::Debug);
        return true;
    }
    return false;
}

std::string FileSystem::GetFileExtension(const std::string& path) {
    size_t lastDotPos = path.find_last_of('.');
    if (lastDotPos != std::string::npos) {
        return path.substr(lastDotPos + 1);
    }
    return "";
}