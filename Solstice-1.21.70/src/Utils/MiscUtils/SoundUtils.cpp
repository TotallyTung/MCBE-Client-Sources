//
// Created by vastrakai on 7/4/2024.
//

#include "SoundUtils.hpp"

#include <Utils/Resource.hpp>
#include <Utils/Resources.hpp>
#include "Audio.hpp"

#include "spdlog/spdlog.h"

Audio audioManager;


void SoundUtils::playSoundFromEmbeddedResource(std::string resourceName, float volume)
{
    static bool soundsDisabled = false;
    if (soundsDisabled) return;

    if(!TRY_CALL([&]() {
        if (!ResourceLoader::Resources.contains(resourceName))
        {
            spdlog::error("Resource {} not found", resourceName);
            return;
        }
        Resource& soundResource = ResourceLoader::Resources[resourceName];
        std::string path = FileUtils::getSolsticeDir() + "Audio\\" + resourceName;
        audioManager.BasePath = FileUtils::getSolsticeDir() + "Audio\\";
        if (!FileUtils::fileExists(path))
        {
            std::ofstream file(path, std::ios::binary);
            file.write(reinterpret_cast<const char*>(soundResource.data()), soundResource.size());
            file.close();
            spdlog::info("Dumped reosurce {} to {}", resourceName, path);
        }

        audioManager.Play(resourceName, volume, false);
    }))
    {
        NotifyUtils::notify("An error has occurred while trying to play " + resourceName + "\nSounds will be disabled for the rest of the session", 20.f, Notification::Type::Error);
        soundsDisabled = true;
    }
}