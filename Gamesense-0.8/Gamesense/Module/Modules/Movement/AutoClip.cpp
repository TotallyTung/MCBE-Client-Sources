#include "AutoClip.h"
vec3_t* center_blockaa;
int y_levelaa = 0;
bool smooth = false;
using namespace std;
AutoClip::AutoClip() : IModule(0x0, Category::PLAYER, "Clip through the nearest block") {
	registerEnumSetting("Protocol", &mode, 0);
	mode.addEntry(EnumEntry("Strict", 0));
	mode.addEntry(EnumEntry("IGN", 1));
	registerBoolSetting("Smooth", &smooth, smooth, "Makes clipping smooth for less lagbacks");
}

AutoClip::~AutoClip() {
}

const char* AutoClip::getRawModuleName() {
	return ("AutoPhase");
}
const char* AutoClip::getModName() {
	if (mode.selected == 0)
	name = " Normal";
	else name = " IGN";
	return name;
}
vec3_t get_centeram(double posX, double posY, double posZ) {
	vec3_t xyz = vec3_t(floor(g_Data.getLocalPlayer()->getPos()->x), g_Data.getLocalPlayer()->getPos()->y, floor(g_Data.getLocalPlayer()->getPos()->z));

	xyz.x += 0.85f;
	xyz.z += 0.5f;
	if (!smooth)
		g_Data.getLocalPlayer()->setPos(xyz);
	else {
		float dist2 = g_Data.getLocalPlayer()->getPos()->dist(xyz);
		g_Data.getLocalPlayer()->lerpTo(xyz, vec2_t(1, 1), (int)fmax((int)dist2 * 0.1, 1));
		g_Data.getLocalPlayer()->lerpMotion(xyz);
	}
	return vec3_t(xyz.x, xyz.y, xyz.z);
}

vec3_t get_centeraam(double posX, double posY, double posZ) {
	vec3_t xyz = vec3_t(floor(g_Data.getLocalPlayer()->getPos()->x), g_Data.getLocalPlayer()->getPos()->y, floor(g_Data.getLocalPlayer()->getPos()->z));
	xyz.x += 0.15f;
	xyz.z += 0.5f;
	if (!smooth)
		g_Data.getLocalPlayer()->setPos(xyz);
	else {
		float dist2 = g_Data.getLocalPlayer()->getPos()->dist(xyz);
		g_Data.getLocalPlayer()->lerpTo(xyz, vec2_t(1, 1), (int)fmax((int)dist2 * 0.1, 1));
		g_Data.getLocalPlayer()->lerpMotion(xyz);
	}
	return vec3_t(xyz.x, xyz.y, xyz.z);
}

vec3_t get_centeraaam(double posX, double posY, double posZ) {
	vec3_t xyz = vec3_t(floor(g_Data.getLocalPlayer()->getPos()->x), g_Data.getLocalPlayer()->getPos()->y, floor(g_Data.getLocalPlayer()->getPos()->z));
	xyz.x += 0.5f;
	xyz.z += 0.85f;
	if (!smooth)
		g_Data.getLocalPlayer()->setPos(xyz);
	else {
		float dist2 = g_Data.getLocalPlayer()->getPos()->dist(xyz);
		g_Data.getLocalPlayer()->lerpTo(xyz, vec2_t(1, 1), (int)fmax((int)dist2 * 0.1, 1));
		g_Data.getLocalPlayer()->lerpMotion(xyz);
	}
	return vec3_t(xyz.x, xyz.y, xyz.z);
}

