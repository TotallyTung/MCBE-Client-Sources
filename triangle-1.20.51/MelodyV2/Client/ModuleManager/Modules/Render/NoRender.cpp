#include "NoRender.h"

NoRender::NoRender() : Module("NoRender", "Stop certain animations & unrender objects/entites.", Category::RENDER) {
	boolsetting("Fire", "Disable fire overlay.", &noFire);
	boolsetting("Weather", "Disable render weather.", &noWeather);
	boolsetting("Sky", "Disable render Sky.", &noSky);
	boolsetting("Terrain", "Disable render Terrain.", &NoTerrain);
	boolsetting("Entities", "Disable render entities", &noEntities);
	boolsetting("Item in hand", "Disable render item in hand", &Noiteminhand);
	//boolsetting("menu Paperdoll", "Disable render menu thingi", &NoMainMenuPaperdollAnimation);
	//boolsetting("MenuCube Map", "Disable render menu thingi", &NoMainMenuCubeMap);
	boolsetting("Block Entities", "Disable render block entities.", &noBlockEntities);
	boolsetting("Particles", "Disable render particles.", &noParticles);
	boolsetting("HUD", "Disable render Hud.", &NoHUD);
}