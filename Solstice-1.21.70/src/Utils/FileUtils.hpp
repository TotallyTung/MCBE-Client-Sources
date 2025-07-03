#pragma once
//
// Created by vastrakai on 6/29/2024.
//

#include <string>
#include <vector>

class FileUtils {
public:
    static std::string getRoamingStatePath();
    static std::string getSolsticeDir();
    static bool fileExists(const std::string& path);
    static void createDirectory(const std::string& path);
    static void validateDirectories();
    static bool deleteFile(const std::string& path);
    static void writeResourceToFile(Resource* resource, const std::string& path);
    static void writeResourceToFile(const std::string& path, const unsigned char* data, size_t size);
    static std::vector<std::string> listFiles(const std::string& path);
    static void createFile(const std::string& path);
    // getFileSize
    static size_t getFileSize(const std::string& path);
    //readFile
    static std::vector<unsigned char> readFile(const std::string& path);
};
