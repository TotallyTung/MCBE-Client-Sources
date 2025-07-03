#include "BlockScan.h"
int range = 50;
float posY = 380.f;
bool tracer = true;
bool outline = true;
bool reset = true;
bool fill = false;

float lo = 0.7f;
float fo = 0.1f;
int minus = 0;
bool EEnderChest = true;
std::vector<Vec3> ScannedPos;
BlockScan::BlockScan() : IModule(0, Category::VISUAL, "Show Block, BlockCounts, Block Trace, Block ESP") {
	registerBoolSetting("EnderChest", &EEnderChest, true);

	registerFloatSetting("LineOpacity", &lo, 0.7f, 0.0f, 1.f);
	registerFloatSetting("FillOpacity", &fo, 0.1f, 0.0f, 1.f);
	registerBoolSetting("Reset", &reset, true);
	registerBoolSetting("Outline", &outline, true);
	registerBoolSetting("Tracer", &tracer, true);
	registerBoolSetting("Fill", &fill, false);
	registerIntSetting("Range", &range, range, 1, 200);
	registerFloatSetting("Pos-Y", &posY, 2.5f, 0.f, Game.getClientInstance()->getGuiData()->windowSizeReal.y);
}

const char* BlockScan::getModuleName() {
	return ("BlockScan");
}
int id = 0;
std::shared_ptr<glmatrixf> refdef2;
static std::vector<Entity*> targetListbs;