vec3_t get_centeraaaam(double posX, double posY, double posZ) {
	vec3_t xyz = vec3_t(floor(g_Data.getLocalPlayer()->getPos()->x), g_Data.getLocalPlayer()->getPos()->y, floor(g_Data.getLocalPlayer()->getPos()->z));
	xyz.x += 0.5f;
	xyz.z += 0.15f;
	if (!smooth)
		g_Data.getLocalPlayer()->setPos(xyz);
	else {
		float dist2 = g_Data.getLocalPlayer()->getPos()->dist(xyz);
		g_Data.getLocalPlayer()->lerpTo(xyz, vec2_t(1, 1), (int)fmax((int)dist2 * 0.1, 1));
		g_Data.getLocalPlayer()->lerpMotion(xyz);
	}
	return vec3_t(xyz.x, xyz.y, xyz.z);
}
vec3_t get_centera1m(double posX, double posY, double posZ) {
	vec3_t xyz = vec3_t(floor(g_Data.getLocalPlayer()->getPos()->x), g_Data.getLocalPlayer()->getPos()->y, floor(g_Data.getLocalPlayer()->getPos()->z));
	xyz.x += 0.85f;
	xyz.z += 0.85f;
	if (!smooth)
		g_Data.getLocalPlayer()->setPos(xyz);
	else {
		float dist2 = g_Data.getLocalPlayer()->getPos()->dist(xyz);
		g_Data.getLocalPlayer()->lerpTo(xyz, vec2_t(1, 1), (int)fmax((int)dist2 * 0.1, 1));
		g_Data.getLocalPlayer()->lerpMotion(xyz);
	}
	return vec3_t(xyz.x, xyz.y, xyz.z);
}

vec3_t get_centeraa1m(double posX, double posY, double posZ) {
	vec3_t xyz = vec3_t(floor(g_Data.getLocalPlayer()->getPos()->x), g_Data.getLocalPlayer()->getPos()->y, floor(g_Data.getLocalPlayer()->getPos()->z));
	xyz.x += 0.15f;
	xyz.z += 0.15f;
	if (!smooth)
		g_Data.getLocalPlayer()->setPos(xyz);
	else {
		float dist2 = g_Data.getLocalPlayer()->getPos()->dist(xyz);
		g_Data.getLocalPlayer()->lerpTo(xyz, vec2_t(1, 1), (int)fmax((int)dist2 * 0.1, 1));
		g_Data.getLocalPlayer()->lerpMotion(xyz);
	}
	return vec3_t(xyz.x, xyz.y, xyz.z);
}

vec3_t get_centeraaa1m(double posX, double posY, double posZ) {
	vec3_t xyz = vec3_t(floor(g_Data.getLocalPlayer()->getPos()->x), g_Data.getLocalPlayer()->getPos()->y, floor(g_Data.getLocalPlayer()->getPos()->z));
	xyz.x += 0.15f;
	xyz.z += 0.85f;
	if (!smooth)
		g_Data.getLocalPlayer()->setPos(xyz);
	else {
		float dist2 = g_Data.getLocalPlayer()->getPos()->dist(xyz);
		g_Data.getLocalPlayer()->lerpTo(xyz, vec2_t(1, 1), (int)fmax((int)dist2 * 0.1, 1));
		g_Data.getLocalPlayer()->lerpMotion(xyz);
	}
	return vec3_t(xyz.x, xyz.y, xyz.z);
}

vec3_t get_centeraaaa1m(double posX, double posY, double posZ) {
	vec3_t xyz = vec3_t(floor(g_Data.getLocalPlayer()->getPos()->x), g_Data.getLocalPlayer()->getPos()->y, floor(g_Data.getLocalPlayer()->getPos()->z));
	xyz.x += 0.85f;
	xyz.z += 0.15f;
	if (!smooth)
		g_Data.getLocalPlayer()->setPos(xyz);
	else {
		float dist2 = g_Data.getLocalPlayer()->getPos()->dist(xyz);
		g_Data.getLocalPlayer()->lerpTo(xyz, vec2_t(1, 1), (int)fmax((int)dist2 * 0.1, 1));
		g_Data.getLocalPlayer()->lerpMotion(xyz);
	}
	return vec3_t(xyz.x, xyz.y, xyz.z);
}

