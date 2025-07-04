#include "NoRender.h"

NoRender::NoRender() : Module("NoRender", "Stop certain animations & unrender objects/entites.", Category::RENDER) {
	addBoolCheck("Fire", "Disable fire overlay.", &noFire);
	addBoolCheck("Weather", "Disable render weather.", &noWeather);
	addBoolCheck("Sky", "Disable render Sky.", &noSky);
	addBoolCheck("Terrain", "Disable render Terrain.", &NoTerrain);
	addBoolCheck("Entities", "Disable render entities", &noEntities);
	addBoolCheck("Item in hand", "Disable render item in hand", &Noiteminhand);
	//addBoolCheck("menu Paperdoll", "Disable render menu thingi", &NoMainMenuPaperdollAnimation);
	//addBoolCheck("MenuCube Map", "Disable render menu thingi", &NoMainMenuCubeMap);
	addBoolCheck("Block Entities", "Disable render block entities.", &noBlockEntities);
	addBoolCheck("Particles", "Disable render particles.", &noParticles);
	addBoolCheck("HUD", "Disable render Hud.", &NoHUD);
}