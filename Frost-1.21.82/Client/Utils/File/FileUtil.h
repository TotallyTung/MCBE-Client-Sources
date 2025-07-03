#pragma once

namespace FileUtil {
    std::string getRoamingStatePath(void) {
        char* path = NULL;
        size_t length;

        _dupenv_s(&path, &length, "appdata");

        return std::string(path) + "\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\";
    };

    std::string getLocalStatePath(void) {
        char* path = NULL;
        size_t length;

        _dupenv_s(&path, &length, "appdata");

        return std::string(path) + "\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\LocalState\\";
    };

    std::string getClientPath(void) {
        std::string ClientFilePath = getRoamingStatePath() + "Frost\\";

        auto folderpath = std::filesystem::path(ClientFilePath);

        if (!std::filesystem::exists(folderpath)) {
            std::filesystem::create_directories(folderpath);
        }

        return ClientFilePath;
    }

    bool doesPathExist(std::string path) {
        return std::filesystem::exists(path);
    }

    void createPath(const std::string& path) {
        std::filesystem::create_directory(path);
    }

    void deletePath(const std::string& path) {
        std::filesystem::remove(path);
    }

    void downloadFile(std::string url, std::string path) {
        if (doesPathExist(path)) return;

        URLDownloadToFileA(nullptr, url.c_str(), path.c_str(), 0, NULL);
    }

    static void writeText(std::string filePath, std::string Message) {
        std::ifstream fileCheck(filePath.c_str());
        if (!fileCheck.good()) {
            fileCheck.close();
            std::ofstream fileCreate(filePath.c_str());
            if (fileCreate.is_open()) {
                fileCreate << Message;
                fileCreate.close();
            }
        }
    }

    std::string readFileContent(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary); // Open in binary mode
        std::string content;
        if (file.is_open()) {
            file.seekg(0, std::ios::end);               // Go to the end of the file
            size_t size = file.tellg();                 // Get the size of the file
            file.seekg(0, std::ios::beg);               // Go back to the beginning of the file
            content.resize(size);                       // Resize string to file size
            file.read(&content[0], size);               // Read the entire content
            file.close();
        }
        else {
            std::cerr << "Error opening file for reading!" << std::endl;
        }
        return content;
    }

    void InitializeClientFolders() {
        if (!doesPathExist(getClientPath() + "configurations\\")) {
            createPath(getClientPath() + "configurations\\");
        }
    }

    void debug(std::string output) {
        static bool init = false;
        static auto path = getClientPath();

        auto file = std::string(path + "output.txt");

        if (!init) {
            auto f = std::filesystem::path(file);

            if (std::filesystem::exists(f))
                std::filesystem::remove(f);

            init = true;
        };

        CloseHandle(CreateFileA(file.c_str(), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL));

        std::ofstream fStream;
        fStream.open(file.c_str(), std::ios::app);

        if (fStream.is_open())
            fStream << output << "" << std::endl;

        return fStream.close();

    };
}