void findEntitybs(Entity* currentEntitybs, bool isRegularEntity) {
	if (currentEntitybs == nullptr)
		return;
	if (currentEntitybs == Game.getLocalPlayer())  // Skip Local player
		return;
	if (!Game.getLocalPlayer()->canAttack(currentEntitybs, false))
		return;
	if (!currentEntitybs->isAlive())
		return;
	if (currentEntitybs->getEntityTypeId() != 317)
		return;
	float dist = (*currentEntitybs->getPos()).dist(*Game.getLocalPlayer()->getPos());
	if (dist < range) {
		targetListbs.push_back(currentEntitybs);
	}
}
void BlockScan::onTick(GameMode* gm) {
	targetListbs.clear();
	if (Game.getLocalPlayer() == nullptr) return;
	Game.forEachEntity(findEntitybs);
};
void BlockScan::onPostRender(MinecraftUIRenderContext* renderCtx) {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;
	int portal = 0;
	int barrel = 0;
	int shulker = 0;
	int chest = 0;
	int trapchest = 0;
	int furnace = 0;
	int bs = 0;
	int as = 0;
	int dropper = 0;
	int hopper = 0;
	int frame = 0;
	int enderchest = 0;
	for (Entity* Poop : targetListbs) {
		as += 1;
	}
	for (Vec3 pppp : ScannedPos) {
		id = (int)player->region->getBlock(pppp)->toLegacy()->blockId;
		if (id == 90) {
			portal += 1;
		}
		else if (id == 205 || id == 218) {
			shulker += 1;
		}
		else if (id == 54) {
			chest += 1;
		}
		else if (id == 146) {
			trapchest += 1;
		}
		else if (id == 61) {
			furnace += 1;
		}
		else if (id == 199 || id == 594) {
			frame += 1;
		}
		else if (id == 154) {
			hopper += 1;
		}
		else if (id == 125) {
			dropper += 1;
		}
		else if (id == 117) {
			bs += 1;
		}
		else if (id == 458) {
			barrel += 1;
		}
		else if (id == 130) {
			enderchest += 1;
		};
	}
	if (Game.canUseMoveKeys()) {
		Vec4 RectPos = Vec4(0.5f, posY, 0.5f + 90, posY + 120);
		if (!EEnderChest) {
			RectPos = Vec4(0.5f, posY, 0.5f + 90, posY + 110);
		};
		DrawUtils::drawRectangle(Vec4(RectPos.x, RectPos.y, RectPos.z, RectPos.w), MC_Color(0, 255, 200), 255);
		DrawUtils::fillRectangleA(RectPos, MC_Color(0, 0, 0, 100));
		if (EEnderChest) {
			DrawUtils::drawText(Vec2(0.f + 3, posY + 1), &std::string(
				"Portal: " + std::to_string(portal) +
				"\nShulker: " + std::to_string(shulker) +
				"\nBarrel: " + std::to_string(barrel) +
				"\nChest: " + std::to_string(chest) +
				"\nTrap Chest: " + std::to_string(trapchest) +
				"\nFurnace: " + std::to_string(furnace) +
				"\nDopper: " + std::to_string(dropper) +
				"\nHopper: " + std::to_string(hopper) +
				"\nbrewing stand: " + std::to_string(bs) +
				"\nFrame: " + std::to_string(frame) +
				"\nArmor Stand: " + std::to_string(as) +
				"\nEnderChest: " + std::to_string(enderchest)
			), MC_Color(255, 255, 255), 1.f, 1.f, false);
		}
		else {
			DrawUtils::drawText(Vec2(0.f + 3, posY + 1), &std::string(
				"Portal: " + std::to_string(portal) +
				"\nShulker: " + std::to_string(shulker) +
				"\nBarrel: " + std::to_string(barrel) +
				"\nChest: " + std::to_string(chest) +
				"\nTrap Chest: " + std::to_string(trapchest) +
				"\nFurnace: " + std::to_string(furnace) +
				"\nDopper: " + std::to_string(dropper) +
				"\nHopper: " + std::to_string(hopper) +
				"\nbrewing stand: " + std::to_string(bs) +
				"\nFrame: " + std::to_string(frame) +
				"\nArmor Stand: " + std::to_string(as)
			), MC_Color(255, 255, 255), 1.f, 1.f, false);
		}
	}
}
void BlockScan::onPreRender(MinecraftUIRenderContext* renderCtx) {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;

	if (Game.canUseMoveKeys()) {
		for (Entity* PoopNaEntity : targetListbs) {
			DrawUtils::setColor(1.f, 0.f, 0.f, lo);
			DrawUtils::drawEntityBox(PoopNaEntity, lo, false);

			if (fill) {
				DrawUtils::setColor(0.f, 1.f, 0.0f, fo);
				DrawUtils::drawEntityBox(PoopNaEntity, fo, true);
			}
		};
	}
};
void BlockScan::onLevelRender() {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;
	if (reset) {
		ScannedPos.clear();
		reset = false;
	};
	if (Game.canUseMoveKeys()) {
		Vec3* pos = player->getPos();
		for (Entity* ppppEntity : targetListbs) {
			Vec3 pppp = ppppEntity->getPos()->floor();
			DrawUtils::setColor(1.f, 1.f, 1.f, lo);
			if (tracer) {
				float calcYaw = (player->yaw + 90) * (PI / 180);
				float calcPitch = (player->pitch) * -(PI / 180);

				Vec3 moveVec;
				moveVec.x = cos(calcYaw) * cos(calcPitch) * 0.5f;
				moveVec.y = sin(calcPitch) * 0.5f;
				moveVec.z = sin(calcYaw) * cos(calcPitch) * 0.5f;

				const Vec3 origin = Game.getClientInstance()->levelRenderer->getOrigin().add(moveVec);
				float pos_x = (float)pppp.x;
				float pos_y = (float)pppp.y;
				float pos_z = (float)pppp.z;
				DrawUtils::drawLine3d(origin, Vec3(pos_x + 0.5f, pos_y + 0.5f, pos_z + 0.5f));
			}

			DrawUtils::setColor(0.85f, 0.f, 1.f, lo);
			DrawUtils::drawEntityBox(ppppEntity, lo, fill);
		}
		for (Vec3 pppp : ScannedPos) {
			float dist = (pppp).dist(pos->floor());
			bool clear = true;
			if (dist < range) {
				clear = false;
			}
			else if (clear) {
				ScannedPos.clear();
			}
			id = (int)player->region->getBlock(pppp)->toLegacy()->blockId;
			if (id == 90 || id == 218 || id == 205 || id == 117 || id == 125 || id == 154 || id == 199 || id == 61 || id == 146 || id == 54 || id == 458 || id == 130) {
				DrawUtils::setColor(1.f, 1.f, 1.f, lo);
				if (tracer) {
					float calcYaw = (player->yaw + 90) * (PI / 180);
					float calcPitch = (player->pitch) * -(PI / 180);

					Vec3 moveVec;
					moveVec.x = cos(calcYaw) * cos(calcPitch) * 0.5f;
					moveVec.y = sin(calcPitch) * 0.5f;
					moveVec.z = sin(calcYaw) * cos(calcPitch) * 0.5f;

					const Vec3 origin = Game.getClientInstance()->levelRenderer->getOrigin().add(moveVec);
					float pos_x = (float)pppp.x;
					float pos_y = (float)pppp.y;
					float pos_z = (float)pppp.z;
					DrawUtils::drawLine3d(origin, Vec3(pos_x + 0.5f, pos_y + 0.5f, pos_z + 0.5f));
				}
			};
			if (id == 90) {
				Vec3 lower = Vec3(pppp.x + 0.35f, pppp.y, pppp.z);
				Vec3 upper = Vec3(pppp.x + 0.65f, pppp.y + 1.f, pppp.z + 1.f);
				DrawUtils::setColor(0.85f, 0.f, 1.f, lo);
				if (outline) {
					DrawUtils::drawBox3d(lower, upper, 1.f, 1);
				};
				DrawUtils::setColor(0.f, 0.3f, 1.f, fo);
				if (fill) {
					DrawUtils::drawBox3dFilled(lower, upper, 1.f, false, 1);
				};
			}
			else if (id == 205 || id == 218) {
				Vec3 lower = Vec3(pppp.x + 1.f, pppp.y, pppp.z + 1.f);
				Vec3 upper = Vec3(pppp.x, pppp.y + 1.f, pppp.z);
				DrawUtils::setColor(1.f, 1.f, 1.f, lo);
				if (outline) {
					DrawUtils::drawBox3d(lower, upper, 1.f, 1);
				};
				DrawUtils::setColor(0.65f, 0.f, 1.f, fo);
				if (fill) {
					DrawUtils::drawBox3dFilled(lower, upper, 1.f, false, 1);
				};
			}
			else if (id == 54 || id == 146) {
				Vec3 lower = Vec3(pppp.x + 0.95f, pppp.y, pppp.z + 0.95f);
				Vec3 upper = Vec3(pppp.x + 0.07f, pppp.y + 0.9f, pppp.z + 0.07f);
				DrawUtils::setColor(1.f, 0.8f, 0.f, lo);
				if (outline) {
					DrawUtils::drawBox3d(lower, upper, 1.f, 1);
				};
				DrawUtils::setColor(0.f, 0.27f, 0.27f, fo);
				if (fill) {
					DrawUtils::drawBox3dFilled(lower, upper, 1.f, false, 1);
				};
			}
			else if (id == 199 || id == 594) {
				Vec3 lower = Vec3(pppp.x + 0.85f, pppp.y, pppp.z + 0.85f);
				Vec3 upper = Vec3(pppp.x + 0.15f, pppp.y + 0.08f, pppp.z + 0.15f);
				DrawUtils::setColor(0.63f, 0.57f, 0.4f, lo);
				if (outline) {
					DrawUtils::drawBox3d(lower, upper, 1.f, 1);
				};
				DrawUtils::setColor(0.31f, 0.16f, 0.09f, fo);
				if (fill) {
					DrawUtils::drawBox3dFilled(lower, upper, 1.f, false, 1);
				};
			}
			else if (id == 458) {
				Vec3 lower = Vec3(pppp.x + 1.f, pppp.y, pppp.z + 1.f);
				Vec3 upper = Vec3(pppp.x, pppp.y + 1.f, pppp.z);
				DrawUtils::setColor(0.09f, 0.08f, 0.06f, lo);
				if (outline) {
					DrawUtils::drawBox3d(lower, upper, 1.f, 1);
				};
				DrawUtils::setColor(0.60f, 0.43f, 0.24f, fo);
				if (fill) {
					DrawUtils::drawBox3dFilled(lower, upper, 1.f, false, 1);
				};
			}
			else if (id == 117) {
				Vec3 lower = Vec3(pppp.x + 1.f, pppp.y, pppp.z + 1.f);
				Vec3 upper = Vec3(pppp.x, pppp.y + 1.f, pppp.z);
				DrawUtils::setColor(0.85f, 0.8f, 0.24f, lo);
				if (outline) {
					DrawUtils::drawBox3d(lower, upper, 1.f, 1);
				};
				DrawUtils::setColor(0.45f, 0.42f, 0.42f, fo);
				if (fill) {
					DrawUtils::drawBox3dFilled(lower, upper, 1.f, false, 1);
				};
			}
			else if (id == 125) {
				Vec3 lower = Vec3(pppp.x + 1.f, pppp.y, pppp.z + 1.f);
				Vec3 upper = Vec3(pppp.x, pppp.y + 1.f, pppp.z);
				DrawUtils::setColor(0.f, 0.f, 0.f, lo);
				if (outline) {
					DrawUtils::drawBox3d(lower, upper, 1.f, 1);
				};
				DrawUtils::setColor(0.25f, 0.25f, 0.25f, fo);
				if (fill) {
					DrawUtils::drawBox3dFilled(lower, upper, 1.f, false, 1);
				};
			}
			else if (id == 61) {
				Vec3 lower = Vec3(pppp.x + 1.f, pppp.y, pppp.z + 1.f);
				Vec3 upper = Vec3(pppp.x, pppp.y + 1.f, pppp.z);
				DrawUtils::setColor(1.f, 1.f, 1.f, lo);
				if (outline) {
					DrawUtils::drawBox3d(lower, upper, 1.f, 1);
				};
				DrawUtils::setColor(0.25f, 0.25f, 0.25f, fo);
				if (fill) {
					DrawUtils::drawBox3dFilled(lower, upper, 1.f, false, 1);
				};
			}
			else if (id == 130) {
				Vec3 lower = Vec3(pppp.x + 0.95f, pppp.y, pppp.z + 0.95f);
				Vec3 upper = Vec3(pppp.x + 0.07f, pppp.y + 0.9f, pppp.z + 0.07f);
				DrawUtils::setColor(0.34f, 0.43f, 0.38f, lo);
				if (outline) {
					DrawUtils::drawBox3d(lower, upper, 1.f, 1);
				};
				DrawUtils::setColor(0.7f, 0.f, 1.f, fo);
				if (fill) {
					DrawUtils::drawBox3dFilled(lower, upper, 1.f, false, 1);
				};
			}
			else if (id == 154) {
				Vec3 lower = Vec3(pppp.x + 0.95f, pppp.y, pppp.z + 0.95f);
				Vec3 upper = Vec3(pppp.x + 0.07f, pppp.y + 0.9f, pppp.z + 0.07f);
				DrawUtils::setColor(0.37f, 0.37f, 0.37f, lo);
				if (outline) {
					DrawUtils::drawBox3d(lower, upper, 1.f, 1);
				};
				DrawUtils::setColor(0.14f, 0.14f, 0.14f, fo);
				if (fill) {
					DrawUtils::drawBox3dFilled(lower, upper, 1.f, false, 1);
				};
			}
		};
		for (int x = (int)pos->x - range; x < pos->x + range - 10; x++) {
			for (int z = (int)pos->z - range; z < pos->z + range - 10; z++) {
				for (int y = (int)pos->y - range - range * 0.6f; y < pos->y + range - 10; y++) {
					Vec3 blockPosition = Vec3(x, y, z);
					id = (int)player->region->getBlock(blockPosition)->toLegacy()->blockId;
					if (id == 90 || id == 218 || id == 205 || id == 117 || id == 125 || id == 154 || id == 199 || id == 594 || id == 61 || id == 146 || id == 54 || id == 458 || id == 130) {
						bool add = true;
						for (Vec3 pppp : ScannedPos) {
							if (blockPosition == pppp) {
								add = false;
							}
						};
						if (add) {
							ScannedPos.push_back(blockPosition);

						}
					}
				}
			}
		}
	}
}