vec3_t get_centeramc(double posX, double posY, double posZ) {
	vec3_t xyz = vec3_t(floor(g_Data.getLocalPlayer()->getPos()->x), g_Data.getLocalPlayer()->getPos()->y, floor(g_Data.getLocalPlayer()->getPos()->z));

	xyz.x += 0.9999f;
	xyz.z += 0.5f;
	if (!smooth)
		g_Data.getLocalPlayer()->setPos(xyz);
	else {
		float dist2 = g_Data.getLocalPlayer()->getPos()->dist(xyz);
		g_Data.getLocalPlayer()->lerpTo(xyz, vec2_t(1, 1), (int)fmax((int)dist2 * 0.1, 1));
		g_Data.getLocalPlayer()->lerpMotion(xyz);
	}
	return vec3_t(xyz.x, xyz.y, xyz.z);
}

vec3_t get_centeraamc(double posX, double posY, double posZ) {
	vec3_t xyz = vec3_t(floor(g_Data.getLocalPlayer()->getPos()->x), g_Data.getLocalPlayer()->getPos()->y, floor(g_Data.getLocalPlayer()->getPos()->z));
	xyz.x += 0.f;
	xyz.z += 0.5f;
	if (!smooth)
		g_Data.getLocalPlayer()->setPos(xyz);
	else {
		float dist2 = g_Data.getLocalPlayer()->getPos()->dist(xyz);
		g_Data.getLocalPlayer()->lerpTo(xyz, vec2_t(1, 1), (int)fmax((int)dist2 * 0.1, 1));
		g_Data.getLocalPlayer()->lerpMotion(xyz);
	}
	return vec3_t(xyz.x, xyz.y, xyz.z);
}

vec3_t get_centeraaamc(double posX, double posY, double posZ) {
	vec3_t xyz = vec3_t(floor(g_Data.getLocalPlayer()->getPos()->x), g_Data.getLocalPlayer()->getPos()->y, floor(g_Data.getLocalPlayer()->getPos()->z));
	xyz.x += 0.5f;
	xyz.z += 0.9999f;
	if (!smooth)
		g_Data.getLocalPlayer()->setPos(xyz);
	else {
		float dist2 = g_Data.getLocalPlayer()->getPos()->dist(xyz);
		g_Data.getLocalPlayer()->lerpTo(xyz, vec2_t(1, 1), (int)fmax((int)dist2 * 0.1, 1));
		g_Data.getLocalPlayer()->lerpMotion(xyz);
	}
	return vec3_t(xyz.x, xyz.y, xyz.z);
}

vec3_t get_centeraaaamc(double posX, double posY, double posZ) {
	vec3_t xyz = vec3_t(floor(g_Data.getLocalPlayer()->getPos()->x), g_Data.getLocalPlayer()->getPos()->y, floor(g_Data.getLocalPlayer()->getPos()->z));
	xyz.x += 0.5f;
	xyz.z += 0.f;
	if (!smooth)
		g_Data.getLocalPlayer()->setPos(xyz);
	else {
		float dist2 = g_Data.getLocalPlayer()->getPos()->dist(xyz);
		g_Data.getLocalPlayer()->lerpTo(xyz, vec2_t(1, 1), (int)fmax((int)dist2 * 0.1, 1));
		g_Data.getLocalPlayer()->lerpMotion(xyz);
	}
	return vec3_t(xyz.x, xyz.y, xyz.z);
}
vec3_t get_centera1mc(double posX, double posY, double posZ) {
	vec3_t xyz = vec3_t(floor(g_Data.getLocalPlayer()->getPos()->x), g_Data.getLocalPlayer()->getPos()->y, floor(g_Data.getLocalPlayer()->getPos()->z));
	xyz.x += 0.9999f;
	xyz.z += 0.9999f;
	if (!smooth)
		g_Data.getLocalPlayer()->setPos(xyz);
	else {
		float dist2 = g_Data.getLocalPlayer()->getPos()->dist(xyz);
		g_Data.getLocalPlayer()->lerpTo(xyz, vec2_t(1, 1), (int)fmax((int)dist2 * 0.1, 1));
		g_Data.getLocalPlayer()->lerpMotion(xyz);
	}
	return vec3_t(xyz.x, xyz.y, xyz.z);
}

