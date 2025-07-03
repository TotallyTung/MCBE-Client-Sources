#pragma once

class Ambience : public Module
{
public:
    Ambience(int keybind = Keys::NONE, bool enabled = false) :
        Module("Ambience", "Visual", "Changes the time and weather (clientside)", keybind, enabled)
    {
        addBool("Rain", "Make the weather raining", &rain);
        //addBool("Time", "Enabling this allows you to change the time of the day", &time);
        addSlider("RainLevel", "Change the level of the rain", &Rainlevel, 0.1, 5);
        addSlider("Time", "Change the time of the day", &Global::AmbienceTime, 0, 30000);
        //addBool("FullBright", "Makes the game brighter", &FullBright);

    }

    bool rain = false;
    bool time = true;
    float Rainlevel = 0.4;
    bool FullBright = false;
    

    void onEvent(RenderContextEvent* event) override
    {
        Player* player = Address::getLocalPlayer();
        ClientInstance* instance = Address::getClientInstance();

        // Check if player or valid key is null
        if (!player || !instance->getBlockSource())
            return;

        if (rain) {
            instance->getBlockSource()->getDimension()->getWeather()->rainLevel = Rainlevel;
        }
        else {
            instance->getBlockSource()->getDimension()->getWeather()->rainLevel = 0;

        // Inherited via IModule
           

        }
    }
public:
    float intensity = 25.f;
    float* gammaPtr = nullptr;
    
    float originalGamma = -1;

    void onTick(GameMode* gm) {
        if (gammaPtr != nullptr && *gammaPtr != 10)
            *gammaPtr = 10;
    }

    void onEnable() {
        if (FullBright) {
        }
        if (gammaPtr != nullptr && *gammaPtr != 10)
            originalGamma = *gammaPtr;
        *gammaPtr = 10;
    }

    void onDisable() {
        if (FullBright) {
        }
        if (gammaPtr != nullptr) {
            if (originalGamma >= 0 && originalGamma <= 1)
                *gammaPtr = originalGamma;
            else
                *gammaPtr = 0.5f;
        }
    }


};