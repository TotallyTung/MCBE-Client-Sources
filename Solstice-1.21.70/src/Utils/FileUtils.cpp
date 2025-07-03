//
// Created by vastrakai on 6/29/2024.
//

#include "FileUtils.hpp"

#include <filesystem>
#include <fstream>
#include <Windows.h>

#include "spdlog/spdlog.h"

std::string FileUtils::getRoamingStatePath()
{
    // Get the appdata environment variable
    char* appdata = nullptr;
    size_t len = 0;
    _dupenv_s(&appdata, &len, "APPDATA");
    if (appdata == nullptr)
        return "";
    return std::string(appdata) + "\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\";
}

std::string FileUtils::getSolsticeDir()
{
    return getRoamingStatePath() + xorstr_("Solstice\\");
}

bool FileUtils::fileExists(const std::string& path)
{
    return std::filesystem::exists(path);
}

void FileUtils::createDirectory(const std::string& path)
{
    if (!std::filesystem::exists(path))
    {
        std::filesystem::create_directory(path);
        spdlog::info("Created directory: {}", path);
    }
}

void FileUtils::validateDirectories()
{
    createDirectory(getSolsticeDir());
    createDirectory(getSolsticeDir() + "Configs\\");
    createDirectory(getSolsticeDir() + "Templates\\");
    createDirectory(getSolsticeDir() + "Databases\\");
    createDirectory(getSolsticeDir() + "Skins\\"); //(skin stealing)
    createDirectory(getSolsticeDir() + "BlinkerSkins\\"); //(skin blinking)
    createDirectory(getSolsticeDir() + "Audio\\");
    createDirectory(getSolsticeDir() + "Scripts\\");
    createDirectory(getSolsticeDir() + "Scripts\\Commands\\");
    createDirectory(getSolsticeDir() + "Scripts\\Modules\\");
    //createFile(getSolsticeDir() + "Scripts\\Libs\\");


    spdlog::info("Directories created successfully.");
}

bool FileUtils::deleteFile(const std::string& path)
{
    if (fileExists(path))
    {
        DeleteFileA(path.c_str());
        return true;
    }
    else
    {
        spdlog::error("Failed to delete file: {}", path);
        return false;
    }
}

void FileUtils::writeResourceToFile(Resource* resource, const std::string& path)
{
    writeResourceToFile(path, reinterpret_cast<const unsigned char*>(resource->data()), resource->size());
}

void FileUtils::writeResourceToFile(const std::string& path, const unsigned char* data, size_t size)
{
    std::ofstream file(path, std::ios::binary);
    file.write(reinterpret_cast<const char*>(data), size);
    file.close();
    spdlog::info("Wrote resource to file: {}", path);
};

std::vector<std::string> FileUtils::listFiles(const std::string& path)
{
    std::vector<std::string> files;

    WIN32_FIND_DATAA findFileData;
    HANDLE hFind = FindFirstFileA((path + "*").c_str(), &findFileData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        spdlog::error("Failed to list files in directory: {}", path);
        return files;
    }

    do
    {
        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            continue;

        files.push_back(findFileData.cFileName);
    } while (FindNextFileA(hFind, &findFileData));

    FindClose(hFind);

    return files;
}

void FileUtils::createFile(const std::string& path)
{
    std::ofstream file(path);
    file.close();
    spdlog::info("Created file: {}", path);
}

size_t FileUtils::getFileSize(const std::string& path)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    return file.tellg();
}

std::vector<unsigned char> FileUtils::readFile(const std::string& path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open())
    {
        spdlog::error("Failed to open file: {}", path);
        return {};
    }

    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> data(size);
    file.read(reinterpret_cast<char*>(data.data()), size);
    file.close();

    return data;
}