vec3_t get_centeraa1mc(double posX, double posY, double posZ) {
	vec3_t xyz = vec3_t(floor(g_Data.getLocalPlayer()->getPos()->x), g_Data.getLocalPlayer()->getPos()->y, floor(g_Data.getLocalPlayer()->getPos()->z));
	xyz.x += 0.f;
	xyz.z += 0.f;
	if (!smooth)
		g_Data.getLocalPlayer()->setPos(xyz);
	else {
		float dist2 = g_Data.getLocalPlayer()->getPos()->dist(xyz);
		g_Data.getLocalPlayer()->lerpTo(xyz, vec2_t(1, 1), (int)fmax((int)dist2 * 0.1, 1));
		g_Data.getLocalPlayer()->lerpMotion(xyz);
	}
	return vec3_t(xyz.x, xyz.y, xyz.z);
}

vec3_t get_centeraaa1mc(double posX, double posY, double posZ) {
	vec3_t xyz = vec3_t(floor(g_Data.getLocalPlayer()->getPos()->x), g_Data.getLocalPlayer()->getPos()->y, floor(g_Data.getLocalPlayer()->getPos()->z));
	xyz.x += 0.f;
	xyz.z += 0.9999f;
	if (!smooth)
		g_Data.getLocalPlayer()->setPos(xyz);
	else {
		float dist2 = g_Data.getLocalPlayer()->getPos()->dist(xyz);
		g_Data.getLocalPlayer()->lerpTo(xyz, vec2_t(1, 1), (int)fmax((int)dist2 * 0.1, 1));
		g_Data.getLocalPlayer()->lerpMotion(xyz);
	}
	return vec3_t(xyz.x, xyz.y, xyz.z);
}

vec3_t get_centeraaaa1mc(double posX, double posY, double posZ) {
	vec3_t xyz = vec3_t(floor(g_Data.getLocalPlayer()->getPos()->x), g_Data.getLocalPlayer()->getPos()->y, floor(g_Data.getLocalPlayer()->getPos()->z));
	xyz.x += 0.9999f;
	xyz.z += 0.f;
	if (!smooth)
		g_Data.getLocalPlayer()->setPos(xyz);
	else {
		float dist2 = g_Data.getLocalPlayer()->getPos()->dist(xyz);
		g_Data.getLocalPlayer()->lerpTo(xyz, vec2_t(1, 1), (int)fmax((int)dist2 * 0.1, 1));
		g_Data.getLocalPlayer()->lerpMotion(xyz);
	}
	return vec3_t(xyz.x, xyz.y, xyz.z);
}

