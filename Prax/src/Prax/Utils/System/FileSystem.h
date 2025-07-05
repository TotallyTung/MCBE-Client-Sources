#pragma once

#include <string>
#include <vector>

class FileSystem {
public:
    static std::string GetPraxDirectory();
    static std::string GetMinecraftDirectory();
    static void UnzipFile(const std::string& zipFilePath, const std::string& destinationFolder);

    static inline std::string Directory = GetPraxDirectory();
    static inline std::string ResourcesDirectory = Directory + "\\resources";
    static inline std::string ConfigDirectory = Directory + "\\config";
    static inline std::string LogDirectory = Directory + "\\logs";
    static inline std::string AssetDirectory = Directory + "assets";
    static inline std::string ImageDirectory = AssetDirectory + "\\images";

    static std::vector<std::string> GetAllFilesInDir(const std::string& path);
    static void CreateDirectoryP(const std::string& path);
    static void Init();
    static bool FileExists(std::string path);
    static void DeleteFileF(const std::string &path);
    static bool CreateFileF(const std::string &path);
    static std::string GetFileExtension(const std::string &path);
};