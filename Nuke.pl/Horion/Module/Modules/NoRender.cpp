#include "NoRender.h"

NoRender::NoRender() : IModule(0, Category::VISUAL, "Stops certain things from rendering in the game") {
	registerBoolSetting("BlockEntities", &blockEntities, blockEntities);
	registerBoolSetting("Particles", &particles, particles);
	registerBoolSetting("Entities", &entities, entities);
	registerBoolSetting("Weather", &weather, weather);
	registerBoolSetting("Fire", &fire, fire);
}

const char *NoRender::getModuleName() {
	return "NoRender";
}