void AutoClip::onTick(C_GameMode* gm) {
	auto freecam = moduleMgr->getModule<Freecam>();
	vec3_t entPos = g_Data.getLocalPlayer()->getPos()->floor();
	entPos.y -= 1;
	if (g_Data.getLocalPlayer() != nullptr) {
		if (abc) {
			if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 1, entPos.y, entPos.z))->toLegacy()->blockId == 7 && g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z + 1))->toLegacy()->blockId == 7) {
				if (!freecam->isEnabled()) {
					if (mode.selected == 0)
						center_blockaa = &get_centera1m(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
					else center_blockaa = &get_centera1mc(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
				}

				gm->player->aabb.upper.y = gm->player->aabb.lower.y;
			}
			else if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x - 1, entPos.y, entPos.z))->toLegacy()->blockId == 7 && g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z - 1))->toLegacy()->blockId == 7) {
				if (!freecam->isEnabled()) {
					if (mode.selected == 0)
						center_blockaa = &get_centeraa1m(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
					else center_blockaa = &get_centeraa1mc(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
				}
				gm->player->aabb.upper.y = gm->player->aabb.lower.y;
			}
			else if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z + 1))->toLegacy()->blockId == 7 && g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x - 1, entPos.y, entPos.z))->toLegacy()->blockId == 7) {
				if (!freecam->isEnabled()) {
					if (mode.selected == 0)
						center_blockaa = &get_centeraaa1m(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
					else center_blockaa = &get_centeraaa1mc(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
				}
				gm->player->aabb.upper.y = gm->player->aabb.lower.y;
			}
			else if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z - 1))->toLegacy()->blockId == 7 && g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 1, entPos.y, entPos.z))->toLegacy()->blockId == 7) {
				if (!freecam->isEnabled()) {
					if (mode.selected == 0)
						center_blockaa = &get_centeraaaa1m(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
					else center_blockaa = &get_centeraaaa1mc(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
				}
				gm->player->aabb.upper.y = gm->player->aabb.lower.y;
			}
			//
			else if ((g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z - 1))->toLegacy()->blockId == 49 || g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z - 1))->toLegacy()->blockId == 7) && (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 1, entPos.y, entPos.z))->toLegacy()->blockId == 49 || g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 1, entPos.y, entPos.z))->toLegacy()->blockId == 7)) {
				if (!freecam->isEnabled()) {
					if (mode.selected == 0)center_blockaa = &get_centeraaaa1m(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
					else center_blockaa = &get_centeraaaa1mc(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
				}
				gm->player->aabb.upper.y = gm->player->aabb.lower.y;
			}
			else if ((g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 1, entPos.y, entPos.z))->toLegacy()->blockId == 49 || g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 1, entPos.y, entPos.z))->toLegacy()->blockId == 7) && (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z + 1))->toLegacy()->blockId == 49 || g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z + 1))->toLegacy()->blockId == 7)) {
				if (!freecam->isEnabled()) {
					if (mode.selected == 0)center_blockaa = &get_centera1m(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
					else center_blockaa = &get_centera1mc(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
				}
				gm->player->aabb.upper.y = gm->player->aabb.lower.y;
			}

			else if ((g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z + 1))->toLegacy()->blockId == 49 || g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z + 1))->toLegacy()->blockId == 7) && (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x - 1, entPos.y, entPos.z))->toLegacy()->blockId == 49 || g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x - 1, entPos.y, entPos.z))->toLegacy()->blockId == 7)) {
				if (!freecam->isEnabled()) {
					if (mode.selected == 0)center_blockaa = &get_centeraaa1m(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
					else center_blockaa = &get_centeraaa1mc(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
				}
				gm->player->aabb.upper.y = gm->player->aabb.lower.y;
			}
			else if ((g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x - 1, entPos.y, entPos.z))->toLegacy()->blockId == 49 || g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x - 1, entPos.y, entPos.z))->toLegacy()->blockId == 7) && (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z - 1))->toLegacy()->blockId == 49 || g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z - 1))->toLegacy()->blockId == 7)) {
				if (!freecam->isEnabled()) {
					if (mode.selected == 0)center_blockaa = &get_centeraa1m(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
					else center_blockaa = &get_centeraa1mc(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
				}
				gm->player->aabb.upper.y = gm->player->aabb.lower.y;
			}


			//
			else if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 1, entPos.y, entPos.z))->toLegacy()->blockId == 7) {
				if (!freecam->isEnabled()) {
					if (mode.selected == 0)center_blockaa = &get_centeram(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
					else center_blockaa = &get_centeramc(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
				}
				gm->player->aabb.upper.y = gm->player->aabb.lower.y;
			}
			else if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x - 1, entPos.y, entPos.z))->toLegacy()->blockId == 7) {
				if (!freecam->isEnabled()) {
					if (mode.selected == 0)center_blockaa = &get_centeraam(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
					else center_blockaa = &get_centeraamc(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
				}
				gm->player->aabb.upper.y = gm->player->aabb.lower.y;
			}
			else if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z + 1))->toLegacy()->blockId == 7) {
				if (!freecam->isEnabled()) {
					if (mode.selected == 0)center_blockaa = &get_centeraaam(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
					else center_blockaa = &get_centeraaamc(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
				}
				gm->player->aabb.upper.y = gm->player->aabb.lower.y;
			}
			else if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z - 1))->toLegacy()->blockId == 7) {
				if (!freecam->isEnabled()) {
					if (mode.selected == 0)center_blockaa = &get_centeraaaam(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
					else center_blockaa = &get_centeraaaamc(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
				}
				gm->player->aabb.upper.y = gm->player->aabb.lower.y;
			}
			else if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 1, entPos.y, entPos.z))->toLegacy()->blockId == 49) {
				if (!freecam->isEnabled()) {
					if (mode.selected == 0)center_blockaa = &get_centeram(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
					else center_blockaa = &get_centeramc(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
				}
				gm->player->aabb.upper.y = gm->player->aabb.lower.y;
			}
			else if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x - 1, entPos.y, entPos.z))->toLegacy()->blockId == 49) {
				if (!freecam->isEnabled()) {
					if (mode.selected == 0)center_blockaa = &get_centeraam(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
					else center_blockaa = &get_centeraamc(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
				}
				gm->player->aabb.upper.y = gm->player->aabb.lower.y;
			}
			else if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z + 1))->toLegacy()->blockId == 49) {
				if (!freecam->isEnabled()) {
					if (mode.selected == 0)center_blockaa = &get_centeraaam(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
					else center_blockaa = &get_centeraaamc(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
				}
				gm->player->aabb.upper.y = gm->player->aabb.lower.y;
			}
			else if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z - 1))->toLegacy()->blockId == 49) {
				if (!freecam->isEnabled()) {
					if (mode.selected == 0)center_blockaa = &get_centeraaaam(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
					else center_blockaa = &get_centeraaaamc(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
				}
				gm->player->aabb.upper.y = gm->player->aabb.lower.y;
			}
			else if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 1, entPos.y, entPos.z))->toLegacy()->blockId == 7 && g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z + 1))->toLegacy()->blockId == 7 && (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 1, entPos.y, entPos.z + 1))->toLegacy()->blockId == 7 || g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 1, entPos.y, entPos.z + 1))->toLegacy()->blockId == 49)) {
				if (!freecam->isEnabled()) {
					if (mode.selected == 0)center_blockaa = &get_centera1m(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
					else center_blockaa = &get_centera1mc(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
				}
				gm->player->aabb.upper.y = gm->player->aabb.lower.y;
			}
			else if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x - 1, entPos.y, entPos.z))->toLegacy()->blockId == 7) {
				if (!freecam->isEnabled()) {
					if (mode.selected == 0)center_blockaa = &get_centeraam(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
					else center_blockaa = &get_centeraamc(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
				}
				gm->player->aabb.upper.y = gm->player->aabb.lower.y;
			}
			else if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z + 1))->toLegacy()->blockId == 7) {
				if (!freecam->isEnabled()) {

					if (mode.selected == 0)center_blockaa = &get_centeraaam(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
					else center_blockaa = &get_centeraaamc(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
				}
				gm->player->aabb.upper.y = gm->player->aabb.lower.y;
			}
			else if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z - 1))->toLegacy()->blockId == 7) {
				if (!freecam->isEnabled()) {
					if (mode.selected == 0)center_blockaa = &get_centeraaaam(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
					else center_blockaa = &get_centeraaaamc(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
				}
				gm->player->aabb.upper.y = gm->player->aabb.lower.y;
			}
			else if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 1, entPos.y, entPos.z))->toLegacy()->blockId == 49) {
				if (!freecam->isEnabled()) {
					if (mode.selected == 0)center_blockaa = &get_centeram(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
					else center_blockaa = &get_centeramc(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
				}
				gm->player->aabb.upper.y = gm->player->aabb.lower.y;
			}
			else if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x - 1, entPos.y, entPos.z))->toLegacy()->blockId == 49) {
				if (!freecam->isEnabled()) {
					if (mode.selected == 0)center_blockaa = &get_centeraam(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
					else center_blockaa = &get_centeraamc(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
				}
				gm->player->aabb.upper.y = gm->player->aabb.lower.y;
			}
			else if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z + 1))->toLegacy()->blockId == 49) {
				if (!freecam->isEnabled()) {
					if (mode.selected == 0)center_blockaa = &get_centeraaam(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
					else center_blockaa = &get_centeraaamc(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
				}
				gm->player->aabb.upper.y = gm->player->aabb.lower.y;
			}
			else if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z - 1))->toLegacy()->blockId == 49) {
				if (!freecam->isEnabled()) {
					if (mode.selected == 0)center_blockaa = &get_centeraaaam(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
					else center_blockaa = &get_centeraaaamc(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
				}
				gm->player->aabb.upper.y = gm->player->aabb.lower.y;
			}
			y_levelaa = (int)round(g_Data.getLocalPlayer()->getPos()->y);
		}
		else if (gm->player->aabb.upper.y = gm->player->aabb.lower.y)g_Data.getLocalPlayer()->aabb.upper.y += 1.8f;



	}
}

