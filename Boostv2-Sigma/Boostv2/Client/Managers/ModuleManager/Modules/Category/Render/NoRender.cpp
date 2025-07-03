#include "NoRender.h"

NoRender::NoRender() : Module("NoRender", "Don't render thing", Category::RENDER) {
	registerSetting(new BoolSetting("Fire", "NULL", &noFire, noFire));
	registerSetting(new BoolSetting("Weather", "Disable render weather", &noWeather, noWeather));
	registerSetting(new BoolSetting("Entities", "Disable render entities", &noEntities, noEntities));
	registerSetting(new BoolSetting("Block Entities", "Disable render block entities", &noBlockEntities, noBlockEntities));
	registerSetting(new BoolSetting("Particles", "Disable render particles", &noParticles, noParticles));
}

void NoRender::onDisable() {
	if (noWeatherBoolPtr != nullptr)
		*noWeatherBoolPtr = false;

	if (noEntitiesBoolPtr != nullptr)
		*noEntitiesBoolPtr = false;

	if (noBlockEntitiesBoolPtr != nullptr)
		*noBlockEntitiesBoolPtr = false;

	if (noParticlesBoolPtr != nullptr)
		*noParticlesBoolPtr = false;
}

void NoRender::onClientTick() {
	if (noWeatherBoolPtr != nullptr)
		*noWeatherBoolPtr = noWeather;

	if (noEntitiesBoolPtr != nullptr)
		*noEntitiesBoolPtr = noEntities;

	if (noBlockEntitiesBoolPtr != nullptr)
		*noBlockEntitiesBoolPtr = noBlockEntities;

	if (noParticlesBoolPtr != nullptr)
		*noParticlesBoolPtr = noParticles;
}