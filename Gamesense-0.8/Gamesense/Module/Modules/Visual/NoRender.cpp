#include "NoRender.h"

using namespace std;
NoRender::NoRender() : IModule(0, Category::VISUAL, "Stops certain things from rendering in the game")
{
    registerBoolSetting("NoWeather", &noweather, noweather);
    registerBoolSetting("BlockEntities", &blockEntities, blockEntities);
    registerBoolSetting("Particles", &particles, particles);
    registerBoolSetting("Entities", &entities, entities);
}

const char *NoRender::getRawModuleName()
{
    return "NoRender";
}

const char *NoRender::getModuleName()
{
    return "NoRender";
}