void AutoClip::onDisable() {
	vec3_t entPos = g_Data.getLocalPlayer()->getPos()->floor();
	entPos.y -= 1;
	if (g_Data.getLocalPlayer() != nullptr)

		if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 1, entPos.y, entPos.z))->toLegacy()->blockId == 7) {
			g_Data.getLocalPlayer()->aabb.upper.y += 1.8f;
		}
		else if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x - 1, entPos.y, entPos.z))->toLegacy()->blockId == 7) {
			g_Data.getLocalPlayer()->aabb.upper.y += 1.8f;
		}

		else if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z + 1))->toLegacy()->blockId == 7) {
			g_Data.getLocalPlayer()->aabb.upper.y += 1.8f;
		}
		else if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z - 1))->toLegacy()->blockId == 7) {
			g_Data.getLocalPlayer()->aabb.upper.y += 1.8f;
		}
		else if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 1, entPos.y, entPos.z))->toLegacy()->blockId == 49) {
			g_Data.getLocalPlayer()->aabb.upper.y += 1.8f;
		}
		else if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x - 1, entPos.y, entPos.z))->toLegacy()->blockId == 49) {
			g_Data.getLocalPlayer()->aabb.upper.y += 1.8f;
		}
		else if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z + 1))->toLegacy()->blockId == 49) {
			g_Data.getLocalPlayer()->aabb.upper.y += 1.8f;
		}
		else if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z - 1))->toLegacy()->blockId == 49) {
			g_Data.getLocalPlayer()->aabb.upper.y += 1.8f;
		}
	vec3_t xyz = vec3_t(floor(g_Data.getLocalPlayer()->getPos()->x), g_Data.getLocalPlayer()->getPos()->y, floor(g_Data.getLocalPlayer()->getPos()->z));
	xyz.x += 0.5f;
	xyz.z += 0.5f;
	if (!smooth)
		g_Data.getLocalPlayer()->setPos(xyz);
	else {
		float dist2 = g_Data.getLocalPlayer()->getPos()->dist(xyz);
		g_Data.getLocalPlayer()->lerpTo(xyz, vec2_t(1, 1), (int)fmax((int)dist2 * 0.1, 1));
		g_Data.getLocalPlayer()->lerpMotion(xyz);
	}


}

void AutoClip::onEnable() {
}

void AutoClip::onMove(C_MoveInputHandler* hand) {
	auto player = g_Data.getLocalPlayer();

	if (hand->isJumping) {
		abc = false;
	}
	else

		if (hand->forward) {
			abc = false;
		}
		else
			if (hand->backward) {
				abc = false;
			}
			else
				if (hand->left) {
					abc = false;
				}
				else
					if (hand->right) {
						abc = false;
					}
					else abc = true;
}
