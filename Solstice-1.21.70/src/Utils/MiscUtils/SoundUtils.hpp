#pragma once
//
// Created by vastrakai on 7/4/2024.
//

#include <string>
#include <Utils/Resource.hpp>
#include <Windows.h>

class SoundUtils {
public:
    static void playSoundFromEmbeddedResource(std::string resourceName, float volume);
};
