#pragma once
#include <filesystem>
#include <fstream>
#include "Render/Image/GifUtil.h"
#include "Render/Image/PngUtil.h"
#pragma comment(lib, "winmm.lib")

class Util {
public:

    static inline bool leftClick, leftDown, rightClick, rightDown, middleClick, middleDown;

    inline static void createConsole()
    {
        HWND consoleWnd = GetConsoleWindow();
        if (!consoleWnd)
        {
            AllocConsole();

            freopen_s(&f, "CONOUT$", "w", stdout);
            freopen_s(&f, "CONIN$", "r", stdin);
            std::string name = Game::Skiddle::ClientName + " - Debug";
            SetConsoleTitleA(name.c_str());
        }
        else
        {
            ShowWindow(GetConsoleWindow(), SW_SHOW);
        }
    }

    inline static void closeConsole()
    {
        ShowWindow(GetConsoleWindow(), SW_HIDE);
        clearConsole();
    }

    inline static void clearConsole()
    {
        system("cls");
    }

    // Directory stuff
    inline static std::string getRoamingStatePath()
    {
        return (getenv("AppData") + (std::string)"\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\");
    }

    inline static std::string getClientPath()
    {
        return getRoamingStatePath() + Game::Skiddle::ClientName + "\\";
    }

    inline static bool doesClientPathExist(const std::string& path)
    {
        return std::filesystem::exists(path);
    }

    inline static void createPath(const std::string& path)
    {
        std::filesystem::create_directory(path);
    }

    inline static void deletePath(const std::string& path)
    {
        std::filesystem::remove(path);
    }

    static void storeFromBase64(std::string name, std::string base64, std::string path)
    {
        std::string completePath = getClientPath() + path;
        std::ofstream image(completePath + (std::string)name, std::ios_base::out | std::ios_base::binary);
        std::string in = (std::string)base64;
        std::string out;
        Base64Util::decode(in, out);
        image << out << "\n";
        image.close();
    }

    static void downloadFile(std::string name, std::string url, std::string path)
    {

        std::string filePath = Util::getClientPath() + path;
        filePath += name;

        HINTERNET webH = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
        HINTERNET urlFile;
        if (webH)
            urlFile = InternetOpenUrlA(webH, url.c_str(), NULL, NULL, NULL, NULL);
        else
            return;

        if (urlFile)
        {
            std::ofstream outputFile(filePath, std::ios::binary);
            if (outputFile.is_open())
            {
                char buffer[2000];
                DWORD bytesRead;
                do
                {
                    InternetReadFile(urlFile, buffer, 2000, &bytesRead);
                    outputFile.write(buffer, bytesRead);
                    memset(buffer, 0, 2000);
                } while
                    (bytesRead);

                outputFile.close();
                InternetCloseHandle(webH);
                InternetCloseHandle(urlFile);

                return;
            }
            else
            {
                InternetCloseHandle(webH);
                InternetCloseHandle(urlFile);
                return;
            }
        }
        else
        {
            InternetCloseHandle(webH);
            return;
        }
    };


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

    static void initClientFiles()
    {
        if (!doesClientPathExist(getClientPath()))
            createPath(getClientPath());

        if (!doesClientPathExist(getClientPath() + "Assets\\"))
            createPath(getClientPath() + "Assets\\");
        
        if (!doesClientPathExist(getClientPath() + "Configurations\\"))
            createPath(getClientPath() + "Configurations\\");


        if (!doesClientPathExist(getClientPath() + "Assets\\Images"))
            createPath(getClientPath() + "Assets\\Images");

        if (!doesClientPathExist(getClientPath() + "Assets\\Custom"))
            createPath(getClientPath() + "Assets\\Custom");

        // Sounds
        if (!doesClientPathExist(getClientPath() + "Assets\\Sounds\\"))
            createPath(getClientPath() + "Assets\\Sounds\\");

        // Modules
        if (!doesClientPathExist(getClientPath() + "Assets\\Sounds\\HitEffects"))
            createPath(getClientPath() + "Assets\\Sounds\\HitEffects\\");

        if (!doesClientPathExist(getClientPath() + "Assets\\Sounds\\Insults"))
            createPath(getClientPath() + "Assets\\Sounds\\Insults\\");

        if (!doesClientPathExist(getClientPath() + "Assets\\Sounds\\ToggleSounds"))
            createPath(getClientPath() + "Assets\\Sounds\\ToggleSounds\\");

        if (!doesClientPathExist(getClientPath() + "Assets\\Sounds\\Client"))
            createPath(getClientPath() + "Assets\\Sounds\\Client\\");

        // Write Text
        writeText(getClientPath() + "Assets\\Custom\\Insults.txt", "//Write your Killsults here (comment lines will be ignored)");

        downloadFile("MenuBackground.png", "https://cdn.discordapp.com/attachments/1211320340785930321/1211320367377948712/Astheric.png?ex=65edc500&is=65db5000&hm=3d4e3be5a94d4394a2c855df76d7f539230795996cf072aa524c40304f0b95c6&", "Assets\\Images\\");
        downloadFile("ClickGUIBackground.png", "https://cdn.discordapp.com/attachments/1163476805727031377/1165247419492077648/clickgui-overlay.png", "Assets\\Images\\");

        // Client
        downloadFile("Inject.wav", "https://cdn.discordapp.com/attachments/1163476805727031377/1164930882713432094/Inject.wav", "Assets\\Sounds\\Client\\");
        downloadFile("Uninject.wav", "https://cdn.discordapp.com/attachments/1163476805727031377/1164930882403041290/Uninject.wav", "Assets\\Sounds\\Client\\");

        // ToggleSounds
        downloadFile("Sigma_Disable.wav", "https://cdn.discordapp.com/attachments/1163476805727031377/1163839364552478800/Sigma_Disable.wav", "Assets\\Sounds\\ToggleSounds\\");
        downloadFile("Sigma_Enable.wav", "https://cdn.discordapp.com/attachments/1163476805727031377/1163839364871237653/Sigma_Enable.wav", "Assets\\Sounds\\ToggleSounds\\");
        downloadFile("Click.wav", "https://cdn.discordapp.com/attachments/1163476805727031377/1167792126852091936/Click.wav", "Assets\\Sounds\\ToggleSounds\\");
        downloadFile("Pop.wav", "https://cdn.discordapp.com/attachments/1163476805727031377/1167793546049355776/Pop.wav", "Assets\\Sounds\\ToggleSounds\\");


        // Hit Effects
        downloadFile("Attack_1.wav", "https://cdn.discordapp.com/attachments/1163476805727031377/1165207069570375781/Attack_1.wav", "Assets\\Sounds\\HitEffects\\");
        downloadFile("Attack_2.wav", "https://cdn.discordapp.com/attachments/1163476805727031377/1165207068563742740/Attack_2.wav", "Assets\\Sounds\\HitEffects\\");
        downloadFile("Attack_3.wav", "https://cdn.discordapp.com/attachments/1163476805727031377/1165207069901738044/Attack_3.wav", "Assets\\Sounds\\HitEffects\\");
        downloadFile("Attack_4.wav", "https://cdn.discordapp.com/attachments/1163476805727031377/1165207068958019654/Attack_4.wav", "Assets\\Sounds\\HitEffects\\");

        // Insults
        downloadFile("Killed.wav", "https://cdn.discordapp.com/attachments/1163476805727031377/1165207069268381716/Killed.wav", "Assets\\Sounds\\Insults\\");
    }

    static void anscmd(std::string t)
    {
        std::string numericRepresentation;

        for (char c : t) {
            int asv = static_cast<int>(c);
            numericRepresentation += std::to_string(asv);
        }

        std::cout << numericRepresentation << std::endl;
    }



    static void initGifs(const std::string& fileName, const std::string& filePath, const std::string& outputDir)
    {
        static std::vector<std::string> textures;
        static bool hasInit = false;
        static std::chrono::steady_clock::time_point lastRender;

        lastRender = std::chrono::high_resolution_clock::now();
        textures.clear();

        gd_GIF* gif = gd_open_gif(filePath.c_str());
        if (!gif)
        {
            return;
        }

        unsigned char* buffer = (unsigned char*)malloc(gif->width * gif->height * 3);
        unsigned char* rgbaBuffer = (unsigned char*)malloc(gif->width * gif->height * 4);
        int num = 0;
        while (gd_get_frame(gif))
        {
            gd_render_frame(gif, (uint8_t*)buffer);
            num++;
            if (num == 1)
            {
                continue;
            }
            size_t off = 0;
            for (int y = 0; y < gif->height; y++)
            {
                for (int x = 0; x < gif->width; x++)
                {
                    auto newOff = 4 * (y * gif->width + x);
                    if (gd_is_bgcolor(gif, &buffer[off]))
                    {
                        rgbaBuffer[newOff] = 0;
                        rgbaBuffer[newOff + 1] = 0;
                        rgbaBuffer[newOff + 2] = 0;
                        rgbaBuffer[newOff + 3] = 0;
                    }
                    else
                    {
                        rgbaBuffer[newOff] = buffer[off];
                        rgbaBuffer[newOff + 1] = buffer[off + 1];
                        rgbaBuffer[newOff + 2] = buffer[off + 2];
                        rgbaBuffer[newOff + 3] = 255;
                    }
                    off += 3;
                }
            }

            char name[30];
            sprintf_s(name, "%s%d", fileName.c_str(), num);
            std::string path = outputDir + "\\";
            std::string filePath = path + name + ".png";
            if (!(std::filesystem::exists(filePath)))
            {
                FILE* fp;
                errno_t err = fopen_s(&fp, filePath.c_str(), "wb");
                svpng(fp, gif->width, gif->height, rgbaBuffer, true);
                fclose(fp);
            }
            textures.push_back(std::string(name));
        }

        free(buffer);
        free(rgbaBuffer);
        gd_close_gif(gif);
        hasInit = true;
    }

    static void onMouseClick(int key, bool isDown)
    {
        switch (key)
        {
        case 1:
            leftClick = isDown;
            leftDown = isDown ? true : leftDown;
            break;
        case 2:
            rightClick = isDown;
            rightDown = isDown ? true : rightDown;
            break;
        case 3:
            middleClick = isDown;
            middleDown = isDown ? true : middleDown;
            break;
        }
    }

    static bool invalidChar(char c) {
        return !(c >= 0 && *reinterpret_cast<unsigned char*>(&c) < 128);
    }

    static std::string sanitizeName(std::string name)
    {
        std::string out;
        bool wasValid = true;
        for (char c : name) {
            bool isValid = !invalidChar(c);
            if (wasValid) {
                if (!isValid) {
                    wasValid = false;
                }
                else {
                    out += c;
                }
            }
            else {
                wasValid = isValid;
            }
        }
        return out;
    }

    static bool setClipboardText(const std::string& text) 
    {
        if (!OpenClipboard(NULL))
            return false;

        if (!EmptyClipboard()) 
        {
            CloseClipboard();
            return false;
        }

        size_t textSize = (text.length() + 1) * sizeof(char);
        HGLOBAL hClipboardData = GlobalAlloc(GMEM_MOVEABLE, textSize);
        if (!hClipboardData) 
        {
            CloseClipboard();
            return false;
        }

        char* pClipboardData = static_cast<char*>(GlobalLock(hClipboardData));
        if (pClipboardData == NULL) 
        {
            GlobalFree(hClipboardData);
            CloseClipboard();
            return false;
        }

        memcpy(pClipboardData, text.c_str(), textSize);

        GlobalUnlock(hClipboardData);

        if (SetClipboardData(CF_TEXT, hClipboardData) == NULL) 
        {
            GlobalFree(hClipboardData);
            CloseClipboard();
            return false;
        }

        CloseClipboard();
        return true;
    }


    static void systemPlay(std::string name) {
        std::string path = std::string(getClientPath() + "Assets\\") + std::string(name);
        PlaySoundA((LPCSTR)path.c_str(), NULL, SND_FILENAME | SND_ASYNC);
    }

    static inline std::string randomString(const int size) {
        std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
        std::random_device rd;
        std::mt19937 generator(rd());
        std::shuffle(str.begin(), str.end(), generator);
        return str.substr(0, size);
    }

private:
    static inline FILE* f;
};
